#include "dma.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
DMAController::DMAController() {
    reset();
}
// ==================================================================================================
void DMAController::reset() {
    for (uint32_t i = 0; i < NUM_DMA_CHANNELS; i++) {
        channels[i] = {0};
    }
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
