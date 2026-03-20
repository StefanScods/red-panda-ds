#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <queue>

#include "types.h"
#include "utils.h"

namespace RedPandaDS {
namespace Core {

// Forward declarations.
class Interconnect;

// TODO!!! Make sure this is an invalid instruction for both CPUs
#define NO_INSTRUCT 0b11101100000000000000000000000000

#define NUM_OF_STANDARD_REGISTERS 16
#define NUM_OF_FIQ_REGISTERS 7
#define NUM_OF_SVC_REGISTERS 2
#define NUM_OF_ABT_REGISTERS 2
#define NUM_OF_IRQ_REGISTERS 2
#define NUM_OF_UND_REGISTERS 2

#define INSTUCTION_PIPELINE_LENGTH 3

// Negative Condition Flag.
#define N_FLAG 31
// Zero Condition Flag
#define Z_FLAG 30
// Carry Condition Flag
#define C_FLAG 29
// Overflow Condition Flag
#define V_FLAG 28
// Cumulative saturation Flag
#define Q_BIT 27
// Disable IRQ interrupts
#define I_BIT 7
// Disable FIQ interrupts
#define F_BIT 6
// Thumb execution state bit
#define T_BIT 5
// Mode upper bound
#define MODE_UPPER_BIT 4
// Mode lower bound
#define MODE_LOWER_BIT 0

#define SP_REGISTER_NUM 13
#define LR_REGISTER_NUM 14
#define PC_REGISTER_NUM 15

// Condition codes
// https://developer.arm.com/documentation/ddi0406/c/Application-Level-Architecture/Instruction-Details/Conditional-execution?lang=en#Chdcgdjb
namespace ConditionMnemonics {
enum ConditionMnemonics : uint8_t {
    EQ = 0b0000,
    NE = 0b0001,
    CS = 0b0010,
    CC = 0b0011,
    MI = 0b0100,
    PL = 0b0101,
    VS = 0b0110,
    VC = 0b0111,
    HI = 0b1000,
    LS = 0b1001,
    GE = 0b1010,
    LT = 0b1011,
    GT = 0b1100,
    LE = 0b1101,
    AL = 0b1110,
    SPECIAL = 0b1111
};
inline const char* toString(ConditionMnemonics code) {
    switch (code) {
        case EQ:
            return "eq";
        case NE:
            return "ne";
        case CS:
            return "cs";
        case CC:
            return "cc";
        case MI:
            return "mi";
        case PL:
            return "pl";
        case VS:
            return "vs";
        case VC:
            return "vc";
        case HI:
            return "hi";
        case LS:
            return "ls";
        case GE:
            return "ge";
        case LT:
            return "lt";
        case GT:
            return "gt";
        case LE:
            return "le";
        // Dont render these.
        case AL:
        case SPECIAL:
            return "";
        default:
            return "unknown";
    }
}
}  // namespace ConditionMnemonics
// https://developer.arm.com/documentation/ddi0406/c/System-Level-Architecture/The-System-Level-Programmers--Model/ARM-processor-modes-and-ARM-core-registers/Program-Status-Registers--PSRs-?lang=en#CIHBFGJG
namespace ProcessorModes {
enum ProcessorModes : uint8_t {
    User = 0b10000,
    FIQ = 0b10001,
    IRQ = 0b10010,
    Supervisor = 0b10011,
    Abort = 0b10111,
    Undefined = 0b11011,
    System = 0b11111,
};
inline const char* toString(ProcessorModes mode) {
    switch (mode) {
        case User:
            return "User";
        case FIQ:
            return "FIQ";
        case IRQ:
            return "IRQ";
        case Supervisor:
            return "Supervisor";
        case Abort:
            return "Abort";
        case Undefined:
            return "Undefined";
        case System:
            return "System";
        default:
            return "Unknown";
    }
}
}  // namespace ProcessorModes

extern std::vector<std::string> g_regNames;

/**
 * @brief The base CPU object to implement common logic shared between the NDS's
 * two CPUs.
 */
class ARM {
protected:
    bool arm9 = false;
    Interconnect* bus = nullptr;

