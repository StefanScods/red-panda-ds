cmake_minimum_required(VERSION 3.10)

# ==================================================================================================
# Source / Include List
# ==================================================================================================
set(EMU_CORE_INCLUDES
   ${GLOBAL_INCLUDES}
   ${CMAKE_CURRENT_SOURCE_DIR}/
   ${CMAKE_CURRENT_SOURCE_DIR}/utils
)
set(EMU_CORE_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/utils/armEncode.cpp

   ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/events.cpp

   ${CMAKE_CURRENT_SOURCE_DIR}/lcd.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/arm7.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/arm9.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/bios.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/interconnect.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/arm7IOMap.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/arm9IOMap.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/debugger.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cpp

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
   ${CMAKE_CURRENT_SOURCE_DIR}/armInterpreter/instructionDisassembler.cpp
)

# ==================================================================================================
# Library Compile Target
# ==================================================================================================
set(EMU_CORE_LIB redPandaDSLib)
add_library(${EMU_CORE_LIB} STATIC ${EMU_CORE_SOURCES})
target_link_libraries(${EMU_CORE_LIB} PUBLIC ${COMMON_LIB})
target_include_directories(${EMU_CORE_LIB} PUBLIC ${EMU_CORE_INCLUDES})

# ==================================================================================================
# Export Settings to parent scope.
# ==================================================================================================
set(EMU_CORE_INCLUDES ${EMU_CORE_INCLUDES} PARENT_SCOPE)
set(EMU_CORE_SOURCES ${EMU_CORE_SOURCES} PARENT_SCOPE)
set(EMU_CORE_LIB ${EMU_CORE_LIB} PARENT_SCOPE)