#include <stdio.h>
#include <stdlib.h>
#include <valgrind/memcheck.h>

#include "assert.h"
#include "option.h"

void OptListInit(void);
void OptListAdd(void);
void OptListParse(int argc, char** argv);

int main(int argc, char** argv){

    TEST(OptListInit());
    TEST(OptListAdd());
    TEST(OptListParse(argc, argv));
}

void OptListInit(void){
    OptList* tmp = opt_listInit();
    
    ASSERT(tmp != NULL);
    ASSERT(tmp->size != 0);
    opt_listQuit(tmp);

    ASSERT(tmp == NULL);
}


void OptListAdd(void){
    OptList* tmp = opt_listInit();
    
    // Test wrong parameters
    ASSERT(opt_listPushOpt(NULL, NULL, NULL, false, 0, NULL) == false);

    ASSERT(opt_listPushOpt(tmp, "-a", NULL, false, 0, NULL) == false);
    ASSERT(opt_listPushOpt(tmp, "-a", "append an option1", false, 0, NULL) == true);


    ASSERT(opt_listPushOpt(tmp, "-a", "append an option2", true, 1, NULL) == true);
    ASSERT(opt_listHasOpt(tmp, "-a") == true);
    ASSERT(opt_listPushOpt(tmp, "-a", "append an option3", true, 1, NULL) == true);
    // The number of options added is still 1
    ASSERT(tmp->occupied == 1);
    // The option stored is the first valid call, with "option1"
    ASSERT(strcmp(tmp->values[0]->description, "append an option1") == 0);
    ASSERT(opt_listPushOpt(tmp, "-b", "append an optionB", true, 1, NULL) == true);
    ASSERT(tmp->occupied == 2);

    opt_listQuit(tmp);


    ASSERT(tmp == NULL);
}

int msg(int argc, char** argv){
    for(int i=0; i< argc; i++){
        printf("MSG [#%d] : %s\n", i, argv[i]);
    }
    return 0;
}

