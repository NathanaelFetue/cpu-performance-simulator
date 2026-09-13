// Lightweight test harness -- no external framework (Catch2/GTest)
// needed for a project this small. Each test is a function returning
// bool; main() aggregates and reports pass/fail with a nonzero exit
// code on failure, so it works cleanly with `ctest` and CI.

#include "simulator.hpp"
#include <iostream>
#include <cmath>
#include <vector>

namespace {

bool approx_equal(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

bool test_empty_program() {
    Simulator sim(1e9);
    auto result = sim.run({});
    return result.instruction_count == 0 &&
           result.total_cycles == 0 &&
           approx_equal(result.cpi, 0.0) &&
           approx_equal(result.execution_time_ns, 0.0);
}

bool test_single_alu_instruction() {
    Simulator sim(1e9); // 1 GHz -> 1 cycle = 1 ns
    std::vector<Instruction> program = {{InstructionType::ALU}};
    auto result = sim.run(program);
    return result.instruction_count == 1 &&
           result.total_cycles == 1 &&
           approx_equal(result.cpi, 1.0) &&
           approx_equal(result.ipc, 1.0) &&
           approx_equal(result.execution_time_ns, 1.0);
}

bool test_cpi_ipc_are_reciprocal() {
    Simulator sim(1e9);
    std::vector<Instruction> program = {
        {InstructionType::LOAD}, {InstructionType::LOAD}, {InstructionType::ALU}
    };
    auto result = sim.run(program);
    // CPI * IPC should always equal 1 by construction.
    return approx_equal(result.cpi * result.ipc, 1.0);
}

bool test_known_cycle_total() {
    // 2 ALU (1 cycle each) + 1 LOAD (4 cycles) + 1 BRANCH (2 cycles)
    // = 2 + 4 + 2 = 8 cycles over 4 instructions -> CPI = 2.0
    Simulator sim(1e9);
    std::vector<Instruction> program = {
        {InstructionType::ALU},
        {InstructionType::ALU},
        {InstructionType::LOAD},
        {InstructionType::BRANCH}
    };
    auto result = sim.run(program);
    return result.total_cycles == 8 &&
           approx_equal(result.cpi, 2.0) &&
           approx_equal(result.ipc, 0.5);
}

bool test_frequency_scaling_halves_time() {
    std::vector<Instruction> program = {
        {InstructionType::LOAD}, {InstructionType::LOAD}
    };
    Simulator sim_1ghz(1e9);
    Simulator sim_2ghz(2e9);

    auto r1 = sim_1ghz.run(program);
    auto r2 = sim_2ghz.run(program);

    // Same cycles, double frequency -> half the execution time.
    return approx_equal(r1.total_cycles, r2.total_cycles) &&
           approx_equal(r1.execution_time_ns, r2.execution_time_ns * 2.0);
}

bool test_set_clock_frequency_updates_result() {
    Simulator sim(1e9);
    std::vector<Instruction> program = {{InstructionType::ALU}};
    auto before = sim.run(program);

    sim.set_clock_frequency(2e9);
    auto after = sim.run(program);

    return approx_equal(before.execution_time_ns, after.execution_time_ns * 2.0);
}

} // namespace

int main() {
    struct Test { const char* name; bool (*fn)(); };
    std::vector<Test> tests = {
        {"empty_program", test_empty_program},
        {"single_alu_instruction", test_single_alu_instruction},
        {"cpi_ipc_are_reciprocal", test_cpi_ipc_are_reciprocal},
        {"known_cycle_total", test_known_cycle_total},
        {"frequency_scaling_halves_time", test_frequency_scaling_halves_time},
        {"set_clock_frequency_updates_result", test_set_clock_frequency_updates_result},
    };

    int failures = 0;
    for (const auto& t : tests) {
        bool ok = t.fn();
        std::cout << (ok ? "[PASS] " : "[FAIL] ") << t.name << "\n";
        if (!ok) failures++;
    }

    std::cout << "\n" << (tests.size() - failures) << "/" << tests.size()
              << " tests passed.\n";
    return failures == 0 ? 0 : 1;
}