    uint32_t reg[NUM_OF_STANDARD_REGISTERS];
    uint32_t cpsr;
    // Banked registers.
    uint32_t spsrFIQ;
    uint32_t regFIQ[NUM_OF_FIQ_REGISTERS];
    uint32_t spsrSVC;
    uint32_t regSVC[NUM_OF_SVC_REGISTERS];
    uint32_t spsrABT;
    uint32_t regABT[NUM_OF_ABT_REGISTERS];
    uint32_t spsrIRQ;
    uint32_t regIRQ[NUM_OF_IRQ_REGISTERS];
    uint32_t spsrUND;
    uint32_t regUND[NUM_OF_UND_REGISTERS];

    // Active registers.
    uint32_t* activeRegs[NUM_OF_STANDARD_REGISTERS] = {
        &(reg[0]),  &(reg[1]),  &(reg[2]),  &(reg[3]), &(reg[4]),  &(reg[5]),
        &(reg[6]),  &(reg[7]),  &(reg[8]),  &(reg[9]), &(reg[10]), &(reg[11]),
        &(reg[12]), &(reg[13]), &(reg[14]), &(reg[15])};
    uint32_t* spsr = nullptr;

    // Program counter maps to the 15th standard register in all modes.
    inline uint32_t& pc() { return *activeRegs[PC_REGISTER_NUM]; }
    // Link register maps to the 14th register.
    inline uint32_t& lr() { return *activeRegs[LR_REGISTER_NUM]; }
    // Stack pointer maps to the 13th register.
    inline uint32_t& sp() { return *activeRegs[SP_REGISTER_NUM]; }

    // Keep track of the component's cycle time.
    cycles fetchCooldown = 0;
    cycles executeCooldown = 0;
    cycles cyclesElapsed = 0;
    cycles currentCycle = 0;
    cycles targetCycle = 0;
    bool justBranched = false;

    // Used to keep track of sequencial vs non sequencial accesss.
    uint32_t previousCodeAddr = 0;
    uint32_t previousDataAddr = 0;

    std::queue<uint32_t> instructionQueue;
    uint32_t instuctionPipeLine[INSTUCTION_PIPELINE_LENGTH];

    // Cycle timing map.
    cycles code_sequencial32BitAccessTimings[0xFF] = {0};
    cycles code_nonSequencial32BitAccessTimings[0xFF] = {0};
    cycles code_sequencial16BitAccessTimings[0xFF] = {0};
    cycles code_nonSequencial16BitAccessTimings[0xFF] = {0};
    cycles data_sequencial32BitAccessTimings[0xFF] = {0};
    cycles data_nonSequencial32BitAccessTimings[0xFF] = {0};
    cycles data_sequencial16BitAccessTimings[0xFF] = {0};
    cycles data_nonSequencial16BitAccessTimings[0xFF] = {0};

    /**
     * @brief adds the current value of `cyclesElapsed` to the `currentCycle` and reduces fetch
     * and execute cooldowns. Zeros `cyclesElapsed`.
     *
     */
    inline void addCyclesElapsed() {
        currentCycle += cyclesElapsed;
        fetchCooldown = underflowSafeSubtract(fetchCooldown, cyclesElapsed);
        executeCooldown = underflowSafeSubtract(executeCooldown, cyclesElapsed);
        cyclesElapsed = 0;
    }

public:
    ARM();
    ~ARM();

    /**
     * @brief Get the name of the CPU.
     *
     * @return std::string
     */
    virtual std::string getCPUName() { return "ARM"; }

    /**
     * @brief Returns true if this CPU implements arm7.
     */
    bool isARM7() { return !arm9; }
    /**
     * @brief Returns true if this CPU implements arm9.
     */
    bool isARM9() { return arm9; }

    /**
     * @brief Set all vars back to their initial value.
     */
    void reset();

