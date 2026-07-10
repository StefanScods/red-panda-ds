#ifndef CORE_H
#define CORE_H

#include <chrono>
#include <queue>
#include <thread>
#include <unordered_set>

#include "cartridge.h"
#include "cpu.h"
#include "debugger.h"
#include "events.h"
#include "interconnect.h"
#include "lcd.h"

namespace RedPandaDS {
namespace Core {

constexpr int ARM9_CYCLE_RATIO = 1;
constexpr int ARM7_CYCLE_RATIO = 2;

#define APPLICATION_EMU_CORE_FPS 60  // fps

namespace ApplicationState {
enum ApplicationState {
    invalid = 0,
    stopped,
    paused,
    running,
};
}

class DSEmuCore {
public:
    DSEmuCore();
    ~DSEmuCore();

    friend class DebuggerInterface;
    // Provide public access to the debugger interface.
    DebuggerInterface debugger;

    // Component Accessors.
    Interconnect* getInterconnect() const { return bus; }
    ARM7TDMI* getARM7Core() const { return arm7; }
    ARM946ES* getARM9Core() const { return arm9; }
    NDS_LCD* getNDS_LCD() const { return ndsLCD; }
    NDS_Cartridge* getNDS_Cartridge() const { return ndsCartridge; }

    /**
     * @brief Initializes the core.
     */
    void init();

    /**
     * @brief Resets the core.
     */
    void reset();

    /**
     * @brief Starts execution of the NDS core.
     */
    void startExecution();

    /**
     * @brief Stops execution of the NDS core.
     */
    void stopExecution();

    /**
     * @brief Loads a ROM file.
     *
     * @param romFile The path to the ROM file to load.
     *
     * @return `bool`
     */
    bool loadROM(const std::string& romFile);

    /**
     * @brief Reloads the current ROM file if one is loaded.
     *
     * @return `bool`
     */
    bool reloadCurrentROM();

    /**
     * @brief Closes the current ROM file.
     */
    void closeROM();

    /**
     * @brief Run the main application state machine.
     */
    void runApplicationIteration();

    /**
     * @brief Adds en event to the Core event queue.
     *
     * @tparam T Type of event to add.
     *
     * @param args Args to pass to the event constructor. The core pointer is automatically passed
     * as the first argument.
     */
    template <typename T, typename... Args>
    void addEventToQueue(Args&&... args) {
        static_assert(std::is_base_of<CoreEvent, T>::value, "T must derive from CoreEvent");
        CoreEvent* newEvent = new T(this, std::forward<Args>(args)...);
        eventQueue.push(newEvent);
    }
    /**
     * @brief Processes the next event.
     *
     * @return `cycles` The number of cycles executed.
     */
    cycles processNextEvent();

    /**
     * @brief Performs all functionally at the end of a NDS frame.
     */
    void endNDSFrame();

    /**
     * @brief Gets the current state of the core.
     *
     * @return ApplicationState::ApplicationState
     */
    ApplicationState::ApplicationState getState() { return state; }

private:
    /**
     * @brief Sets the current state of the core.
     *
     * @param newState New state to set.
     */
    void setState(ApplicationState::ApplicationState newState);

public:
    /**
     * @brief Toggles the emulation state between paused and running.
     */
    void togglePausedState();

    /**
     * @brief Steps the CPU by one instruction during the next `runApplicationIteration()`.
     */
    void stepCPU();

    // Callback setters.
    void setOnFrameEndCallback(const std::function<void()>& callback) {
        assert(onFrameEndCallback == nullptr && "onFrameEndCallback already set!");
        onFrameEndCallback = callback;
    }
    void setOnStateChangeCallback(const std::function<void()>& callback) {
        assert(onStateChangeCallback == nullptr && "onStateChangeCallback already set!");
        onStateChangeCallback = callback;
    }

private:
    // Components.
    Interconnect* bus = nullptr;
    ARM7TDMI* arm7 = nullptr;
    NDS_ARM7_BIOS* arm7BIOS = nullptr;
    ARM946ES* arm9 = nullptr;
    NDS_ARM9_BIOS* arm9BIOS = nullptr;
    NDS_LCD* ndsLCD = nullptr;
    NDS_Cartridge* ndsCartridge = nullptr;

    // Callbacks.
    std::function<void()> onFrameEndCallback = nullptr;
    std::function<void()> onStateChangeCallback = nullptr;

    // FPS Trackers.
    std::chrono::_V2::system_clock::time_point endOfFrameTargetTime;
    const std::chrono::microseconds FPS_targetFrameTime{1000000 / APPLICATION_EMU_CORE_FPS};

    // Event queue and cycle counter.
    ApplicationState::ApplicationState state = ApplicationState::invalid;
    std::priority_queue<CoreEvent*, std::vector<CoreEvent*>, CompareCoreEvents> eventQueue;
    cycles currentCycle = 0;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
