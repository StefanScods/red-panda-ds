/**
 * @file armEncode.cpp
 * @brief Covert written instructions into their encoded binary equivalent.
 */

#include "armEncode.h"

#include <regex>

#include "interconnect.h"

#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
void cleanUpEncodeTemps(std::filesystem::path tempDir) {
    if (std::filesystem::exists(tempDir)) {
        std::filesystem::remove_all(tempDir);
    }
}
// ==================================================================================================
std::vector<Encoding> armEncodeASM(std::string instructions, bool arm7) {
    LogDebugPrefixed("Assembling:\n===============================\n"
                         << instructions,
                     "ASM Encode");
    std::vector<Encoding> encodings;

    // Create directory to hold all the temporaries.
    std::filesystem::path tempDir = "asm_temp";
    if (!std::filesystem::exists(tempDir)) {
        std::filesystem::create_directory(tempDir);
    }

    // Write to a temp file.
    std::filesystem::path inputAsmFile = tempDir / "temp.s";
    std::ofstream asmFile(inputAsmFile);
    asmFile << instructions << std::endl;
    asmFile.close();

    std::string arch = arm7 ? "armv4t" : "ARMv5TE";
    // Assemble the instructions.
    std::filesystem::path outputObjectFile = tempDir / "temp.o";
    std::string armASMCommand = "arm-none-eabi-gcc -c " + inputAsmFile.string() + " -o " +
                                outputObjectFile.string() + " -march=" + arch;
    LogDebugPrefixed("Calling: " << armASMCommand, "ASM Encode");
    if (system(armASMCommand.c_str()) != 0) {
        LogErrorPrefixed("Failed to compile assembly file!", "ASM Encode");
        cleanUpEncodeTemps();
        return encodings;
    }

    // Dump the object file.
    std::filesystem::path outputObjectDumpFile = tempDir / "temp.txt";
    armASMCommand = "arm-none-eabi-objdump -d " + outputObjectFile.string() + " >> " +
                    outputObjectDumpFile.string();
    LogDebugPrefixed("Calling: " << armASMCommand, "ASM Encode");
    if (system(armASMCommand.c_str()) != 0) {
        LogErrorPrefixed("Failed to dump object file!", "ASM Encode");
        cleanUpEncodeTemps();
        return encodings;
    }

    LogDebugPrefixed("Created Assembly Dump:\n==========================================",
                     "ASM Encode");
    bool armMode = true;
    uint32_t prevAddress = 0;
    std::string line;
    // Re-read the dump and extract the asm encodings.
    std::ifstream dumpFile(outputObjectDumpFile);
    while (std::getline(dumpFile, line)) {
        LogDebugPrefixed(line, "ASM Encode");
        std::string nopString = "...";
        if (line.find(nopString) != std::string::npos) {
            // Detected a switch to thumb encoding.
            if (armMode) {
                // Set the last instruction to thumb mode as well.
                encodings.back().arm = false;
                armMode = false;
            }
            continue;
        }
        // Regex to match the "address: encoding    instuction" lines.
        std::regex pattern(R"(\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]+))");
        std::smatch match;
        if (!std::regex_search(line, match, pattern)) continue;
        uint32_t instruction = std::stoul(match[2], nullptr, 16);
        uint32_t address = std::stoul(match[1], nullptr, 16);
        // Detect switches in encodings.
        if (armMode && (address - prevAddress) == THUMB_MODE_INST_SIZE) {
            // Set the last instruction to thumb mode as well.
            encodings.back().arm = false;
            armMode = false;
        }
        if (!armMode && (address - prevAddress) == ARM_MODE_INST_SIZE) {
            // Set the last instruction to thumb mode as well.
            encodings.back().arm = true;
            armMode = true;
        }
        // Add the new instruction.
        encodings.emplace_back(instruction, address, armMode);
        prevAddress = address;
    }

    dumpFile.close();
    cleanUpEncodeTemps();
    LogDebugPrefixed("Finished assembling program!", "ASM Encode");
    return encodings;
}
// ==================================================================================================
void writeProgramToMemory(std::string program, uint32_t startAddress, Interconnect* bus,
                          bool arm7) {
    std::vector<Encoding> instuctionEncodings = armEncodeASM(program, arm7);
    LogDebug("Writing program to 0x" << std::hex << startAddress << std::dec << "...");
    uint32_t bytesWritten = 0;
    for (const Encoding& encoding : instuctionEncodings) {
        uint32_t address = startAddress + encoding.address;
        LogDebug("Writing instuction to " << PrintHex(encoding.instruction)
                                          << (encoding.arm ? " (ARM)" : " (THUMB)") << " to "
                                          << PrintHex(address) << "...");
        if (encoding.arm) {
            arm7 ? bus->write32ARM7(address, encoding.instruction)
                 : bus->write32ARM9(address, encoding.instruction);
            bytesWritten += ARM_MODE_INST_SIZE;
        } else {
            arm7 ? bus->write16ARM7(address, encoding.instruction)
                 : bus->write16ARM9(address, encoding.instruction);
            bytesWritten += THUMB_MODE_INST_SIZE;
        }
    }
    LogDebug("Finished writing program! - " << instuctionEncodings.size() << " instructions - "
                                            << bytesWritten << " byte(s)");
}
// ==================================================================================================
