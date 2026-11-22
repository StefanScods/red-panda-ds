cmake_minimum_required(VERSION 3.10)

# ==================================================================================================
# Source / Include List
# ==================================================================================================
set(TEST_CORE_INCLUDES
    ${GLOBAL_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

set(TEST_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/commonTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testUtils.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/testInterconnect.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/CPU/ARM/branchInstructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPU/ARM/dataInstructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPU/ARM/loadStoreInstructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPU/THUMB/branchInstructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPU/THUMB/dataInstructions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CPU/THUMB/loadStoreInstructions.cpp
)

if(NOT SKIP_TEST_ROMS)
    list(APPEND TEST_CORE_SOURCES

        ${CMAKE_CURRENT_SOURCE_DIR}/e2e/common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/e2e/common.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/e2e/testDMA.cpp
    )
endif()

# ==================================================================================================
# Test Binary
# ==================================================================================================
set(TEST_CORE_BINARY_NAME runCoreTests)
add_executable(${TEST_CORE_BINARY_NAME} ${TEST_CORE_SOURCES})
target_include_directories(${TEST_CORE_BINARY_NAME} PRIVATE ${TEST_CORE_INCLUDES})
target_link_libraries(${TEST_CORE_BINARY_NAME} PUBLIC ${EMU_CORE_LIB} GTest::gtest_main)

if(NOT SKIP_TEST_ROMS)
    target_compile_definitions(${TEST_CORE_BINARY_NAME} PRIVATE
        TEST_ROM_OUTPUT="${TEST_ROM_OUTPUT}"
    )
endif()

gtest_discover_tests(${TEST_CORE_BINARY_NAME})