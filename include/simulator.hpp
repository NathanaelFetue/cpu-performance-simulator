#pragma once

#include "instruction.hpp"
#include <vector>
#include <cstdint>

// Results of running a program through the simulator.
// Keeping this as a plain struct (not just printed values) makes it
// trivial to write real tests and to feed results into the
// frequency-sweep / mix-comparison experiments later.
struct SimulationResult {
    uint64_t instruction_count;
    uint64_t total_cycles;
    double cpi;              // cycles per instruction
    double ipc;              // instructions per cycle (= 1 / cpi)
    double execution_time_ns; // at a given clock frequency

    void print() const;
};

class Simulator {
public:
    // clock_frequency_hz: e.g. 1e9 for 1 GHz
    explicit Simulator(double clock_frequency_hz);

    // Runs the full instruction stream through the non-pipelined
    // cycle model and returns aggregate results.
    SimulationResult run(const std::vector<Instruction>& program) const;

    // Lets experiments change frequency without rebuilding the whole
    // simulator (used in the frequency-sweep experiment).
    void set_clock_frequency(double clock_frequency_hz);

private:
    double clock_frequency_hz_;
};
