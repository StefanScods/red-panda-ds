cmake_minimum_required(VERSION 3.10)

message("Building Test Suite")

# ==================================================================================================
# Google Test
# ==================================================================================================
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

# Init the includes with the core application files and the test framework
set(TEST_INCLUDES
    ${EMU_CORE_INCLUDES}
)
include(GoogleTest)

# ==================================================================================================
# Test Suites
# ==================================================================================================
add_subdirectory(core)
