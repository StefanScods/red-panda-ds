cmake_minimum_required(VERSION 3.10)

set(PROJECT RedPandaDSEmu)
set(BINARY_NAME redPandaDS)
project(${PROJECT})

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
set(SOURCE)
set(INCLUDE_DIRS
)
add_subdirectory(src)

# Print out all the source files to be compiled.
string (REPLACE ";" "\n" SRC_FORMAT "${SOURCE}")
message("Found source files:\n${SRC_FORMAT}")

# Uncomment this line to remove all print statements within the program.
# add_definitions(-DNO_LOG_OVERRIDE)

# add_executable(${BINARY_NAME} WIN32 ${SOURCE})
add_executable(${BINARY_NAME} ${SOURCE}) # Without win32, shows terminal

target_include_directories(${BINARY_NAME} PRIVATE ${INCLUDE_DIRS})