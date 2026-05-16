#include "cartridge.h"

#include "utils.h"

// Control print statements.
#define LOG_LEVEL 4
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
std::string CartridgeHeader::getGameTitle() const {
    std::string result;
    for (unsigned int i = 0; i < NDS_CARTRIDGE_GAME_TITLE_WIDTH; i++) {
        if (gameTitle[i] == 0) break;
        result += gameTitle[i];
    }
    return result;
}
// ==================================================================================================
std::string CartridgeHeader::getGameCode() const {
    return std::string(reinterpret_cast<const char*>(&gameCode),
                       reinterpret_cast<const char*>(&gameCode) + sizeof(gameCode));
}
// ==================================================================================================
NDS_Cartridge::NDS_Cartridge() {
    closeROM();
}
// ==================================================================================================
void NDS_Cartridge::closeROM() {
    loadedFilePath.clear();
    if (file.is_open()) {
        file.close();
    }
    header = {};
    engTitleString = "";
    iconPixels.clear();
    iconPixels.resize(NDS_CARTRIDGE_ICON_DIMENSION * NDS_CARTRIDGE_ICON_DIMENSION);
    iconPalettes.clear();
    iconPalettes.resize(NDS_CARTRIDGE_ICON_NUM_PALETTES);
}
// ==================================================================================================
void NDS_Cartridge::reset() {
}
// ==================================================================================================
bool NDS_Cartridge::isOpen() {
    return file.is_open();
}
// ==================================================================================================
bool NDS_Cartridge::loadROMFromFile(const std::string& filepath) {
    if (file.is_open()) {
        closeROM();
    }
    // Load the file path.
    loadedFilePath = filepath;
    file.open(filepath, std::ios::binary);
    if (!file.is_open()) {
        LogError("Failed to open ROM file: \"" << filepath << "\"!");
        return false;
    }
    // Start parsing the header.
    if (!loadHeader()) {
        return false;
    }

    // Load the title and the icon.
    if (!loadIconAndTitle()) {
        return false;
    }

    return true;
}
// ==================================================================================================
void NDS_Cartridge::readFromROM(uint32_t addr, uint32_t size, uint8_t* destBuffer) {
    assert(isOpen());
    assert(destBuffer != nullptr);
    file.seekg(addr, std::ios_base::beg);
    file.read(reinterpret_cast<char*>(destBuffer), size);
}
// ==================================================================================================
bool NDS_Cartridge::loadHeader() {
    // 0x000
    file.seekg(0, std::ios_base::beg);
    file.read(reinterpret_cast<char*>(header.gameTitle), NDS_CARTRIDGE_GAME_TITLE_WIDTH);
    // 0x00C
    file.read(reinterpret_cast<char*>(&header.gameCode), sizeof(header.gameCode));
    // 0x010
    file.read(reinterpret_cast<char*>(&header.makerCode), sizeof(header.makerCode));
    // 0x012
    file.read(reinterpret_cast<char*>(&header.unitCode), sizeof(header.unitCode));
    // 0x013
    file.read(reinterpret_cast<char*>(&header.encryptionSeedSelect),
              sizeof(header.encryptionSeedSelect));
    // 0x014
    file.read(reinterpret_cast<char*>(&header.deviceCapacity), sizeof(header.deviceCapacity));
    // 0x015
    file.read(reinterpret_cast<char*>(header.reserved1), NDS_CARTRIDGE_RESERVED_1_WIDTH);
    // 0x01C
    file.read(reinterpret_cast<char*>(header.reserved2), NDS_CARTRIDGE_RESERVED_2_WIDTH);
    // 0x01D
    file.read(reinterpret_cast<char*>(&header.ndsRegion), sizeof(header.ndsRegion));
    // 0x01E
    file.read(reinterpret_cast<char*>(&header.romVersion), sizeof(header.romVersion));
    // 0x01F
    file.read(reinterpret_cast<char*>(&header.autoStart), sizeof(header.autoStart));
    // 0x020
    file.read(reinterpret_cast<char*>(&header.arm9RomOffset), sizeof(header.arm9RomOffset));
    // 0x024
    file.read(reinterpret_cast<char*>(&header.arm9EntryAddress), sizeof(header.arm9EntryAddress));
    // 0x028
    file.read(reinterpret_cast<char*>(&header.arm9RamAddress), sizeof(header.arm9RamAddress));
    // 0x02C
    file.read(reinterpret_cast<char*>(&header.arm9Size), sizeof(header.arm9Size));
    // 0x030
    file.read(reinterpret_cast<char*>(&header.arm7RomOffset), sizeof(header.arm7RomOffset));
    // 0x034
    file.read(reinterpret_cast<char*>(&header.arm7EntryAddress), sizeof(header.arm7EntryAddress));
    // 0x038
    file.read(reinterpret_cast<char*>(&header.arm7RamAddress), sizeof(header.arm7RamAddress));
    // 0x03C
    file.read(reinterpret_cast<char*>(&header.arm7Size), sizeof(header.arm7Size));
    // 0x040
    file.read(reinterpret_cast<char*>(&header.fntOffset), sizeof(header.fntOffset));
    // 0x044
    file.read(reinterpret_cast<char*>(&header.fntSize), sizeof(header.fntSize));
    // 0x048
    file.read(reinterpret_cast<char*>(&header.fatOffset), sizeof(header.fatOffset));
    // 0x04C
    file.read(reinterpret_cast<char*>(&header.fatSize), sizeof(header.fatSize));
    // 0x050
    file.read(reinterpret_cast<char*>(&header.arm9OverlayOffset), sizeof(header.arm9OverlayOffset));
    // 0x054
    file.read(reinterpret_cast<char*>(&header.arm9OverlaySize), sizeof(header.arm9OverlaySize));
    // 0x058
    file.read(reinterpret_cast<char*>(&header.arm7OverlayOffset), sizeof(header.arm7OverlayOffset));
    // 0x05C
    file.read(reinterpret_cast<char*>(&header.arm7OverlaySize), sizeof(header.arm7OverlaySize));
    // 0x060
    file.read(reinterpret_cast<char*>(&header.normalCommandSettings),
              sizeof(header.normalCommandSettings));
    // 0x064
    file.read(reinterpret_cast<char*>(&header.key1CommandSettings),
              sizeof(header.key1CommandSettings));
    // 0x068
    file.read(reinterpret_cast<char*>(&header.iconTitleOffset), sizeof(header.iconTitleOffset));
    // 0x06C
    file.read(reinterpret_cast<char*>(&header.secureAreaChecksum),
              sizeof(header.secureAreaChecksum));
    // 0x06E
    file.read(reinterpret_cast<char*>(&header.secureAreaDelay), sizeof(header.secureAreaDelay));
    // 0x070
    file.read(reinterpret_cast<char*>(&header.arm9AutoLoadHookRamAddress),
              sizeof(header.arm9AutoLoadHookRamAddress));
    // 0x074
    file.read(reinterpret_cast<char*>(&header.arm7AutoLoadHookRamAddress),
              sizeof(header.arm7AutoLoadHookRamAddress));
    // 0x078
    file.read(reinterpret_cast<char*>(header.secureAreaDisable),
              NDS_CARTRIDGE_SECURE_AREA_DISABLE_WIDTH);
    // 0x080
    file.read(reinterpret_cast<char*>(&header.totalUsedRomSize), sizeof(header.totalUsedRomSize));
    // 0x084
    file.read(reinterpret_cast<char*>(&header.romHeaderSize), sizeof(header.romHeaderSize));
    // 0x088
    file.read(reinterpret_cast<char*>(header.unknown), NDS_CARTRIDGE_UNKNOWN_WIDTH);
    // 0x08C
    file.read(reinterpret_cast<char*>(header.reserved3), NDS_CARTRIDGE_RESERVED_3_WIDTH);
    // 0x094
    file.read(reinterpret_cast<char*>(&header.nandEndOfRomArea), sizeof(header.nandEndOfRomArea));
    // 0x096
    file.read(reinterpret_cast<char*>(&header.nandStartOfRwArea), sizeof(header.nandStartOfRwArea));
    // 0x098
    file.read(reinterpret_cast<char*>(header.reserved4), NDS_CARTRIDGE_RESERVED_4_WIDTH);
    // 0x0B0
    file.read(reinterpret_cast<char*>(header.reserved5), NDS_CARTRIDGE_RESERVED_5_WIDTH);
    // 0x0C0
    file.read(reinterpret_cast<char*>(header.nintendoLogo), NDS_CARTRIDGE_NINTENDO_LOGO_WIDTH);
    // 0x15C
    file.read(reinterpret_cast<char*>(&header.nintendoLogoChecksum),
              sizeof(header.nintendoLogoChecksum));
    // 0x15E
    file.read(reinterpret_cast<char*>(&header.headerChecksum), sizeof(header.headerChecksum));
    // 0x160
    file.read(reinterpret_cast<char*>(&header.debugRomOffset), sizeof(header.debugRomOffset));
    // 0x164
    file.read(reinterpret_cast<char*>(&header.debugSize), sizeof(header.debugSize));
    // 0x168
    file.read(reinterpret_cast<char*>(&header.debugRamAddress), sizeof(header.debugRamAddress));
    // 0x16C-0xFFF are reserved.

    // Check the header checksum.
    uint16_t calculatedHeaderChecksum = getCRC16(reinterpret_cast<uint8_t*>(&header), 0x15E);
    if (header.headerChecksum != calculatedHeaderChecksum) {
        LogError("Head checksum failed! Ensure only NDS ROMs are selected!");
        LogDebug("Expected: " << PrintHex(header.headerChecksum));
        LogDebug("Actual: " << PrintHex(calculatedHeaderChecksum));
        return false;
    }

    // Check the nintendo logo checksum.
    uint16_t calculatedNintendoLogoChecksum =
        getCRC16(header.nintendoLogo, NDS_CARTRIDGE_NINTENDO_LOGO_WIDTH);
    if (header.nintendoLogoChecksum != calculatedNintendoLogoChecksum) {
        LogError("Nintendo logo checksum failed!");
        LogDebug("Expected: " << PrintHex(header.nintendoLogoChecksum));
        LogDebug("Actual: " << PrintHex(calculatedNintendoLogoChecksum));
        return false;
    }

    // Todo!!! Support DSI games.
    // Catch non-nds games.
    if (header.unitCode != 0x0 && header.unitCode != 0x2) {
        LogError("Only NDS games are currently supported!");
        LogDebug("Unit code: " << PrintHex(header.unitCode));
        return false;
    }

    return true;
}
// ==================================================================================================
bool NDS_Cartridge::loadIconAndTitle() {
    uint8_t* iconTitleData = new uint8_t[NDS_CARTRIDGE_ICON_TITLE_WIDTH];
    if (iconTitleData == nullptr) {
        return false;
    }
    readFromROM(header.iconTitleOffset, NDS_CARTRIDGE_ICON_TITLE_WIDTH, iconTitleData);

    // Extract the english title string.
    uint16_t* tileData = reinterpret_cast<uint16_t*>(iconTitleData + 0x0340);
    for (unsigned int i = 0; i < NDS_CARTRIDGE_TITLE_MAX_SIZE; i++) {
        if (tileData[i] == 0) break;
        engTitleString += static_cast<char>(tileData[i]);
    }

    // Extract the palettes.
    iconPalettes[0] = 0xFFFFFFFF;
    uint16_t* paletteData = reinterpret_cast<uint16_t*>(iconTitleData + 0x0220);
    for (unsigned int i = 1; i < NDS_CARTRIDGE_ICON_NUM_PALETTES; i++) {
        uint8_t r = readBits(paletteData[i], 0, 4) << 3;
        uint8_t g = readBits(paletteData[i], 5, 9) << 3;
        uint8_t b = readBits(paletteData[i], 10, 14) << 3;

        uint32_t paletteColour = (0xFF << 24) | (r << 16) | (g << 8) | b;
        iconPalettes[i] = paletteColour;
    }

    // Extract the icon data.
    uint32_t* pixelData = reinterpret_cast<uint32_t*>(iconTitleData + 0x0020);
    for (unsigned int chunkNum = 0; chunkNum < NDS_CARTRIDGE_ICON_NUM_CHUNKS; chunkNum++) {
        unsigned int tileX = chunkNum % NDS_CARTRIDGE_ICON_CHUNKS_PER_ROW;
        unsigned int tileY = chunkNum / NDS_CARTRIDGE_ICON_CHUNKS_PER_ROW;
        for (unsigned int i = 0; i < NDS_CARTRIDGE_ICON_CHUNK_SIZE; i++) {
            unsigned int chunkX = i % NDS_CARTRIDGE_ICON_CHUNK_DIMENSION;
            unsigned int chunkY = i / NDS_CARTRIDGE_ICON_CHUNK_DIMENSION;
            uint32_t chunkData = pixelData[chunkNum * NDS_CARTRIDGE_ICON_CHUNK_DIMENSION + chunkY];

            unsigned int x = tileX * NDS_CARTRIDGE_ICON_CHUNK_DIMENSION + chunkX;
            unsigned int y = tileY * NDS_CARTRIDGE_ICON_CHUNK_DIMENSION + chunkY;
            iconPixels[y * NDS_CARTRIDGE_ICON_DIMENSION + x] =
                iconPalettes[(chunkData >> (chunkX * 4)) & 0xF];
        }
    }

    // TODO!!! extract DSI icon frames + animation data.

    delete[] iconTitleData;
    return true;
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
