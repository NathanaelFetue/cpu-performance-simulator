#pragma once

#include <string>
#include <cstdint>

// Non-pipelined instruction-latency model (J1-J5 scope).
// Each instruction type has a fixed cycle cost. This is deliberately
// simplified: no pipeline overlap, no hazards, no branch prediction.
// Those come in J6-12. Here we only answer: "how many cycles total?"

enum class InstructionType {
    ALU,     // ADD, SUB, AND, OR, etc.
    MUL,     // multiply -- costs more than a simple ALU op
    LOAD,    // memory read
    STORE,   // memory write
    BRANCH   // conditional/unconditional jump
};

struct Instruction {
    InstructionType type;
};

// Cycle cost table for the non-pipelined model.
// These are illustrative values, not tied to a specific real ISA --
// the point is to reason about the model, not memorize a datasheet.
inline uint64_t cycle_cost(InstructionType type) {
    switch (type) {
        case InstructionType::ALU:    return 1;
        case InstructionType::MUL:    return 3;
        case InstructionType::LOAD:   return 4;
        case InstructionType::STORE:  return 4;
        case InstructionType::BRANCH: return 2;
    }
    return 0; // unreachable, keeps compiler happy
}

inline std::string to_string(InstructionType type) {
    switch (type) {
        case InstructionType::ALU:    return "ALU";
        case InstructionType::MUL:    return "MUL";
        case InstructionType::LOAD:   return "LOAD";
        case InstructionType::STORE:  return "STORE";
        case InstructionType::BRANCH: return "BRANCH";
    }
    return "UNKNOWN";
}
