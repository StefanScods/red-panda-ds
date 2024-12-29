cmake_minimum_required(VERSION 3.10)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(TEST_SOURCES
   ${TEST_SOURCES}
   ${CMAKE_CURRENT_SOURCE_DIR}/testInterconnect.cpp
   PARENT_SCOPE
)
