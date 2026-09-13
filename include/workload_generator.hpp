#pragma once

#include "instruction.hpp"
#include <vector>
#include <random>

// Generates a synthetic program with a given instruction mix.
// This is intentionally simple (random draw per percentage) --
// good enough to compare "what if a program is ALU-heavy vs
// LOAD-heavy", which is the point of the J1-5 experiments.
struct InstructionMix {
    double alu_pct;
    double mul_pct;
    double load_pct;
    double store_pct;
    double branch_pct;
    // Caller is responsible for making these sum to 1.0 (not enforced
    // here on purpose -- keep the helper dumb and the caller explicit).
};

inline std::vector<Instruction> generate_program(const InstructionMix& mix,
                                                   size_t instruction_count,
                                                   unsigned seed = 42) {
    std::vector<Instruction> program;
    program.reserve(instruction_count);

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (size_t i = 0; i < instruction_count; ++i) {
        double r = dist(rng);
        InstructionType type;
        if (r < mix.alu_pct) {
            type = InstructionType::ALU;
        } else if (r < mix.alu_pct + mix.mul_pct) {
            type = InstructionType::MUL;
        } else if (r < mix.alu_pct + mix.mul_pct + mix.load_pct) {
            type = InstructionType::LOAD;
        } else if (r < mix.alu_pct + mix.mul_pct + mix.load_pct + mix.store_pct) {
            type = InstructionType::STORE;
        } else {
            type = InstructionType::BRANCH;
        }
        program.push_back({type});
    }
    return program;
}
