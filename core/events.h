#ifndef CORE_EVENTS_H
#define CORE_EVENTS_H

#include <functional>
#include <type_traits>
#include <utility>

#include "types.h"

namespace RedPandaDS {
namespace Core {

// Forward declare.
class DSEmuCore;

namespace EventTargetComponent {
enum EventTargetComponent : uint8_t { AnyCPU = 0, ARM7, ARM9 };
}

/**
 * @brief Defines an emulator Core event.
 *
 * @param timestamp The timestamp to finish this event.
 * @param target The component to target.
 * @param onEventFinishCallback Callback function to invoke once the event timestamp is processed.
 */
struct CoreEvent {
    CoreEvent(DSEmuCore* d_core) : core(d_core) {};
    virtual ~CoreEvent() {};

    DSEmuCore* core = nullptr;
    cycles timestamp = 0;
    EventTargetComponent::EventTargetComponent target;
    std::function<void()> onEventFinishCallback = nullptr;
};

constexpr int NDS_ARM9_FRAME_CYCLES = 66 * 1024 * 1024 / 60;
/**
 * @brief Defines a standard frame event for the emulator.
 */
struct StandardFrameEvent : public CoreEvent {
    StandardFrameEvent(DSEmuCore* d_core, cycles currentTimestamp);
};

/**
 * @brief Helper compare for two events in the event queue.
 */
struct CompareCoreEvents {
    bool operator()(const CoreEvent* a, const CoreEvent* b) const {
        return a->timestamp > b->timestamp;
    }
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
