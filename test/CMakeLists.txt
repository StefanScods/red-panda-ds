cmake_minimum_required(VERSION 3.10)

set(TEST_INCLUDES
    ${TEST_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/
    PARENT_SCOPE
)

set(TEST_SOURCES
   ${TEST_SOURCES}
   ${CMAKE_CURRENT_SOURCE_DIR}/commonTest.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/testInterconnect.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/CPU/ARM/branchInstructions.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/CPU/ARM/dataInstructions.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/CPU/ARM/loadStoreInstructions.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/CPU/THUMB/dataInstructions.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/testUtils.cpp
   PARENT_SCOPE
)
