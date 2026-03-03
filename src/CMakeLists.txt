cmake_minimum_required(VERSION 3.10)

set(EMU_CORE_INCLUDES
   ${EMU_CORE_INCLUDES}
   ${CMAKE_CURRENT_SOURCE_DIR}/
   ${CMAKE_CURRENT_SOURCE_DIR}/utils
   PARENT_SCOPE
)

set(EMU_CORE_SOURCES
   ${EMU_CORE_SOURCES}
   ${CMAKE_CURRENT_SOURCE_DIR}/utils/armEncode.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/arm7.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/arm9.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/interconnect.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp

   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/arm_decodeAndExecute.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/arm_branch.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/arm_data.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/arm_loadStore.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/arm_misc.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/thumb_decodeAndExecute.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/thumb_branch.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/thumb_data.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/thumb_loadStore.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/thumb_misc.cpp
   PARENT_SCOPE
)

