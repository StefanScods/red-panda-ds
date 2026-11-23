#ifndef E2E_TEST_HARNESS
#define E2E_TEST_HARNESS

#include "types.h"

namespace RedPandaDS {
namespace Core {

#define E2E_RESULT_ADDR 0x023FFFFC

/**
 * @brief Packet for containing a test result.
 */
struct E2ETestResult {
    uint32_t result;
    uint64_t numIterations;
    cycles numCycles;
    uint32_t valid;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
