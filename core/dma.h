#ifndef DMA_H
#define DMA_H

#include "types.h"

namespace RedPandaDS {
namespace Core {

#define NUM_DMA_CHANNELS 4

namespace DMAState {
enum DMAState : uint32_t { DISABLED = 0, PENDING, TRIGGERED };
}

struct DMAChannelConfig {
    uint32_t sourceAddress;
    uint32_t destinationAddress;
    uint32_t wordCount;
    uint8_t destControl;
    uint8_t sourceControl;
    bool repeat;
    bool dataSize;
    bool bit11;
    uint8_t startCondition;
    uint8_t irqEnable;
    uint8_t enable;

    DMAState::DMAState state;
    uint32_t currentSource;
    uint32_t currentDest;
    uint32_t progress;
    cycles cooldown;
};

/**
 * @brief Provides an interface to the ARM DMA controller.
 */
struct DMAController {
    DMAController();
    /**
     * @brief Set all vars back to their initial value.
     */
    void reset();

    DMAChannelConfig channels[NUM_DMA_CHANNELS];
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
