#include <stdio.h>

#include "option.h"

OptList* opt_listInit(void){

    OptList* tmp = calloc(1, sizeof(OptList));
    if(tmp == NULL){
        return NULL;
    }

    tmp->values = calloc(1, sizeof(Opt*));
    if(tmp->values == NULL){
        free(tmp);
        tmp = NULL;
        return NULL;
    }

    tmp->size = 1;
    tmp->occupied = 0;
    return tmp;
}

bool opt_listInited(OptList* list){
    if(list == NULL || list->size == 0){
        return false;
    }
    return true;
}

void opt_listFree(OptList* list){
    if(opt_listInited(list) == false){
        return;
    }

    for(size_t i=0; i < list->size; i++){
        opt_optFree(list->values[i]);
        list->values[i] = NULL;
    }

    free(list->values);
    list->values = NULL;
    free(list);
    list = NULL;
}

bool opt_listHasOpt(OptList* list, const char* name){
    if(list == NULL || opt_listInited(list) == false || name == NULL || strcmp(name, "") == 0){
        return false;
    }

    for(size_t i=0; i < list->size; i++){
        if(list->values[i] == NULL){
            continue;
        }

        if(strcmp(list->values[i]->name, name) == 0){
            return true;
        }

        if(opt_optHasOpt(list->values[i], name) == true){
            return true;
        }
    }
    return false;
}

Opt* opt_listGetOpt(OptList* list, const char* name){
    if(list == NULL || opt_listInited(list) == false || name == NULL || strcmp(name, "") == 0){
        return NULL;
    }

    for(size_t i=0; i < list->size; i++){
        if(list->values[i] == NULL){
            continue;
        }

        if(strcmp(list->values[i]->name, name) == 0){
            return list->values[i];
        }

        Opt* tmp = opt_optGetOpt(list->values[i], name);
        if(tmp != NULL){
            return tmp;
        }
    }
    return NULL;
}

bool opt_listResize(OptList* list, size_t size){
    if(list == NULL || opt_listInited(list) == false){
        return false;
    }

    if(size <= list->size){
        size = list->size + OPT_RESIZE;
    }


    Opt** tmp = realloc(list->values, size*sizeof(Opt*));
    if(tmp == NULL){
        return false;
    }

    for(size_t i = list->size; i<size; i++){
        tmp[i] = NULL;
    }

    list->values = tmp;
    list->size = size;
    return true;
}

bool opt_listPushOpt(OptList* list, const char* name, const char* description, bool mandatory, size_t count, OptCaller callback){
    if(opt_listInited(list) == false || name == NULL || strcmp(name, "") == 0){
        return false;
    }

    // No need to add this option: already present
    if (opt_listHasOpt(list, name) == true){
        return true;
    }

    if(list->occupied == list->size){
        if(opt_listResize(list, list->size + OPT_RESIZE) == false){
            return false;
        }
    }

    Opt* tmp = opt_optInit(name, description, mandatory, count, callback);
    if(opt_optInited(tmp) == false){
        return false;
    }

    for(size_t i = 0; i< list->size; i++){
        if(list->values[i] == NULL){
            list->values[i] = tmp;
            list->occupied++;
            return true;
        }
    }

    return false;
}

bool opt_listPushChild(OptList* list, const char* name, const char* cname, const char* description, bool mandatory, size_t count, OptCaller callback){
    if(list == NULL || opt_listInited(list) == false || name == NULL || strcmp(name, "") == 0 || cname == NULL || strcmp(cname, "") == 0 || description == NULL){
        return false;
    }

    Opt* parent = opt_listGetOpt(list, name);
    if(parent == NULL){
        return false;
    }

    if(opt_optHasOpt(parent, cname) == true){
        return true;
    }

    return opt_optPushOpt(parent, cname, description, mandatory, count, callback);
}

void opt_listParse(OptList* list, int argc, char** argv){
    if(list == NULL || opt_listInited(list) == false || argv == NULL){
        return;
    }
    // For each opts in the list, find matches
    for(size_t i=0; i< list->size; i++){
        if(list->values[i] == NULL){
            continue;
        }
        opt_optParse(list->values[i], argc, argv);
    }
}

Opt* opt_optInit(const char* name, const char* description, bool mandatory, size_t count, OptCaller callback){
    if(name == NULL || strcmp(name, "") == 0 || description == NULL){
        return NULL;
    }

    Opt* tmp = calloc(1, sizeof(Opt));
    if(tmp == NULL){
        return NULL;
    }

    // Can be null
    tmp->callback = callback;
    tmp->children = calloc(1, sizeof(Opt*));
    tmp->childrenSize = 0;
    if(tmp->children == NULL){
        free(tmp);
        return NULL;
    }

    tmp->count = count;
    tmp->offset = 0;

    tmp->description = NULL;
    size_t lenDescription = strlen(description);
    tmp->description = calloc(lenDescription+1, sizeof(char));
    if(tmp->description == NULL){
        free(tmp->children);
        free(tmp);
        return NULL;
    }
    strcpy(tmp->description, description);

    tmp->mandatory = mandatory;
    
    tmp->name = NULL;
    size_t lenName = strlen(name);
    tmp->name = calloc(lenName+1, sizeof(char));
    if(tmp->name == NULL){
        free(tmp->description);
        free(tmp->children);
        free(tmp);
        return NULL;
    }
    strcpy(tmp->name, name);

    tmp->parsed = false;
    tmp->valided = false;
    tmp->vector = NULL;
    return tmp;
}

