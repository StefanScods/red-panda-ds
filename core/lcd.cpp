#include "lcd.h"

namespace RedPandaDS {
namespace Core {
// ==================================================================================================
NDS_LCD::NDS_LCD() {
    topScreenBuffer1.resize(DS_LCD_WIDTH * DS_LCD_HEIGHT);
    topScreenBuffer2.resize(DS_LCD_WIDTH * DS_LCD_HEIGHT);
    bottomScreenBuffer1.resize(DS_LCD_WIDTH * DS_LCD_HEIGHT);
    bottomScreenBuffer2.resize(DS_LCD_WIDTH * DS_LCD_HEIGHT);
    // reset();
}
// ==================================================================================================
NDS_LCD::~NDS_LCD() {
}
// ==================================================================================================
void NDS_LCD::reset() {
    // Zero out the displays.
    for (uint32_t i = 0; i < DS_LCD_WIDTH; i++) {
        for (uint32_t j = 0; j < DS_LCD_HEIGHT; j++) {
            uint32_t index = j * DS_LCD_WIDTH + i;
            topScreenBuffer1[index] = 0xff000000;
            topScreenBuffer2[index] = 0xff000000;
            bottomScreenBuffer1[index] = 0xff000000;
            bottomScreenBuffer2[index] = 0xff000000;
        }
    }
    topScreenDrawBuffer.store(&topScreenBuffer1, std::memory_order_release);
    bottomScreenDrawBuffer.store(&bottomScreenBuffer1, std::memory_order_release);
    topScreenWorkBuffer.store(&topScreenBuffer2, std::memory_order_release);
    bottomScreenWorkBuffer.store(&bottomScreenBuffer2, std::memory_order_release);
}
// ==================================================================================================
void NDS_LCD::swapBuffers() {
    std::vector<uint32_t>* topWorkBuf = topScreenWorkBuffer.load(std::memory_order_acquire);
    std::vector<uint32_t>* bottomWorkBuf = bottomScreenWorkBuffer.load(std::memory_order_acquire);
    // Render the finished work buffers.
    topScreenDrawBuffer.store(topWorkBuf, std::memory_order_release);
    bottomScreenDrawBuffer.store(bottomWorkBuf, std::memory_order_release);
    // Get new work buffers.
    topScreenWorkBuffer.store(
        (topWorkBuf == &topScreenBuffer1) ? &topScreenBuffer2 : &topScreenBuffer1,
        std::memory_order_release);
    bottomScreenWorkBuffer.store(
        (bottomWorkBuf == &bottomScreenBuffer1) ? &bottomScreenBuffer2 : &bottomScreenBuffer1,
        std::memory_order_release);
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