    /**
     * @brief Execute the instruction at instuctionPipeLine[0]. This clears instuctionPipeLine[0].
     *  Returns the number of cycles the execution took.
     *
     * @return cycles
     */
    virtual cycles execute();

    /**
     * @brief Execute the instruction at instuctionPipeLine[0] during ARM Mode.
     *
     * @return cycles
     */
    virtual cycles ARM_execute();

    /**
     * @brief Execute the instruction at instuctionPipeLine[0] during THUMB Mode.
     *
     * @return cycles
     */
    virtual cycles THUMB_execute();

    /**
     * @brief Fetch the next instuction. This increments PC and fills
     * instuctionPipeLine[2]. Returns the number of cycles the fetching took.
     *
     * @return cycles
     */
    virtual cycles fetch();

    /**
     * @brief Main cycle logic of the module.
     *
     * @return cycles
     */
    virtual cycles cycle();

    /**
     * @brief Keep cycling until the next instuction is executed. Cycle behaviour may not be
     * correct so use for testing.
     *
     * @param numExecutions The number of time to fetch and execute
     *
     * @return cycles
     */
    cycles fetchAndExecute(int numExecutions = 1);

    /**
     * @brief Clear the instruction pipeline.
     */
    void clearInstructionPipeline();

    /**
     * @brief Move the instruction pipeline by one stage.
     */
    void advanceInstructionPipeline();

    /**
     * @brief Debug function to force PC to a value.
     * @param value
     */
    void setPC(uint32_t value) {
        pc() = value;
        clearInstructionPipeline();
    }

    /**
     * @brief Read a register value.
     * @param regNum (0-15)
     */
    uint32_t readReg(uint32_t regNum) const { return *activeRegs[regNum]; }
    /**
     * @brief Read a register value form a specific mode.
     *
     * @param mode The mode to read from.
     * @param regNum (0-15) (16 - CPSR) (17 - SPSR)
     */
    uint32_t readModeReg(ProcessorModes::ProcessorModes mode, uint32_t regNum) const;

    /**
     * @brief Write to a register.
     * @param regNum (0-15)
     * @param data Data to write
     */
    void writeReg(uint32_t regNum, uint32_t data) { *activeRegs[regNum] = data; }

    /**
     * @brief Read a cpu flag value.
     * @param flagBit
     */
    bool readFlag(uint32_t flagBit) const { return readBit(cpsr, flagBit); }

    /**
     * @brief Write to a cpu flag value.
     * @param flagBit
     */
    void setFlag(uint32_t flagBit, bool data) { writeBit(cpsr, data, flagBit); }

    /**
     * @brief Get the current processor mode.
     *
     * @return `ProcessorModes::ProcessorModes`
     */
    ProcessorModes::ProcessorModes getProcessorMode() {
        return ProcessorModes::ProcessorModes(cpsr & 0x1F);
    }
    /**
     * @brief Set the current processor mode.
     *
     * @param mode Mode to switch too.
     */
    void setProcessorMode(ProcessorModes::ProcessorModes mode);

    /**
     * @brief Get whether the processor is in thumb mode.
     *
     * @return `bool`
     */
    bool getThumbMode() { return readBit(cpsr, T_BIT); }

    /**
     * @brief Set the processor's thumb mode.
     *
     * @param mode `True` if targeting thumb mode.
     */
    void setThumbMode(bool thumb) { return writeBit(cpsr, thumb, T_BIT); }

    /**
     * @brief Write to the SPSR. Will do nothing if SPSR is not
     * supported in the current mode.
     *
     * @param data The data to write.
     */
    void setSPSR(uint32_t data) {
        if (spsr == nullptr) return;
        *spsr = data;
    }

    /**
     * @brief Read from the SPSR. Will always return 0 if SPSR is
     *  not supported in the current mode.
     *
     * @return `uint32_t`
     */
    uint32_t readSPSR() {
        if (spsr == nullptr) return 0;
        return *spsr;
    }

    /**
     * @brief Write to the CPSR.
     *
     * @param data The data to write.
     */
    void setCPSR(uint32_t data);

