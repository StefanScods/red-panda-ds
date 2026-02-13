/**
 * @file armEncode.cpp
 * @brief Covert written instructions into their encoded binary equivalent.
 */

#include "armEncode.h"

#include "interconnect.h"

#define LOG_LEVEL 1
#include "logger.h"

// ==================================================================================================
void cleanUpEncodeTemps(std::filesystem::path tempDir) {
    if (std::filesystem::exists(tempDir)) {
        std::filesystem::remove_all(tempDir);
    }
}
// ==================================================================================================
std::vector<uint32_t> armEncodeASM(std::string instructions, bool arm7) {
    LogDebugPrefixed("Assembling:\n===============================\n"
                         << instructions,
                     "ASM Encode");
    std::vector<uint32_t> encodings;

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

    bool foundText = false;
    std::string line;
    int count = 0;
    std::string substring = "<.text>:";
    // Re-read the dump and extract the asm encodings.
    std::ifstream dumpFile(outputObjectDumpFile);
    while (std::getline(dumpFile, line)) {
        if (!foundText && line.find(substring) != std::string::npos) {
            foundText = true;
        } else if (foundText) {
            // Find "num: encoding    instuction"
            substring = ":	";
            size_t pos = line.find(substring);
            // Ran out of instructions.
            if (pos == std::string::npos) break;
            // Remove everything other than the encoding.
            std::string after = line.substr(pos + substring.length());
            pos = after.find(' ');
            if (pos == std::string::npos) {
                LogErrorPrefixed("Could not extract encoding for line " << count << "!",
                                 "ASM Encode");
                dumpFile.close();
                cleanUpEncodeTemps();
                return encodings;
            }
            after = after.substr(0, pos);
            // Append the encoding.
            encodings.push_back(std::stoul(after, nullptr, 16));
            count++;
        }
    }

    dumpFile.close();
    cleanUpEncodeTemps();
    LogDebugPrefixed("Finished assembling program!", "ASM Encode");
    return encodings;
}
// ==================================================================================================
void writeProgramToMemory(std::string program, uint32_t startAddress, Interconnect* bus,
                          bool arm7) {
    std::vector<uint32_t> instuctionEncodings = armEncodeASM(program, arm7);
    LogDebug("Writing program to 0x" << std::hex << startAddress << std::dec << "...");
    for (int i = 0; i < instuctionEncodings.size(); i++) {
        uint32_t address = startAddress + i * ARM_WORD_SIZE;
        LogDebug("Writing instuction to 0x" << std::hex << instuctionEncodings[i] << " to 0x" << address << std::dec << "...");
        arm7 ? bus->write32ARM7(address, instuctionEncodings[i])
             : bus->write32ARM7(address, instuctionEncodings[i]);
    }
    LogDebug("Finished writing program! - " << instuctionEncodings.size() << " instructions - "
                                            << instuctionEncodings.size() * ARM_WORD_SIZE
                                            << " byte(s)");
}
// ==================================================================================================
