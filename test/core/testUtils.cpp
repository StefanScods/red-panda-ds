#include <gtest/gtest.h>

#include <sstream>

#include "core/armInterpreter/instructionDisassembler.h"
#include "core/utils/armEncode.h"
#include "core/utils/utils.h"

using namespace RedPandaDS::Core;

/**
 * @brief Test fixture for the application's utility functions.
 */
class TestUtils : public testing::Test {
protected:
    TestUtils() {}
    ~TestUtils() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestUtils, ReadBit) {
    // Read different portions of the data.
    uint32_t data = 0b10110000111100001111000011110000;
    ASSERT_EQ(readBit(data, 0), 0b0);
    ASSERT_EQ(readBit(data, 1), 0b0);
    ASSERT_EQ(readBit(data, 6), 0b1);
    ASSERT_EQ(readBit(data, 7), 0b1);
    ASSERT_EQ(readBit(data, 30), 0b0);
    ASSERT_EQ(readBit(data, 31), 0b1);
}

TEST_F(TestUtils, ReadBits) {
    // Read different portions of the data.
    uint32_t data = 0b11110000111100001111000011110000;
    ASSERT_EQ(readBits(data, 0, 31), data);
    ASSERT_EQ(readBits(data, 0, 15), 0b1111000011110000);
    ASSERT_EQ(readBits(data, 16, 31), 0b1111000011110000);
    ASSERT_EQ(readBits(data, 0, 7), 0b11110000);
    ASSERT_EQ(readBits(data, 4, 11), 0b00001111);
    ASSERT_EQ(readBits(data, 6, 12), 0b1000011);
    ASSERT_EQ(readBits(data, 31, 31), 0b1);
    ASSERT_EQ(readBits(data, 0, 0), 0b0);
}

TEST_F(TestUtils, WriteBit) {
    // Write different portions of the data.
    uint32_t data = 0b10110000111100001111000011110000;
    writeBit(data, 1, 0);
    ASSERT_EQ(data, 0b10110000111100001111000011110001);

    data = 0b10110000111100001111000011110000;
    writeBit(data, 0, 31);
    ASSERT_EQ(data, 0b00110000111100001111000011110000);

    data = 0b10110000111100001111000011110000;
    writeBit(data, 0, 30);
    ASSERT_EQ(data, 0b10110000111100001111000011110000);
}

TEST_F(TestUtils, WriteBits) {
    // Write different portions of the data.
    uint32_t data = 0b11110000111100001111000011110000;
    writeBits<uint32_t>(data, 0b00001111000011110000111100001011, 0, 31);
    ASSERT_EQ(data, 0b00001111000011110000111100001011);

    data = 0b11110000111100001111000011110000;
    writeBits<uint32_t>(data, 0b1111, 0, 3);
    ASSERT_EQ(data, 0b11110000111100001111000011111111);

    data = 0b11110000111100001111000011110000;
    writeBits<uint32_t>(data, 0b0000, 28, 31);
    ASSERT_EQ(data, 0b00000000111100001111000011110000);
}

TEST_F(TestUtils, ROR) {
    uint32_t data = 0b00000000000000000000000000000011;

    u32AndBool returnValue = ROR(data, 0);
    ASSERT_EQ(returnValue.data_u32, 0b0000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROR(data, 1);
    ASSERT_EQ(returnValue.data_u32, 0b10000000000000000000000000000001);
    ASSERT_EQ(returnValue.data_bool, 1);

    returnValue = ROR(data, 2);
    ASSERT_EQ(returnValue.data_u32, 0b11000000000000000000000000000000);
    ASSERT_EQ(returnValue.data_bool, 1);

    returnValue = ROR(data, 3);
    ASSERT_EQ(returnValue.data_u32, 0b01100000000000000000000000000000);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROR(data, 32);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 0);
}

TEST_F(TestUtils, ROL) {
    uint32_t data = 0b00000000000000000000000000000011;

    u32AndBool returnValue = ROL(data, 0);
    ASSERT_EQ(returnValue.data_u32, 0b0000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROL(data, 1);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000000110);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROL(data, 2);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000001100);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROL(data, 31);
    ASSERT_EQ(returnValue.data_u32, 0b10000000000000000000000000000001);
    ASSERT_EQ(returnValue.data_bool, 1);

    returnValue = ROL(data, 32);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 1);
}

