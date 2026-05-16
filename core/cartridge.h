#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <fstream>
#include <string>
#include <vector>

#include "types.h"

namespace RedPandaDS {
namespace Core {

#define NDS_CARTRIDGE_GAME_TITLE_WIDTH 12

#define NDS_CARTRIDGE_GAME_TITLE_WIDTH 12
#define NDS_CARTRIDGE_SECURE_AREA_DISABLE_WIDTH 8
#define NDS_CARTRIDGE_NINTENDO_LOGO_WIDTH 0x9C
#define NDS_CARTRIDGE_RESERVED_1_WIDTH 7
#define NDS_CARTRIDGE_RESERVED_2_WIDTH 1
#define NDS_CARTRIDGE_RESERVED_3_WIDTH 8
#define NDS_CARTRIDGE_RESERVED_4_WIDTH 0x18
#define NDS_CARTRIDGE_RESERVED_5_WIDTH 0x10
#define NDS_CARTRIDGE_UNKNOWN_WIDTH 4

#define NDS_CARTRIDGE_ICON_TITLE_WIDTH 0x2400
#define NDS_CARTRIDGE_TITLE_MAX_SIZE 0x100
#define NDS_CARTRIDGE_ICON_DIMENSION 32
#define NDS_CARTRIDGE_ICON_NUM_PALETTES 16
#define NDS_CARTRIDGE_ICON_CHUNK_DIMENSION 8
constexpr unsigned int NDS_CARTRIDGE_ICON_CHUNK_SIZE =
    NDS_CARTRIDGE_ICON_CHUNK_DIMENSION * NDS_CARTRIDGE_ICON_CHUNK_DIMENSION;
constexpr unsigned int NDS_CARTRIDGE_ICON_CHUNKS_PER_ROW =
    NDS_CARTRIDGE_ICON_DIMENSION / NDS_CARTRIDGE_ICON_CHUNK_DIMENSION;
constexpr unsigned int NDS_CARTRIDGE_ICON_NUM_CHUNKS =
    NDS_CARTRIDGE_ICON_DIMENSION * NDS_CARTRIDGE_ICON_DIMENSION / NDS_CARTRIDGE_ICON_CHUNK_SIZE;

struct CartridgeHeader {
    // 0x000
    uint8_t gameTitle[NDS_CARTRIDGE_GAME_TITLE_WIDTH];
    // 0x00C
    uint32_t gameCode;
    // 0x010
    uint16_t makerCode;
    // 0x012
    uint8_t unitCode;
    // 0x013
    uint8_t encryptionSeedSelect;
    // 0x014
    uint8_t deviceCapacity;
    // 0x015
    uint8_t reserved1[NDS_CARTRIDGE_RESERVED_1_WIDTH];
    // 0x01C
    uint8_t reserved2[NDS_CARTRIDGE_RESERVED_2_WIDTH];
    // 0x01D
    uint8_t ndsRegion;
    // 0x01E
    uint8_t romVersion;
    // 0x01F
    uint8_t autoStart;
    // 0x020
    uint32_t arm9RomOffset;
    // 0x024
    uint32_t arm9EntryAddress;
    // 0x028
    uint32_t arm9RamAddress;
    // 0x02C
    uint32_t arm9Size;
    // 0x030
    uint32_t arm7RomOffset;
    // 0x034
    uint32_t arm7EntryAddress;
    // 0x038
    uint32_t arm7RamAddress;
    // 0x03C
    uint32_t arm7Size;
    // 0x040
    uint32_t fntOffset;
    // 0x044
    uint32_t fntSize;
    // 0x048
    uint32_t fatOffset;
    // 0x04C
    uint32_t fatSize;
    // 0x050
    uint32_t arm9OverlayOffset;
    // 0x054
    uint32_t arm9OverlaySize;
    // 0x058
    uint32_t arm7OverlayOffset;
    // 0x05C
    uint32_t arm7OverlaySize;
    // 0x060
    uint32_t normalCommandSettings;
    // 0x064
    uint32_t key1CommandSettings;
    // 0x068
    uint32_t iconTitleOffset;
    // 0x06C
    uint16_t secureAreaChecksum;
    // 0x06E
    uint16_t secureAreaDelay;
    // 0x070
    uint32_t arm9AutoLoadHookRamAddress;
    // 0x074
    uint32_t arm7AutoLoadHookRamAddress;
    // 0x078
    uint8_t secureAreaDisable[NDS_CARTRIDGE_SECURE_AREA_DISABLE_WIDTH];
    // 0x080
    uint32_t totalUsedRomSize;
    // 0x084
    uint32_t romHeaderSize;
    // 0x088
    uint8_t unknown[NDS_CARTRIDGE_UNKNOWN_WIDTH];
    // 0x08C
    uint8_t reserved3[NDS_CARTRIDGE_RESERVED_3_WIDTH];
    // 0x094
    uint16_t nandEndOfRomArea;
    // 0x096
    uint16_t nandStartOfRwArea;
    // 0x098
    uint8_t reserved4[NDS_CARTRIDGE_RESERVED_4_WIDTH];
    // 0x0B0
    uint8_t reserved5[NDS_CARTRIDGE_RESERVED_5_WIDTH];
    // 0x0C0
    uint8_t nintendoLogo[NDS_CARTRIDGE_NINTENDO_LOGO_WIDTH];
    // 0x15C
    uint16_t nintendoLogoChecksum;
    // 0x15E
    uint16_t headerChecksum;
    // 0x160
    uint32_t debugRomOffset;
    // 0x164
    uint32_t debugSize;
    // 0x168
    uint32_t debugRamAddress;