bool opt_optInited(Opt* opt){
    if(opt == NULL || opt->name == NULL){
        return false;
    }

    return true;
}


void opt_optFree(Opt* opt){
    if(opt == NULL || opt_optInited(opt) == false){
        return;
    }


    opt->callback = NULL;
    for(size_t i=0; i< opt->childrenSize; i++){
        opt_optFree(opt->children[i]);
        opt->children[i] = NULL;
    }
    free(opt->children);
    opt->children = NULL;
    opt->childrenSize = 0;

    opt->count = 0;
    opt->offset = 0;
    if(opt->description != NULL){
        free(opt->description);
        opt->description = NULL;
    }
    free(opt->name);
    opt->name = NULL;

    opt->mandatory = false;
    opt->parsed=false;
    opt->valided=false;
    opt->vector = NULL;
    free(opt);
}


bool opt_optHasOpt(Opt* opt, const char* name){
    if(opt == NULL || opt_optInited(opt) == false || name == NULL || strcmp(name, "")==0){
        return false;
    }

    if(opt->children == NULL && opt->childrenSize == 0){
        return false;
    }

    for(size_t i=0; i < opt->childrenSize; i++){
        if(strcmp(opt->children[i]->name, name) == 0){
            return true;
        }
    }


    return false;
}


Opt* opt_optGetOpt(Opt* opt, const char* name){
    if(opt == NULL || opt_optInited(opt) == false || name == NULL || strcmp(name, "") == 0 ){
        return NULL;
    }

    if(opt->children == NULL && opt->childrenSize == 0){
        return NULL;
    }

    for(size_t i=0; i < opt->childrenSize; i++){
        if(opt->children[i] == NULL){
            continue;
        }
        if(strcmp(opt->children[i]->name, name) == 0){
            return opt->children[i];
        }
    }

    return NULL;
}


void opt_optParse(Opt* opt, int argc, char** argv){
    if(opt == NULL || opt_optInited(opt) == false || argc == 0 || argv == NULL){
        return;
    }
    
    opt->parsed = false;
    opt->valided = false;

    // Parse all parameters on this option
    for(size_t i=0; i<argc; i++){
        // Security on argv
        if(argv[i] == NULL){
            continue;
        }

        if(strcmp(argv[i], opt->name) == 0){
            opt->parsed = true;
            opt->vector = argv+i;
            opt->offset = i;
        }
    }

    // Here the opt is just missing
    if(opt->parsed == false && opt->mandatory == true){
        printf("Missing required arg : %s\t%s\n", opt->name, opt->description);
        opt->valided = false;
        return;
    }

    // Missing parameters detection
    if(opt->offset + opt->count >= argc){
        printf("Missing parameters for option %s, at least need %ld\n", opt->name, opt->count);
        opt->valided = false;
        return;
    }

    // sub opts parse
    if(opt->parsed == true && opt->children != NULL){
        for(size_t i=0; i< opt->childrenSize; i++){
            if(opt->children[i] == NULL){
                continue;
            }

            opt_optParse(opt->children[i], argc, argv);
        }
    }
    if(opt->parsed == true){
        opt->valided = true;
    }
}

int opt_optExecute(Opt* opt){
    if(opt == NULL || opt_optInited(opt) == false){
        return 0;
    }

    if(opt->callback != NULL){
        return opt->callback(opt->count+1, opt->vector);
    }
    return 0;
}

bool opt_optPushOpt(Opt* opt, const char* name, const char* description, bool mandatory, size_t count, OptCaller callback){
    if(opt == NULL || opt_optInited(opt) == false || name == NULL || strcmp(name, "") == 0 || description == NULL){
        return false;
    }

    if(opt_optHasOpt(opt, name) == true){
        return true;
    }


    Opt** tmpChildren = realloc(opt->children, (opt->childrenSize+1)*sizeof(Opt*));
    if(tmpChildren == NULL){
        return false;
    }

    opt->children = tmpChildren;
    opt->children[opt->childrenSize] = NULL;

    Opt* tmpOpt = opt_optInit(name, description, mandatory, count, callback);
    if(tmpOpt == NULL){
        return false;
    }

    opt->children[opt->childrenSize] = tmpOpt;
    opt->childrenSize++;
    return true;
}