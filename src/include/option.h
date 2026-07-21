#ifndef OPTION_H
#define OPTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define OPT_RESIZE 5

// Equivalent to main
typedef int(*OptCaller)(int c, char** v);

typedef struct Opt{
    size_t count; // argc
    char** vector; // argv + n
    int offset;
    char* name;
    char* description;
    struct Opt** children;
    size_t childrenSize;
    bool mandatory;
    bool parsed;
    bool valided;
    OptCaller callback;
}Opt;


typedef struct OptList{
    Opt** values;
    size_t size;
    size_t occupied;
}OptList;




OptList* opt_listInit(void);
bool opt_listInited(OptList* list);
void opt_listFree(OptList* list);

#define opt_listQuit(list) do{\
    opt_listFree(list);\
    list = NULL;\
}while(0);
bool opt_listHasOpt(OptList* list, const char* name);
Opt* opt_listGetOpt(OptList* list, const char* name);

bool opt_listResize(OptList* list, size_t size);

bool opt_listPushOpt(OptList* list, const char* name, const char* description, bool mandatory, size_t count, OptCaller callback);
bool opt_listPushChild(OptList* list, const char* name, const char* cname, const char* description, bool mandatory, size_t count, OptCaller callback);
void opt_listParse(OptList* list, int argc, char** argv);

Opt* opt_optInit(const char* name, const char* description, bool mandatory, size_t count, OptCaller callback);
bool opt_optInited(Opt* opt);
void opt_optFree(Opt* opt);
bool opt_optHasOpt(Opt* opt, const char* name);
Opt* opt_optGetOpt(Opt* opt, const char* name);
void opt_optParse(Opt* opt, int argc, char** argv);
int opt_optExecute(Opt* opt);
bool opt_optPushOpt(Opt* opt, const char* name, const char* description, bool mandatory, size_t count, OptCaller callback);
#ifdef __cplusplus
}
#endif

#endif