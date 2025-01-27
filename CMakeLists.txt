cmake_minimum_required(VERSION 3.10)

set(PROJECT RedPandaDSEmu)
project(${PROJECT})

set(BINARY_NAME redPandaDS)
set(TEST_BINARY_NAME runTests)

# Uncomment this line to remove all print statements within the program.
# add_definitions(-DNO_LOG_OVERRIDE)

# Uncomment this line to enable all debug print statements within the program.
# add_definitions(-DLOG_LEVEL_OVERRIDE=3)

# Build the testing suite.
set(BUILD_TEST True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output to either debug or release
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# Init the source and include vars and begin finding source and header files
set(EMU_CORE_SOURCES)
set(EMU_CORE_INCLUDES)
add_subdirectory(src)

# Get the complete list of source files used to create the main application.
set(SOURCES
    ${EMU_CORE_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
# Print out all the source files to be compiled.
string (REPLACE ";" "\n" SRC_FORMAT "${SOURCES}")
message("Found source files:\n${SRC_FORMAT}")
set(INCLUDES
    ${EMU_CORE_INCLUDES}
)
# Build the main executable.
# add_executable(${BINARY_NAME} WIN32 ${SOURCES})
add_executable(${BINARY_NAME} ${SOURCES}) # Without win32, shows terminal
target_include_directories(${BINARY_NAME} PRIVATE ${INCLUDES})

# Build test suite for application.
if(${BUILD_TEST})
    message("Building Test Suite")
    # Set up test suite.
    include(FetchContent)
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
    # Fetch all the test source and include files.
    set(TEST_SOURCES)
    add_subdirectory(test)
    set(SOURCES
        ${TEST_SOURCES}
        ${EMU_CORE_SOURCES}
    )

    # Force Debug statements to be printed while running tests.
    # add_definitions(-DLOG_LEVEL_OVERRIDE=3)

    # Print out all the test source files to be compiled.
    string (REPLACE ";" "\n" SRC_FORMAT "${SOURCES}")
    message("Found test source files:\n${SRC_FORMAT}")

    add_executable(${TEST_BINARY_NAME} ${SOURCES})
    target_link_libraries(${TEST_BINARY_NAME} GTest::gtest_main)

    target_include_directories(${TEST_BINARY_NAME} PRIVATE ${TEST_INCLUDES})
    gtest_discover_tests(${TEST_BINARY_NAME})
endif()


