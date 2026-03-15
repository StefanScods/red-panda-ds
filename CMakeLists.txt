cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(PROJECT RedPandaDSEmu)
project(${PROJECT})

# ==================================================================================================
# Release / Debug Directory
# ==================================================================================================
# Output to either debug or release
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# ==================================================================================================
# Logger Overrides
# ==================================================================================================

# Uncomment this line to remove all print statements within the program.
# add_definitions(-DNO_LOG_OVERRIDE)

# Uncomment this line to enable all debug print statements within the program.
# add_definitions(-DLOG_LEVEL_OVERRIDE=3)

# ==================================================================================================
# Global CMake Settings
# ==================================================================================================
set(GLOBAL_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

# ==================================================================================================
# Common Target
# ==================================================================================================
add_subdirectory(common)

# ==================================================================================================
# Core Target
# ==================================================================================================
add_subdirectory(core)

# ==================================================================================================
# Test
# ==================================================================================================
set(BUILD_TEST True)
if(${BUILD_TEST})
    add_subdirectory(test)
endif()

# ==================================================================================================
# Applications
# ==================================================================================================
add_subdirectory(frontends/redPandaDS)
