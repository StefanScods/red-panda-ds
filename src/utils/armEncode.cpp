/**
 * @file armEncode.cpp
 * @brief Covert written instructions into their encoded binary equivelent.
 */

#include "armEncode.h"

#define LOG_LEVEL 1
#include "logger.h"

void cleanUpEncodeTemps(std::filesystem::path tempDir) {
    if (std::filesystem::exists(tempDir)) {
        std::filesystem::remove_all(tempDir);
    }
}

std::vector<uint32_t> armEncodeASM(std::string instructions) {
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

    // Assemble the instructions.
    std::filesystem::path outputObjectFile = tempDir / "temp.o";
    std::string armASMCommand =
        "arm-none-eabi-gcc -c " + inputAsmFile.string() + " -o " + outputObjectFile.string();
    if (system(armASMCommand.c_str()) != 0) {
        LogErrorPrefixed("Failed to compile assembly file!", "ASM Encode");
        cleanUpEncodeTemps();
        return encodings;
    }

    // Dump the object file.
    std::filesystem::path outputObjectDumpFile = tempDir / "temp.txt";
    armASMCommand = "arm-none-eabi-objdump -d " + outputObjectFile.string() + " >> " +
                    outputObjectDumpFile.string();
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
