cmake_minimum_required(VERSION 3.10)

set(TEST_ROM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(TEST_ROM_OUTPUT "${CMAKE_BINARY_DIR}/ROMs")

# ==================================================================================================
# Tool chain.
# ==================================================================================================
find_program(ARM_AS arm-none-eabi-as REQUIRED)
find_program(ARM_LD arm-none-eabi-ld REQUIRED)
find_program(ARM_OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(NDSTOOL ndstool REQUIRED)

include(common/cmake/ndsRom.cmake)

# ==================================================================================================
# Test ROM List
# ==================================================================================================
add_subdirectory(template)