void OptListParse(int argc, char** argv){
    OptList* tmp = opt_listInit();

    // Need 1 mandatory arg without param
    opt_listPushOpt(tmp, "-mpa", "mandatory parent a", true, 0, NULL);
    Opt* mpaOpt = opt_listGetOpt(tmp, "-mpa");

    int c1 = 1;
    char* v1[]={
        "programName",
    };
    opt_listParse(tmp, c1, v1);
    ASSERT(mpaOpt->parsed == false);
    ASSERT(mpaOpt->valided == false);

    int c2 = 2;
    char* v2[]={
        "programName",
        "-mpa"
    };
    opt_listParse(tmp, c2, v2);
    ASSERT(mpaOpt->parsed == true);
    ASSERT(mpaOpt->valided == true);


    // Need 1 mandatory arg with param
    opt_listPushOpt(tmp, "-mpb", "mandatory parent b", true, 1, NULL);
    Opt* mpbOpt = opt_listGetOpt(tmp, "-mpb");

    opt_listParse(tmp, c2, v2);
    ASSERT(mpbOpt->parsed == false);
    ASSERT(mpbOpt->valided == false);

    int c3 = 3;
    char* v3[] = {
        "programName",
        "-mpa",
        "-mpb"
    };
    opt_listParse(tmp, c3, v3);
    ASSERT(mpbOpt->parsed == true);
    ASSERT(mpbOpt->valided == false);

    int c4 = 4;
    char* v4[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa"
    };
    opt_listParse(tmp, c4, v4);
    ASSERT(mpbOpt->parsed == true);
    ASSERT(mpbOpt->valided == true);

    // Need 1 mandatory arg with param and callback
    opt_listPushOpt(tmp, "-mpc", "mandatory parent c", true, 0, (OptCaller)msg);
    Opt* mpcOpt = opt_listGetOpt(tmp, "-mpc");

    opt_listParse(tmp, c4, v4);
    ASSERT(mpcOpt->parsed == false);
    ASSERT(mpcOpt->valided == false);

    int c5 = 5;
    char* v5[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-mpc"
    };
    opt_listParse(tmp, c5, v5);
    ASSERT(mpcOpt->parsed == true);
    ASSERT(mpcOpt->valided == true);

    // Need 1 optionnal arg without param
    opt_listPushOpt(tmp, "-opa", "optionnal parent a", false, 0, NULL);
    Opt* opaOpt = opt_listGetOpt(tmp, "-opa");
    opt_listParse(tmp, c5, v5);
    ASSERT(opaOpt->parsed == false);
    ASSERT(opaOpt->valided == false);

    int c6 = 6;
    char* v6[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-opa",
        "-mpc"
    };
    opt_listParse(tmp, c6, v6);
    ASSERT(opaOpt->parsed == true);
    ASSERT(opaOpt->valided == true);

    // Need 1 optionnal arg with param
    opt_listPushOpt(tmp, "-opb", "optionnal parent b", false, 1, NULL);
    Opt* opbOpt = opt_listGetOpt(tmp, "-opb");
    opt_listParse(tmp, c6, v6);
    ASSERT(opbOpt->parsed == false);
    ASSERT(opbOpt->valided == false);

    int c7 = 8;
    char* v7[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-opa",
        "-mpc",
        "-opb",
        "bbb"
    };
    opt_listParse(tmp, c7, v7);
    ASSERT(opbOpt->parsed == true);
    ASSERT(opbOpt->valided == true);
    ASSERT(opbOpt->callback == NULL);

    opt_listPushChild(tmp, "-mpa", "-msa", "mandatory sub a", true, 1, NULL);
    Opt* msaOpt = opt_listGetOpt(tmp, "-msa");
    ASSERT(msaOpt != NULL);

    opt_listParse(tmp, c7, v7);
    ASSERT(msaOpt->parsed == false);
    ASSERT(msaOpt->valided == false);

    int c8 = 10;
    char* v8[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-opa",
        "-mpc",
        "-opb",
        "bbb",
        "-msa",
        "ccc"
    };
    opt_listParse(tmp, c8, v8);
    ASSERT(msaOpt->parsed == true);
    ASSERT(msaOpt->valided == true);

    // Need 1 optionnal arg with a mandatory sub
    opt_listPushChild(tmp, "-opa", "-msb", "mandatory sub b", true, 1, NULL);
    Opt* msbOpt = opt_listGetOpt(tmp, "-msb");
    opt_listParse(tmp, c8, v8);

    ASSERT(msbOpt != NULL);
    ASSERT(msbOpt->parsed == false);

    int c9 = 9;
    char* v9[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-mpc",
        "-opb",
        "bbb",
        "-msa",
        "ccc"
    };
    opt_listParse(tmp, c9, v9);
    ASSERT(opaOpt->parsed == false);
    ASSERT(opaOpt->valided == false);

    ASSERT(msbOpt->parsed == false);
    ASSERT(msbOpt->valided == false);


    int c10 = 10;
    char* v10[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-opa",
        "-mpc",
        "-opb",
        "bbb",
        "-msa",
        "ccc"
    };
    opt_listParse(tmp, c10, v10);
    ASSERT(opaOpt->parsed == true);
    ASSERT(opaOpt->valided == true);

    ASSERT(msbOpt->parsed == false);
    ASSERT(msbOpt->valided == false);

    int c11 = 11;
    char* v11[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-opa",
        "-mpc",
        "-opb",
        "bbb",
        "-msa",
        "ccc",
        "-msb"
    };
    opt_listParse(tmp, c11, v11);
    ASSERT(opaOpt->parsed == true);
    ASSERT(opaOpt->valided == true);

    ASSERT(msbOpt->parsed == true);
    ASSERT(msbOpt->valided == false);

        int c12 = 12;
    char* v12[] = {
        "programName",
        "-mpa",
        "-mpb",
        "aaa",
        "-opa",
        "-mpc",
        "-opb",
        "bbb",
        "-msa",
        "ccc",
        "-msb",
        "ddd"
    };

    opt_listParse(tmp, c12, v12);
    ASSERT(opaOpt->parsed == true);
    ASSERT(opaOpt->valided == true);

    ASSERT(msbOpt->parsed == true);
    ASSERT(msbOpt->valided == true);
    opt_listQuit(tmp);
}