    /**
     * @brief Read from the CPSR
     *
     * @return `uint32_t`
     */
    uint32_t readCPSR() { return cpsr; }

    /**
     * @brief Read the bus.
     *
     * @param address Address to read. Address should be word-aligned for 32bit reads or
     * halfword-aligned for 16 bit reads.
     * @param size Size of data to fetch in bits (32, 16, or 8). Default is 32 bits.
     * @param codeRead Whether this read is apart of an opcode fetch. Effects cycle time.
     * @return busPayload
     */
    virtual busPayload readBus(uint32_t address, uint32_t size = 32, bool codeRead = false);

    /**
     * @brief Write to the bus.
     *
     * @param address Address to write. Address should be word-aligned for 32bit writes or
     * halfword-aligned for 16 bit writes.
     * @param data The data to write.
     * @param size Size of data to write in bits (32, 16, or 8). Default is 32 bits.
     * @return busPayload
     */
    virtual busPayload writeBus(uint32_t address, uint32_t data, uint32_t size = 32);

    /**
     * @brief Sets the `bus` property to the supplied argument. This function must be called
     * before any execution takes place.
     *
     * @param d_bus The interconnect to "connect" to.
     */
    void connectToInterconnect(Interconnect* d_bus) { bus = d_bus; }

    /**
     * @brief Sets the target number of cycles -> determines how long to execute for.
     *
     * @param target Target number of cycles to set this component too.
     */
    void setTargetCycle(cycles target) { targetCycle = target; }

    /**
     * @brief Given the current state of the CPU flags, check if a condition is passed. Returns
     * `true` if the condition passed.
     *
     * @param condition The mnemonic of the condition to check.
     *
     * @return `bool`
     */
    bool checkIfConditionPassed(ConditionMnemonics::ConditionMnemonics condition);

    /**
     * @brief Update the CPU after a processor mode change has occurred.
     */
    void handleProcessorModeChange();

    /**
     * @brief Updates PC + performs any processing after PC has been updated.
     *
     * @param dest The new address to branch to.
     * @param exchange If the processor should change modes after a branch.
     */
    void branch(uint32_t dest, bool exchange = true);

    /**
     * @brief Call after updating an arbitrary reg to ensure writes to PC are handled correctly.
     *
     * @param destReg The register being updated. No op if dest register is not the PC.
     */
    void fixupIfTargetingPC(uint32_t destReg);

    /**
     * @brief Check if the passed operand saturates the given bitwidth. If saturated, returns the
     * saturated value and sets the Q flag. If the value does not saturate, the operand will return
     * and the Q flag will be untouched (remain 0 or 1).
     *
     * @param operand Value to check saturation.
     * @param bitSize The number of bits implementing the integer.
     *
     * @return `int64_t`
     */
    int64_t signedSaturatedQ(int64_t operand, uint8_t bitSize);

