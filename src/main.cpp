#include "simulator.hpp"
#include "workload_generator.hpp"
#include <iostream>

void experiment_instruction_mix() {
    std::cout << "=== Experiment 1: Instruction Mix Comparison ===\n\n";

    const size_t N = 100000;

    // Program A: ALU-heavy (e.g. arithmetic-bound workload)
    InstructionMix mix_a{0.70, 0.0, 0.20, 0.0, 0.10};
    // Program B: LOAD-heavy (e.g. memory-bound workload)
    InstructionMix mix_b{0.30, 0.0, 0.50, 0.0, 0.20};

    Simulator sim(1e9); // 1 GHz, fixed for this experiment

    auto program_a = generate_program(mix_a, N);
    auto program_b = generate_program(mix_b, N);

    std::cout << "-- Program A (70% ALU, 20% LOAD, 10% BRANCH) --\n";
    sim.run(program_a).print();

    std::cout << "\n-- Program B (30% ALU, 50% LOAD, 20% BRANCH) --\n";
    sim.run(program_b).print();

    std::cout << "\nTakeaway: same instruction count, same clock speed --\n"
              << "the memory-bound program (B) takes noticeably longer\n"
              << "because LOAD costs more cycles than ALU in this model.\n\n";
}

void experiment_frequency_sweep() {
    std::cout << "=== Experiment 2: Frequency Sweep ===\n\n";

    const size_t N = 100000;
    InstructionMix mix{0.60, 0.05, 0.20, 0.10, 0.05};
    auto program = generate_program(mix, N);

    std::vector<double> frequencies_ghz = {1.0, 2.0, 3.0, 4.0};

    for (double freq_ghz : frequencies_ghz) {
        Simulator sim(freq_ghz * 1e9);
        auto result = sim.run(program);
        std::cout << freq_ghz << " GHz -> execution time: "
                  << result.execution_time_ns << " ns "
                  << "(CPI/IPC unchanged: " << result.cpi << " / " << result.ipc << ")\n";
    }

    std::cout << "\nTakeaway: CPI and IPC stay identical across frequencies --\n"
              << "they depend only on the instruction mix and cycle model,\n"
              << "not on clock speed. Execution time scales down linearly\n"
              << "with frequency because total_cycles is fixed.\n"
              << "This is exactly why, later on, REDUCING cycles (better\n"
              << "pipeline, better cache behavior) matters as much as --\n"
              << "often more than -- raising the clock frequency.\n\n";
}

int main() {
    experiment_instruction_mix();
    experiment_frequency_sweep();
    return 0;
}
