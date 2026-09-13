# cpu-performance-simulator

A small, deliberately simple simulator for reasoning about CPU performance
fundamentals: CPI, IPC, and execution time -- before introducing pipelining,
hazards, or caches (those come in later projects).

## Scope (by design)

This project answers **"how many cycles does a program take?"** using a
non-pipelined instruction-latency model. It does **not** simulate:
- Pipeline stages or hazards (RAW/WAR/WAW, structural, control)
- Branch prediction
- Cache behavior
- Real ISA opcodes or ELF parsing

Those are the subject of the next projects in the sequence:

```
CPU Performance Simulator  (this project)
        |  "how many cycles?"
        v
Pipeline Simulator
        |  "why these extra cycles?"
        v
Cache Simulator
        |  "why do memory accesses cost so much?"
        v
MESI + False Sharing
        |  "what happens with multiple cores?"
```

## Model

Each instruction type has a fixed cycle cost (non-pipelined model):

| Type   | Cycles |
|--------|--------|
| ALU    | 1      |
| MUL    | 3      |
| LOAD   | 4      |
| STORE  | 4      |
| BRANCH | 2      |

```
total_cycles = sum(cycle_cost(instr) for instr in program)
CPI = total_cycles / instruction_count
IPC = 1 / CPI
execution_time = total_cycles / clock_frequency
```

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Running

```bash
./cpu_sim        # runs the two experiments below
./test_simulator # runs the unit test suite
```

## Experiments

**1. Instruction mix comparison** -- an ALU-heavy program (70% ALU) vs. a
memory-bound one (50% LOAD), same instruction count, same clock speed.
Shows that the memory-bound program takes measurably longer because LOAD
costs 4x an ALU op in this model.

**2. Frequency sweep** -- the same program run at 1/2/3/4 GHz. Shows that
CPI and IPC are invariant to clock frequency (they depend only on the
instruction mix), while execution time scales down linearly. This is the
seed of a more advanced idea explored later: reducing the number of
cycles (better pipelining, better cache locality) is often more valuable
than simply raising the clock frequency.

## Tests

Six unit tests cover: empty program handling, a known single-instruction
case, the CPI/IPC reciprocal relationship, a hand-computed multi-instruction
cycle total, frequency-scaling correctness, and the mutable
`set_clock_frequency` API. No external test framework -- a small custom
harness keeps the project dependency-free and easy to read end to end.

## Project structure

```
include/
  instruction.hpp        -- instruction types and cycle-cost table
  simulator.hpp           -- Simulator class and SimulationResult
  workload_generator.hpp  -- synthetic program generation from a mix
src/
  simulator.cpp           -- core simulation logic
  main.cpp                -- runs the two experiments
tests/
  test_simulator.cpp      -- unit tests
CMakeLists.txt
```
