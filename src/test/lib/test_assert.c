#include <stdio.h>
#include <stdlib.h>

#include "assert.h"


void test_assert_success(){
    ASSERT(1 == 1);
}

void test_assert_failure(){
    assertResetAssert();

    ASSERT(0 == 1);
    
    ASSERT(assertGetFailure() == 1);
    assertResetAssert();

}

int main(int argc, char** argv){
    TEST(test_assert_success());
    TEST(test_assert_failure());

    return 0;
}

