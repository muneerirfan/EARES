# EARES workflow scheduling example

This repository contains a C++17 implementation of the motivational workflow example from Longxin Zhang et al., *Reliability Enhancement Strategies for Workflow Scheduling Under Energy Consumption Constraints in Clouds*, IEEE Transactions on Sustainable Computing (2024).

It provides two run modes:

- **Reference mode** prints the values transcribed from the paper's Table IV. It does not run the scheduling algorithm.
- **Algorithm mode** calculates ranks, deadlines, energy allocations, VM/frequency selections, and reliability from the workflow and VM data in the source.

## Requirements

A C++17 compiler is required. The program uses only the C++ standard library. The commands below use `g++`.

## Build

From the repository directory:

```bash
g++ -std=c++17 -O2 main.cpp graph.cpp rank.cpp timing.cpp energy.cpp reliability.cpp scheduler.cpp paper_example.cpp -o eares
```

PowerShell on Windows:

```powershell
g++ -std=c++17 -O2 main.cpp graph.cpp rank.cpp timing.cpp energy.cpp reliability.cpp scheduler.cpp paper_example.cpp -o eares.exe
```

## Run

Run the published Table IV reference trace:

```bash
./eares
```

On PowerShell:

```powershell
.\eares.exe
```

Run the reconstructed calculation:

```bash
./eares --algorithm
```

```powershell
.\eares.exe --algorithm
```

Algorithm mode prints task reliability in scientific notation and workflow reliability with high precision, so values close to 1 are visible instead of being rounded to `1.0000`. A representative workflow reliability for the current default assumptions is approximately `0.99999938938517496378`; results depend on the reliability parameters configured in `eares.h`.

## Source layout

- `main.cpp` selects the run mode and prints calculated results.
- `graph.cpp` defines the 10-task DAG, task WCET values, and VM parameters.
- `rank.cpp` calculates upward ranks and the scheduling order.
- `timing.cpp` estimates task timing and partitions deadlines.
- `energy.cpp` calculates task energy bounds and preallocations.
- `scheduler.cpp` searches VMs and frequencies for feasible task assignments.
- `reliability.cpp` implements the checkpoint and Poisson reliability calculations.
- `paper_example.cpp` holds the published Table IV values used by reference mode only.
- `eares.h` contains shared data structures and configurable model constants.

The algorithm mode models communication, normalized execution time, power and energy, checkpoint overhead, Poisson reliability, URank, deadline partitioning, energy preallocation, dynamic task energy budgets, and VM/frequency selection.

## Configuration and interpretation

The example uses the DAG and WCET values associated with Fig. 1 and Table II and the VM parameters associated with Table III. The workflow deadline, energy budget, checkpoint overhead, `Ku`, and frequency search step are constants in `eares.h`.

The paper does not give a single explicit pair of VM-specific `lambda_fmax` values and fault-rate sensitivity for this motivational example. The implementation exposes defaults (`DEFAULT_LAMBDA_FMAX` and `DEFAULT_FAULT_SENSITIVITY`) in `eares.h`; these are assumptions, not values recovered from Table IV. Changing them can change task reliability and the scheduler's selected VM/frequency.

The printed Table IV trace is kept separate because the paper's printed inputs and equations do not reproduce every Table IV value. The paper also reports rounded task reliabilities whose direct product does not equal the printed workflow reliability. Deadline partitioning follows the exit task's workflow deadline in Eq. (25), consistent with the reported scale-factor range; the paper's notation is ambiguous on this point.

Treat algorithm-mode output as a reconstruction under the documented assumptions, not as an exact reproduction of the published trace.
