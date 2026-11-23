.global test_saveResult, test_endTest

.equ TEST_RESULT_ADDR, 0x023FFFFC
// =================================================================================================
// Saves a result to the test's result location in Main RAM.
// Arguments:
// r0 = value to save
test_saveResult:
    push {r1} 
    ldr r1, =TEST_RESULT_ADDR
    str r0, [r1]
    pop {r1} 
    bx lr
// =================================================================================================
// Ends the test and posts the result in Main RAM/
// Arguments:
// r0 = test's return value.
test_endTest:
    bl test_saveResult
test_endTest_done:
    b test_endTest_done
// =================================================================================================
