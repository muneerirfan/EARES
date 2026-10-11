# EARES: Energy-Aware and Reliability-Aware Workflow Scheduling

A C++ implementation of an Energy-Aware and Reliability-Aware Scheduling (EARES) approach for workflow execution in a cloud computing environment, based on a research paper.

The project models workflow tasks, dependencies, virtual machines, execution times, energy consumption, reliability, and deadline and energy constraints. It includes an algorithm mode that calculates scheduling results and a reference mode that displays the reported results from the paper's motivational example.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technologies](#technologies)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Build Instructions](#build-instructions)
- [Running the Program](#running-the-program)
- [Example Results](#example-results)
- [Configuration and Assumptions](#configuration-and-assumptions)
- [Limitations](#limitations)
- [Future Improvements](#future-improvements)
- [Reference](#reference)

## Overview

Workflow scheduling is an important problem in cloud computing. A workflow consists of tasks connected by dependencies, and each task must be assigned to a suitable virtual machine while considering execution time, energy consumption, and reliability.

This project implements a C++ model of the EARES scheduling approach using a workflow example based on the referenced research paper.

The implementation evaluates scheduling decisions under deadline and energy constraints and reports metrics such as:

- Workflow makespan
- Total energy consumption
- Workflow reliability
- Schedule feasibility

The project is intended for academic study, algorithm experimentation, and comparison with the motivational example presented in the research paper.

## Features

- Workflow representation using a directed acyclic graph (DAG).
- Task dependencies and scheduling order.
- Virtual machine selection and processor-frequency configuration.
- Execution-time and energy-consumption calculations.
- Reliability estimation using configurable assumptions.
- Deadline and energy constraint checks.
- Computed algorithm mode.
- Reference mode for displaying the paper's reported example results.
- Modular C++ source code separated by functionality.

## Technologies

- **Language:** C++
- **Compiler:** GCC / g++
- **Standard:** C++17
- **Development environment:** Visual Studio Code
- **Test environment:** Windows with MSYS2 and MinGW-w64 GCC

## Project Structure

```text
eares-paper-corrected/
├── main.cpp           # Program entry point and execution modes
├── eares.h            # Shared declarations and configuration
├── graph.cpp          # Workflow graph, tasks, VMs, and constraints
├── rank.cpp           # Task ranking and scheduling order
├── timing.cpp         # Execution-time and checkpoint calculations
├── energy.cpp         # Energy consumption calculations
├── reliability.cpp    # Reliability estimation
├── scheduler.cpp      # Scheduling algorithm
├── paper_example.cpp  # Motivational example and reference results
├── README.md          # Project documentation
└── .gitignore         # Files excluded from Git
```

## Requirements

Before building the project, ensure that you have:

1. Git installed.
2. GCC/g++ with C++17 support.
3. Visual Studio Code or another C++ development environment.
4. A terminal configured to use your compiler.

To verify the compiler installation, run:

```powershell
g++ --version
```

## Build Instructions

### 1. Clone the repository

Replace `YOUR-USERNAME` with your GitHub username.

```powershell
git clone https://github.com/YOUR-USERNAME/eares-paper-corrected.git
cd eares-paper-corrected
```

If you already have the project locally, open its folder in VS Code instead.

### 2. Compile the source files

Run the following command from the project root:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp energy.cpp graph.cpp paper_example.cpp rank.cpp reliability.cpp scheduler.cpp timing.cpp -o program.exe
```

This compiles the source files and creates `program.exe`.

If compilation fails, check that all listed files exist in the project directory and that the compiler is available in your terminal.

## Running the Program

### Algorithm mode

Run:

```powershell
.\program.exe --algorithm
```

This mode executes the implemented scheduling algorithm and calculates scheduling metrics using the project's current implementation and configuration.

### Paper reference mode

Run:

```powershell
.\program.exe
```

This mode displays the reference results associated with the paper's motivational example.

**Important:** Reference mode displays stored reference values; it does not independently calculate or verify those values.

## Example Results

The following values were observed when running the algorithm mode with the current implementation:

| Metric | Computed result |
|---|---:|
| Workflow makespan | 77.7143 |
| Total energy consumption | 74.7135 |
| Workflow reliability | 1.0000 |
| Feasible under implemented checks | YES |

The paper's motivational example reports the following reference values:

| Metric | Paper-reported result |
|---|---:|
| Workflow makespan | 82.11 |
| Total energy consumption | 72.9112 |
| Workflow reliability | 0.9751 |

These two sets of values differ. The computed results should therefore be treated as **current implementation outputs, not as a validated reproduction of the paper's results**.

The reported feasibility result means that the implementation considers the schedule feasible according to its own checks. It does not, by itself, establish that every equation and constraint has been implemented exactly as described in the paper.

## Configuration and Assumptions

The motivational example uses the following parameters in the current implementation:

| Parameter | Value |
|---|---:|
| Workflow deadline | 110 |
| Energy constraint | 80 |
| Checkpoint overhead (\(O_c\)) | 0.1 |
| \(K_u\) | 2 |
| Frequency step | 0.01 |

The paper does not specify a single numerical value for every reliability parameter used in the motivational example. The implementation exposes reliability assumptions, including the maximum-frequency failure-rate parameter and fault sensitivity, in `eares.h`.

These assumptions should be documented and justified when comparing the computed reliability with the paper's reported value.

## Limitations

The current implementation is a work in progress. The following points require further verification:

- **Numerical validation:** Computed makespan, energy, and reliability do not currently match the paper's reported values.
- **Checkpoint timing:** Checkpoint-related execution-time calculations must be checked against the paper's equations and units.
- **Reliability parameters:** Assumed parameter values can significantly affect the reliability result.
- **Scheduling constraints:** Feasibility checks should be verified against the complete timing and energy models.
- **Reference consistency:** Stored reference values are useful for comparison but do not validate the computed algorithm.

Consequently, the project should be described as a research-based implementation under validation rather than a fully verified reproduction of the published algorithm.

## Future Improvements

- Validate each implemented equation against the research paper.
- Verify checkpoint overhead and checkpoint-adjusted execution times.
- Review VM selection and frequency-scaling decisions.
- Calibrate reliability parameters using explicitly justified assumptions.
- Add automated tests for timing, energy, reliability, and scheduling constraints.
- Compare computed outputs with the paper's reported example.
- Improve diagnostic messages for infeasible schedules.

## Reference

The research paper used as the basis for this implementation:

*Reliability, Deadline, and Energy-Aware Workflow Scheduling in Cloud Computing.*

Refer to the original publication for the formal mathematical model, algorithm descriptions, and experimental results.

## Author

Developed as an academic C++ project to study energy-aware and reliability-aware workflow scheduling in cloud computing.

Contributions, bug reports, and suggestions for improvement are welcome.