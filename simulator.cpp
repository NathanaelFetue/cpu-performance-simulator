#include "simulator.hpp"
#include <iostream>
#include <iomanip>

Simulator::Simulator(double clock_frequency_hz)
    : clock_frequency_hz_(clock_frequency_hz) {}

void Simulator::set_clock_frequency(double clock_frequency_hz) {
    clock_frequency_hz_ = clock_frequency_hz;
}

SimulationResult Simulator::run(const std::vector<Instruction>& program) const {
    SimulationResult result{};
    result.instruction_count = program.size();

    uint64_t total_cycles = 0;
    for (const auto& instr : program) {
        total_cycles += cycle_cost(instr.type);
    }
    result.total_cycles = total_cycles;

    // Guard against empty programs (avoid division by zero).
    if (result.instruction_count == 0) {
        result.cpi = 0.0;
        result.ipc = 0.0;
        result.execution_time_ns = 0.0;
        return result;
    }

    result.cpi = static_cast<double>(total_cycles) /
                 static_cast<double>(result.instruction_count);
    result.ipc = 1.0 / result.cpi;

    // execution_time = total_cycles / clock_frequency
    // clock_frequency_hz_ is in Hz (cycles/second), so this gives
    // seconds; we convert to nanoseconds for readability.
    double execution_time_s = static_cast<double>(total_cycles) / clock_frequency_hz_;
    result.execution_time_ns = execution_time_s * 1e9;

    return result;
}

void SimulationResult::print() const {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Instructions:     " << instruction_count << "\n";
    std::cout << "Total cycles:     " << total_cycles << "\n";
    std::cout << "CPI:              " << cpi << "\n";
    std::cout << "IPC:              " << ipc << "\n";
    std::cout << "Execution time:   " << execution_time_ns << " ns\n";
}
