# Assert.h

Concerned file : include/assert.h

This file is describing how to use the include/assert.h lib.

# Functions list
- `int assertGetSuccess(void)`: getter on the success assert counter
- `int assertGetFailure(void)`: getter on the failure assert counter
- `int assertGetTotal(void)`: getter on the total assert counter
- `void assertResetAssert(void)`: reset all these counters



## Macros list
- `ASSERT(expr)`: execute `expr` and raise assertion counters statuses
- `TEST(expr)`: execute `expr` and raise test counters statuses

## Hierarchy on tests and asserts

A test execute a set of asserts. If all the asserts have successed, the test has successed. Else the test failed.

I.E.:
- Test 1:
    - Assert 1 success
    - Assert 2 success
    - Assert 3 failure

In this example, the test 1 failed because assert 3 failed.

If you need to test the failure on an assertion, it is possible to reset assert counters before the assertions, run an failed assertion, assert the failure counter. At the end of assert, reset the counters again, else the test will always be on failure state.