cmake_minimum_required(VERSION 3.10)

function(add_nds_rom ROM_NAME)
    set(ROM_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    set(BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/${ROM_NAME}")
    set(ROM_FILE "${CMAKE_BINARY_DIR}/roms/${ROM_NAME}.nds")

    file(MAKE_DIRECTORY "${BUILD_DIR}")

    # ARM9
    set(ARM9_BIN "${BUILD_DIR}/arm9.bin")
    add_custom_command(
        OUTPUT "${ARM9_BIN}"

        COMMAND ${ARM_AS} -mcpu=arm946e-s -march=armv5te
        -o arm9.o "${ROM_DIR}/arm9.s"

        COMMAND ${ARM_LD}
        -T "${TEST_ROM_ROOT}/common/linker_arm9.ld"
        -o arm9.elf arm9.o

        COMMAND ${ARM_OBJCOPY} -O binary arm9.elf "${ARM9_BIN}"

        WORKING_DIRECTORY "${BUILD_DIR}"

        DEPENDS
        "${ROM_DIR}/arm9.s"
        "${TEST_ROM_ROOT}/common/linker_arm9.ld"

        VERBATIM
    )

    # ARM7
    set(ARM7_BIN "${BUILD_DIR}/arm7.bin")
    add_custom_command(
        OUTPUT "${ARM7_BIN}"

        COMMAND ${ARM_AS} -mcpu=arm7tdmi -march=armv4t
        -o arm7.o "${ROM_DIR}/arm7.s"

        COMMAND ${ARM_LD}
        -T "${TEST_ROM_ROOT}/common/linker_arm7.ld"
        -o arm7.elf arm7.o

        COMMAND ${ARM_OBJCOPY} -O binary arm7.elf "${ARM7_BIN}"

        WORKING_DIRECTORY "${BUILD_DIR}"

        DEPENDS
        "${ROM_DIR}/arm7.s"
        "${TEST_ROM_ROOT}/common/linker_arm7.ld"

        VERBATIM
    )

    # Package NDS
    add_custom_command(
        OUTPUT "${ROM_FILE}"

        COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_ROM_OUTPUT}"

        COMMAND ${NDSTOOL} -c "${ROM_FILE}"
        -9 "${ARM9_BIN}"
        -7 "${ARM7_BIN}"
        -r9 0x02000000 -e9 0x02000000
        -r7 0x03800000 -e7 0x03800000
        -g RPDS 00 "${ROM_NAME}"

        DEPENDS
        "${ARM9_BIN}"
        "${ARM7_BIN}"

        VERBATIM
    )

    add_custom_target(${ROM_NAME} ALL
        DEPENDS "${ROM_FILE}"
    )
endfunction()