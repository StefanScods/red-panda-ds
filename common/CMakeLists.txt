cmake_minimum_required(VERSION 3.10)

# ==================================================================================================
# Source / Include List
# ==================================================================================================
set(COMMON_INCLUDES
   ${GLOBAL_INCLUDES}
   ${CMAKE_CURRENT_SOURCE_DIR}/
)
set(COMMON_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/common.cpp
)

# ==================================================================================================
# Library Compile Target
# ==================================================================================================
set(COMMON_LIB redPandaDSCommonLib)
add_library(${COMMON_LIB} STATIC ${COMMON_SOURCES})
target_include_directories(${COMMON_LIB} PUBLIC ${COMMON_INCLUDES})

# ==================================================================================================
# Export Settings to parent scope.
# ==================================================================================================
set(COMMON_INCLUDES ${COMMON_INCLUDES} PARENT_SCOPE)
set(COMMON_SOURCES ${COMMON_SOURCES} PARENT_SCOPE)
set(COMMON_LIB ${COMMON_LIB} PARENT_SCOPE)
