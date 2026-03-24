#ifndef LCD_H
#define LCD_H

#include <atomic>
#include <vector>

namespace RedPandaDS {
namespace Core {

constexpr int DS_LCD_WIDTH = 256;
constexpr int DS_LCD_HEIGHT = 192;

class NDS_LCD {
public:
    NDS_LCD();
    ~NDS_LCD();
    /**
     * @brief Set all vars back to their initial value.
     */
    void reset();
    /**
     * @brief Swap the work and render buffers.
     */
    void swapBuffers();

    // Accessors.
    std::vector<uint32_t>* getTopScreenDrawBuffer() {
        return topScreenDrawBuffer.load(std::memory_order_acquire);
    }
    std::vector<uint32_t>* getBottomScreenDrawBuffer() {
        return bottomScreenDrawBuffer.load(std::memory_order_acquire);
    }
    std::vector<uint32_t>* getTopScreenWorkBuffer() {
        return topScreenWorkBuffer.load(std::memory_order_acquire);
    }
    std::vector<uint32_t>* getBottomScreenWorkBuffer() {
        return bottomScreenWorkBuffer.load(std::memory_order_acquire);
    }

private:
    std::vector<uint32_t> topScreenBuffer1;
    std::vector<uint32_t> topScreenBuffer2;
    std::vector<uint32_t> bottomScreenBuffer1;
    std::vector<uint32_t> bottomScreenBuffer2;

    std::atomic<std::vector<uint32_t>*> topScreenDrawBuffer;
    std::atomic<std::vector<uint32_t>*> bottomScreenDrawBuffer;

    std::atomic<std::vector<uint32_t>*> topScreenWorkBuffer;
    std::atomic<std::vector<uint32_t>*> bottomScreenWorkBuffer;
};

}  // namespace Core
}  // namespace RedPandaDS
#endif