    // =============================================================================================
    // ARM Instruction set.
    // =============================================================================================
    // Invalid instruction.
    cycles ARM_UNDEFINED_INST(uint32_t instruct);
    // Decode helpers.
    cycles ARM_dataProcessingDecodeAndExecute(uint32_t instruct, uint8_t cond);
    cycles ARM_loadStoreDecodeAndExecute(uint32_t instruct, uint8_t cond);
    cycles ARM_branchAndBlockDataTransferDecodeAndExecute(uint32_t instruct, uint8_t cond);
    cycles ARM_coprocessorAndSupervisorDecodeAndExecute(uint32_t instruct, uint8_t cond);
    // Data category.
    cycles ARM_AND(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles ARM_AND_REG(uint32_t instruct);
    cycles ARM_AND_REG_SHIFT(uint32_t instruct);
    cycles ARM_AND_IMM(uint32_t instruct);
    cycles ARM_EOR(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles ARM_EOR_REG(uint32_t instruct);
    cycles ARM_EOR_REG_SHIFT(uint32_t instruct);
    cycles ARM_EOR_IMM(uint32_t instruct);
    cycles ARM_SUB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags);
    cycles ARM_SUB_REG(uint32_t instruct);
    cycles ARM_SUB_REG_SHIFT(uint32_t instruct);
    cycles ARM_SUB_IMM(uint32_t instruct);
    cycles ARM_RSB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags);
    cycles ARM_RSB_REG(uint32_t instruct);
    cycles ARM_RSB_REG_SHIFT(uint32_t instruct);
    cycles ARM_RSB_IMM(uint32_t instruct);
    cycles ARM_ADD(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags);
    cycles ARM_ADD_REG(uint32_t instruct);
    cycles ARM_ADD_REG_SHIFT(uint32_t instruct);
    cycles ARM_ADD_IMM(uint32_t instruct);
    cycles ARM_ADC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles ARM_ADC_REG(uint32_t instruct);
    cycles ARM_ADC_REG_SHIFT(uint32_t instruct);
    cycles ARM_ADC_IMM(uint32_t instruct);
    cycles ARM_SBC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles ARM_SBC_REG(uint32_t instruct);
    cycles ARM_SBC_REG_SHIFT(uint32_t instruct);
    cycles ARM_SBC_IMM(uint32_t instruct);
    cycles ARM_RSC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles ARM_RSC_REG(uint32_t instruct);
    cycles ARM_RSC_REG_SHIFT(uint32_t instruct);
    cycles ARM_RSC_IMM(uint32_t instruct);
    cycles ARM_TST(uint32_t opp1, uint32_t opp2, bool carry);
    cycles ARM_TST_REG(uint32_t instruct);
    cycles ARM_TST_REG_SHIFT(uint32_t instruct);
    cycles ARM_TST_IMM(uint32_t instruct);
    cycles ARM_TEQ(uint32_t opp1, uint32_t opp2, bool carry);
    cycles ARM_TEQ_REG(uint32_t instruct);
    cycles ARM_TEQ_REG_SHIFT(uint32_t instruct);
    cycles ARM_TEQ_IMM(uint32_t instruct);
    cycles ARM_CMP(uint32_t opp1, uint32_t opp2);
    cycles ARM_CMP_REG(uint32_t instruct);
    cycles ARM_CMP_REG_SHIFT(uint32_t instruct);
    cycles ARM_CMP_IMM(uint32_t instruct);
    cycles ARM_CMN(uint32_t opp1, uint32_t opp2);
    cycles ARM_CMN_REG(uint32_t instruct);
    cycles ARM_CMN_REG_SHIFT(uint32_t instruct);
    cycles ARM_CMN_IMM(uint32_t instruct);
    cycles ARM_ORR(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles ARM_ORR_REG(uint32_t instruct);
    cycles ARM_ORR_REG_SHIFT(uint32_t instruct);
    cycles ARM_ORR_IMM(uint32_t instruct);
    cycles ARM_MOV(uint32_t desReg, uint32_t srcValue, bool carry, bool setFlags);
    cycles ARM_MOV_REG(uint32_t instruct);
    cycles ARM_MOV_REG_SHIFT(uint32_t instruct);
    cycles ARM_MOV_IMM(uint32_t instruct);
    cycles ARM_BIC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles ARM_BIC_REG(uint32_t instruct);
    cycles ARM_BIC_REG_SHIFT(uint32_t instruct);
    cycles ARM_BIC_IMM(uint32_t instruct);
    cycles ARM_MVN(uint32_t desReg, uint32_t opp1, bool carry, bool setFlags);
    cycles ARM_MVN_REG(uint32_t instruct);
    cycles ARM_MVN_REG_SHIFT(uint32_t instruct);
    cycles ARM_MVN_IMM(uint32_t instruct);
    cycles ARM_MUL(uint32_t instruct);
    cycles ARM_MLA(uint32_t instruct);
    cycles ARM_UMAAL(uint32_t instruct);
    cycles ARM_MLS(uint32_t instruct);
    cycles ARM_UMULL(uint32_t instruct);
    cycles ARM_UMLAL(uint32_t instruct);
    cycles ARM_SMULL(uint32_t instruct);
    cycles ARM_SMLAL(uint32_t instruct);
    cycles ARM_SMLABB(uint32_t instruct);
    cycles ARM_SMLABT(uint32_t instruct);
    cycles ARM_SMLATB(uint32_t instruct);
    cycles ARM_SMLATT(uint32_t instruct);
    cycles ARM_SMLAWB(uint32_t instruct);
    cycles ARM_SMLAWT(uint32_t instruct);
    cycles ARM_SMLA(uint32_t desReg, int32_t opp1, int32_t opp2, int32_t addend);
    cycles ARM_SMULBB(uint32_t instruct);
    cycles ARM_SMULBT(uint32_t instruct);
    cycles ARM_SMULTB(uint32_t instruct);
    cycles ARM_SMULTT(uint32_t instruct);
    cycles ARM_SMULWB(uint32_t instruct);
    cycles ARM_SMULWT(uint32_t instruct);
    cycles ARM_SMUL(uint32_t desReg, int32_t opp1, int32_t opp2);
    cycles ARM_SMLALBB(uint32_t instruct);
    cycles ARM_SMLALBT(uint32_t instruct);
    cycles ARM_SMLALTB(uint32_t instruct);
    cycles ARM_SMLALTT(uint32_t instruct);
    cycles ARM_SMLAL(uint32_t desRegLow, uint32_t desRegHigh, int64_t opp1, int64_t opp2,
                     int64_t addend);
    cycles ARM_MRS(uint32_t instruct);
    cycles ARM_MSR_REG(uint32_t instruct);
    cycles ARM_MSR_IMM(uint32_t instruct);
    cycles ARM_MSR(bool readSPSR, uint32_t value, uint32_t maskBits);
    cycles ARM_CLZ(uint32_t instruct);
    cycles ARM_QADD(uint32_t instruct);
    cycles ARM_QSUB(uint32_t instruct);
    cycles ARM_QDADD(uint32_t instruct);
    cycles ARM_QDSUB(uint32_t instruct);
    // Load and store category.
    cycles ARM_STR(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                   bool wback);
    cycles ARM_STRT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add);
    cycles ARM_STRB(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback);
    cycles ARM_STRBT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add);
    cycles ARM_LDR(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                   bool wback);
    cycles ARM_LDRT(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool add);
    cycles ARM_LDRB(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback);
    cycles ARM_LDRBT(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool add);
    cycles ARM_STRH_REG(uint32_t instruct);
    cycles ARM_STRH_IMM(uint32_t instruct);
    cycles ARM_STRH(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback);
    cycles ARM_STRD_REG(uint32_t instruct);
    cycles ARM_STRD_IMM(uint32_t instruct);
    cycles ARM_STRD(uint32_t srcReg1, uint32_t srcReg2, uint32_t baseReg, uint32_t offset, bool pre,
                    bool add, bool wback);
    cycles ARM_LDRH_REG(uint32_t instruct);
    cycles ARM_LDRH_IMM(uint32_t instruct);
    cycles ARM_LDRH(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback);
    cycles ARM_LDRD_REG(uint32_t instruct);
    cycles ARM_LDRD_IMM(uint32_t instruct);
    cycles ARM_LDRD(uint32_t desReg1, uint32_t desReg2, uint32_t baseReg, uint32_t offset, bool pre,
                    bool add, bool wback);
    cycles ARM_LDRSB_REG(uint32_t instruct);
    cycles ARM_LDRSB_IMM(uint32_t instruct);
    cycles ARM_LDRSB(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback);
    cycles ARM_LDRSH_REG(uint32_t instruct);
    cycles ARM_LDRSH_IMM(uint32_t instruct);
    cycles ARM_LDRSH(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback);
    cycles ARM_STMIA(uint32_t instruct);
    cycles ARM_STMIB(uint32_t instruct);
    cycles ARM_STMI(uint32_t baseReg, uint32_t registerList, bool pre, bool wback);
    cycles ARM_STMDA(uint32_t instruct);
    cycles ARM_STMDB(uint32_t instruct);
    cycles ARM_STMD(uint32_t baseReg, uint32_t registerList, bool pre, bool wback);
    cycles ARM_STM_USER_REG(uint32_t instruct);
    cycles ARM_LDMIA(uint32_t instruct);
    cycles ARM_LDMIB(uint32_t instruct);
    cycles ARM_LDMI(uint32_t baseReg, uint32_t registerList, bool pre, bool wback);
    cycles ARM_LDMDA(uint32_t instruct);
    cycles ARM_LDMDB(uint32_t instruct);
    cycles ARM_LDMD(uint32_t baseReg, uint32_t registerList, bool pre, bool wback);
    cycles ARM_LDM_USER_REG(uint32_t instruct);
    cycles ARM_SWP(uint32_t instruct);
    cycles ARM_SWPB(uint32_t instruct);
    // Branch category.
    cycles ARM_B(uint32_t instruct);
    cycles ARM_BL(uint32_t instruct);
    cycles ARM_BLX_REG(uint32_t instruct);
    cycles ARM_BLX_IMM(uint32_t instruct);
    cycles ARM_BX(uint32_t instruct);
    // Misc category.
    cycles ARM_BKPT(uint32_t instruct);
    cycles ARM_SVC(uint32_t instruct);
    cycles ARM_MCRR(uint32_t instruct);
    cycles ARM_MCR(uint32_t instruct);
    cycles ARM_MRRC(uint32_t instruct);
    cycles ARM_MRC(uint32_t instruct);
    cycles ARM_STC(uint32_t instruct);
    cycles ARM_LDC(uint32_t instruct);
    cycles ARM_CDP(uint32_t instruct);
    // =============================================================================================
    // Thumb Instruction set.
    // =============================================================================================
    // Invalid instruction.
    cycles THUMB_UNDEFINED_INST(uint32_t instruct);
    // Decode helpers.
    cycles THUMB_shiftAddSubtractMoveCompareDecodeAndExecute(uint32_t instruct);
    cycles THUMB_dataProcessingDecodeAndExecute(uint32_t instruct);
    cycles THUMB_specialDataAndBranchDecodeAndExecute(uint32_t instruct);
    cycles THUMB_loadStoreDecodeAndExecute(uint32_t instruct);
    cycles THUMB_miscDecodeAndExecute(uint32_t instruct);
    cycles THUMB_condBranchAndSupervisorCallDecodeAndExecute(uint32_t instruct);
    // Data category.
    cycles THUMB_MOV_REG_HIGH(uint32_t instruct);
    cycles THUMB_MOV_REG_LOW(uint32_t instruct);
    cycles THUMB_MOV_IMM(uint32_t instruct);
    cycles THUMB_MOV(uint32_t desReg, uint32_t srcValue, bool carry, bool setFlags);
    cycles THUMB_AND_REG(uint32_t instruct);
    cycles THUMB_EOR_REG(uint32_t instruct);
    cycles THUMB_LSL_REG(uint32_t instruct);
    cycles THUMB_LSL_IMM(uint32_t instruct);
    cycles THUMB_LSL(uint32_t desReg, uint32_t operand, uint8_t amount, bool carry);
    cycles THUMB_LSR_REG(uint32_t instruct);
    cycles THUMB_LSR_IMM(uint32_t instruct);
    cycles THUMB_LSR(uint32_t desReg, uint32_t operand, uint8_t amount, bool carry);
    cycles THUMB_ASR_REG(uint32_t instruct);
    cycles THUMB_ASR_IMM(uint32_t instruct);
    cycles THUMB_ASR(uint32_t desReg, uint32_t operand, uint8_t amount, bool carry);
    cycles THUMB_ADD_REG(uint32_t instruct);
    cycles THUMB_ADD_REG_HIGH(uint32_t instruct);
    cycles THUMB_ADD_REG_LOW(uint32_t instruct);
    cycles THUMB_ADD_IMM3(uint32_t instruct);
    cycles THUMB_ADD_IMM(uint32_t instruct);
    cycles THUMB_ADD_SP_IMM7(uint32_t instruct);
    cycles THUMB_ADD_SP_IMM8(uint32_t instruct);
    cycles THUMB_ADD(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles THUMB_SBC_REG(uint32_t instruct);
    cycles THUMB_ADC_REG(uint32_t instruct);
    cycles THUMB_SUB_IMM(uint32_t instruct);
    cycles THUMB_SUB_REG(uint32_t instruct);
    cycles THUMB_SUB_IMM3(uint32_t instruct);
    cycles THUMB_SUB_SP_IMM7(uint32_t instruct);
    cycles THUMB_SUB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags);
    cycles THUMB_ROR_REG(uint32_t instruct);
    cycles THUMB_TST_REG(uint32_t instruct);
    cycles THUMB_RSB_REG(uint32_t instruct);
    cycles THUMB_CMP_REG(uint32_t instruct);
    cycles THUMB_CMP_REG_HIGH(uint32_t instruct);
    cycles THUMB_CMP_IMM(uint32_t instruct);
    cycles THUMB_CMP(uint32_t opp1, uint32_t opp2);
    cycles THUMB_CMN_REG(uint32_t instruct);
    cycles THUMB_ORR_REG(uint32_t instruct);
    cycles THUMB_MUL(uint32_t instruct);
    cycles THUMB_BIC_REG(uint32_t instruct);
    cycles THUMB_MVN_REG(uint32_t instruct);
    cycles THUMB_ADR(uint32_t instruct);
    // Load and Store category.
    cycles THUMB_STR(uint32_t srcReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_STRH(uint32_t srcReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_STRB(uint32_t srcReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_LDR(uint32_t desReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_LDRH(uint32_t desReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_LDRB(uint32_t desReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_LDRSH(uint32_t desReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_LDRSB(uint32_t desReg, uint32_t baseReg, uint32_t offset);
    cycles THUMB_STM(uint32_t instruct);
    cycles THUMB_LDM(uint32_t instruct);
    cycles THUMB_LDR_LIT(uint32_t instruct);
    cycles THUMB_PUSH(uint32_t instruct);
    cycles THUMB_POP(uint32_t instruct);
    // Branch category
    cycles THUMB_B(uint32_t instruct);
    cycles THUMB_B_COND(uint32_t instruct);
    cycles THUMB_BX(uint32_t instruct);
    cycles THUMB_BLX(uint32_t instruct);
    // Misc category.
    cycles THUMB_BKPT(uint32_t instruct);
    cycles THUMB_SVC(uint32_t instruct);
};

/**
 * @brief CPU model for the ARM7TDMI. The ARM7TDMI is mainly responsible for handling NDS I/O and
 * is the primary processor of the GBA. CPU targets the ARMv4 instruction set.
 */
class ARM7TDMI : public ARM {
private:
public:
    ARM7TDMI();
    ~ARM7TDMI();

    // Function overrides.
    cycles cycle() override;
    std::string getCPUName() override { return "ARM7TDMI"; }
    busPayload readBus(uint32_t address, uint32_t size = 32, bool codeRead = false) override;
    busPayload writeBus(uint32_t address, uint32_t data, uint32_t size = 32) override;
};

/**
 * @brief CPU model for the ARM946E-S. The ARM946E-S is NDS primary processor. CPU targets the
 * ARMv5TE instuction set.
 */
class ARM946ES : public ARM {
private:
public:
    ARM946ES();
    ~ARM946ES();

    // Function overrides.
    cycles cycle() override;
    std::string getCPUName() override { return "ARM946ES"; }
    busPayload readBus(uint32_t address, uint32_t size = 32, bool codeRead = false) override;
    busPayload writeBus(uint32_t address, uint32_t data, uint32_t size = 32) override;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