    std::string getGameTitle() const;
    std::string getGameCode() const;
};

class NDS_Cartridge {
public:
    NDS_Cartridge();

    /**
     * @brief Get the loaded ROM file path.
     *
     * @return `const std::string&`
     */
    const std::string& getLoadedROMFilePath() const { return loadedFilePath; }

    /**
     * @brief Close the current ROM.
     */
    void closeROM();

    /**
     * @brief Reset the cartridge componentent without clearing the selected ROM.
     */
    void reset();

    /**
     * @brief Returns `true` if a ROM file is loaded in the 'cartridge'.
     *
     * @return `bool`
     */
    bool isOpen();

    /**
     * @brief Loads a ROM file and creates a "cartridge" interface. Returns false upon failure.
     *
     * @param filepath Path to the ROM file to load.
     *
     * @return `bool`
     */
    bool loadROMFromFile(const std::string& filepath);

    /**
     * @brief Get a read-only interface to the cartridge header.
     *
     * @return `const CartridgeHeader&`
     */
    const CartridgeHeader& getHeader() const { return header; }

    /**
     * @brief Get a read-only interface to the cartridge english title.
     *
     * @return `const std::string&`
     */
    const std::string& getEngTitleString() const { return engTitleString; }

    /**
     * @brief Get a read-only interface to the cartridge icon pixel data.
     *
     * @return `const std::vector<uint32_t>&`
     */
    const std::vector<uint32_t> getIconPixels() const { return iconPixels; }

    /**
     * @brief Reads from the opened ROM file.
     *
     * @param addr Address of the ROM file.
     * @param size Number of bytes to read.
     * @param destBuffer Output buffer to read into.
     */
    void readFromROM(uint32_t addr, uint32_t size, uint8_t* destBuffer);

private:
    /**
     * @brief Helper function for `loadROMFromFile()`. Loads and validates the header section of the
     * ROM. Returns false upon failure.
     *
     * @return `bool`
     */
    bool loadHeader();

    /**
     * @brief Helper function for `loadROMFromFile()`. Loads the icon and title string from the
     * ROM. Returns false upon failure.
     *
     * @return `bool`
     */
    bool loadIconAndTitle();

    std::ifstream file;

    std::string loadedFilePath;
    CartridgeHeader header;
    std::string engTitleString;
    std::vector<uint32_t> iconPixels;
    std::vector<uint32_t> iconPalettes;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