TEST_F(TestUtils, ARMDisassemble) {
    std::vector<std::string> testCases = {"ADD R0, R1, R2",
                                          "ADDS R3, R4, R5",
                                          "SUB R6, R7, #10",
                                          "SBC R8, R9, R10",
                                          "RSB R1, R2, R3",
                                          "AND R4, R5, R6",
                                          "ORR R7, R8, R9",
                                          "EOR R10, R11, R12",
                                          "BIC R0, R1, R2",
                                          "MOV R3, R4",
                                          "MVN R5, R6",
                                          "CMP R7, R8",
                                          "CMN R9, R10",
                                          "TST R11, R12",
                                          "TEQ R1, R2",
                                          "ADD R0, R1, R2, LSL #2",
                                          "SUB R3, R4, R5, LSR #3",
                                          "AND R6, R7, R8, ASR #1",
                                          "ORR R9, R10, R11, ROR #4",
                                          "EOR R12, R0, R1, LSL R2",
                                          "ADDne R0, R1, R2",
                                          "SUBeq R3, R4, R5",
                                          "MOVgt R6, R7",
                                          "CMPle R8, R9",
                                          "MUL R0, R1, R2",
                                          "MLA R3, R4, R5, R6",
                                          "UMULL R7, R8, R9, R10",
                                          "SMULL R11, R12, R0, R1",
                                          "UMLAL R2, R3, R4, R5",
                                          "SMLAL R6, R7, R8, R9",
                                          "LDR R0, [R1]",
                                          "STR R2, [R3]",
                                          "LDR R4, [R5, #4]",
                                          "STR R6, [R7, #-8]",
                                          "LDR R0, [R1, #4]!",
                                          "STR R2, [R3], #8",
                                          "LDR R4, [R5], #-12",
                                          "LDR R6, [R7, R8]",
                                          "STR R9, [R10, R11, LSL #2]",
                                          "LDRB R0, [R1]",
                                          "STRB R2, [R3]",
                                          "LDRH R4, [R5]",
                                          "STRH R6, [R7]",
                                          "LDRSB R8, [R9]",
                                          "LDRSH R10, [R11]",
                                          "LDMIA R0!, {R1-R3}",
                                          "STMDB R4!, {R5-R7}",
                                          "LDMIA SP!, {R0-R3}",
                                          "STMDB SP!, {R4-R7,LR}",
                                          "B .+4",
                                          "BL .+8",
                                          "Bne .-4",
                                          "Blt .+16",
                                          "SWP R0, R1, [R2]",
                                          "SWPB R3, R4, [R5]",
                                          "MRS R0, CPSR",
                                          "MSR CPSR_f, R1",
                                          "MSR SPSR_fc, R2",
                                          "MCR p15, #0, R0, c1, c0, #1",
                                          "MRC p15, #0, R1, c2, c0, #2",
                                          "CDP p10, #0, c1, c2, c3, #3",
                                          "LDC p1, c0, [R0]",
                                          "STC p2, c1, [R1, #4]",
                                          "BX LR",
                                          "BLX R3",
                                          "QADD R0, R1, R2",
                                          "QSUB R3, R4, R5",
                                          "ADD R0, R0, #0",
                                          "SUBS R1, R1, #1",
                                          "RSBS R2, R2, #0",
                                          "MOV R3, #0",
                                          "MOV R4, #255",
                                          "MVN R5, #0",
                                          "ADD R6, R7, #4278190080",
                                          "SUB R8, R9, #2147483648",
                                          "ADD R0, PC, #4",
                                          "MOV R1, PC",
                                          "CMP PC, R2",
                                          "ADD SP, SP, #16",
                                          "SUB SP, SP, #32",
                                          "MOV R0, R1",
                                          "MOV R2, R3, LSR #31",
                                          "MOV R4, R5, ASR #31",
                                          "ADD R0, R1, R2, LSL R3",
                                          "SUB R4, R5, R6, ASR R7",
                                          "ADDne R0, R1, R2",
                                          "MOVeq R3, R4",
                                          "CMNgt R5, R6",
                                          "MULS R0, R1, R2",
                                          "MLAS R3, R4, R5, R6",
                                          "UMULLS R7, R8, R9, R10",
                                          "SMLALS R11, R12, R0, R1",
                                          "MUL R0, R0, R0",
                                          "ADD PC, R1, R1",
                                          "LDR R0, [R1]",
                                          "STR R2, [R3]",
                                          "MOV PC, SP, RRX",
                                          "LDR R4, [PC, #4]",
                                          "LDR R5, [PC, #-8]",
                                          "LDR R6, [R6, #4]!",
                                          "STR R7, [R7], #4",
                                          "LDR R0, [SP, #4]",
                                          "STR R1, [SP, #-4]!",
                                          "LDRH R2, [R3, #2]",
                                          "STRH R4, [R5, #-2]",
                                          "LDRSH R6, [R7, #2]",
                                          "LDRSB R8, [R9, #-1]",
                                          "LDRH R10, [R11, R12]",
                                          "STRH R0, [R1, R2]",
                                          "LDMIA R0!, {R0}",
                                          "STMDB R1!, {R1}",
                                          "LDMIA R2!, {R3-R10}",
                                          "STMDB R4!, {R5-R12,LR}",
                                          "LDMIA R0!, {R1,R2,PC}",
                                          "STMDB R3!, {R4,R5,LR}",
                                          "B .",
                                          "Bne .",
                                          "BL .",
                                          "B .-8",
                                          "BX R0",
                                          "BX LR",
                                          "MRS R1, SPSR",
                                          "MSR CPSR_c, R2",
                                          "MSR CPSR_fsxc, R3",
                                          "MCR p15, #7, R0, c1, c0",
                                          "MRC p15, #3, R1, c2, c0, #1",
                                          "CDP p2, #5, c1, c2, c3, #7",
                                          "LDC p1, c0, [R0, #4]",
                                          "STC p2, c1, [R1], #8",
                                          "SWPB R12, R2, [R1]",
                                          "QADD R0, R0, R0",
                                          "QSUB R1, R1, R1"

    };
    std::stringstream stream;
    for (const auto& testCase : testCases) {
        stream << testCase << '\n';
    }
    std::vector<Encoding> encodings = armEncodeASM(stream.str(), false);
    EXPECT_EQ(encodings.size(), testCases.size());
    for (uint32_t i = 0; i < encodings.size(); i++) {
        InstructionDisassembly disassembly = dissembleARMInstruction(encodings.at(i).instruction);

        std::cout << testCases.at(i) << ": " << "0x" << std::hex
                  << (uint32_t)(encodings.at(i).instruction) << std::dec << std::endl;
        EXPECT_EQ(testCases.at(i), disassembly.toString());
    }
}
