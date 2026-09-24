#ifndef DMA_H
#define DMA_H

namespace RedPandaDS {
namespace Core {

/**
 * @brief Provides an interface to the ARM DMA controller.
 */
struct DMAController {
    DMAController();
    /**
     * @brief Set all vars back to their initial value.
     */
    void reset();
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
