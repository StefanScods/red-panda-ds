#include "events.h"

#include "core.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {
// ==================================================================================================
StandardFrameEvent::StandardFrameEvent(DSEmuCore* d_core, cycles currentTimestamp)
    : CoreEvent(d_core) {
    timestamp = currentTimestamp + NDS_ARM9_FRAME_CYCLES;
    target = EventTargetComponent::AnyCPU;
    onEventFinishCallback = [this]() {
        core->endNDSFrame();
        // On event finished, add the next frame to the event queue.
        core->addEventToQueue<StandardFrameEvent>(timestamp);
    };
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
