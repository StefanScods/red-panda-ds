cmake_minimum_required(VERSION 3.10)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(TEST_SOURCES
   ${TEST_SOURCES}
   ${CMAKE_CURRENT_SOURCE_DIR}/commonTest.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/testInterconnect.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/testCPUDataInstructions.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/testCPULoadStoreInstructions.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/testUtils.cpp
   PARENT_SCOPE
)
