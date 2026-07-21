#ifndef ASSERT_H
#define ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <stdbool.h>


static int _assert_success=0;
static int _assert_failure = 0;
static int _assert_total=0;

static int _test_success=0;
static int _test_failure=0;
static int _test_total=0;

#define ASSERT(expr) do{\
    if((expr) == true){\
        _assert_success++;\
        _assert_total++;\
    }\
    else{\
        printf("\t(Assertion %s failed on file %s, function %s, line %d)\n", #expr, __FILE__, __func__, __LINE__);\
        _assert_failure++;\
        _assert_total++;\
    }\
}while(0);

#define TEST(expr) do{\
    printf("Testing %s ...\n", #expr);\
    _assert_success = 0;\
    _assert_failure = 0;\
    _assert_total = 0;\
    expr;\
    if(_assert_failure == 0){\
        printf("success\n");\
        _test_success++;\
        _test_total++;\
    }\
    else{\
        printf("failure\n");\
        _test_failure++;\
        _test_total++;\
    }\
    printf("\tTest %d / %d successed;\n\t\tAssertions: %d / %d\n", _test_success, _test_total, _assert_success, _assert_total);\
    printf("\tTest %d / %d failed;\n\t\tAssertions: %d / %d\n", _test_failure, _test_total, _assert_failure, _assert_total);\
}while(0);

int assertGetSuccess(void){return _assert_success;}
int assertGetFailure(void){return _assert_failure;}
int assertGetTotal(void){return _assert_total;}

void assertResetAssert(void){_assert_success=0; _assert_failure=0; _assert_total=0;}


#ifdef __cplusplus
}
#endif

#endif