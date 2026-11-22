#include "common.h"

using namespace RedPandaDS::Core;

// =================================================================================================
std::string getTestROMFile(std::string name) {
#ifdef TEST_ROM_OUTPUT
    return std::string(TEST_ROM_OUTPUT) + "/" + name + ".nds";
#else
    return name + ".nds";
#endif
}
// =================================================================================================
