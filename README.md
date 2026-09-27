# EARES paper-based C++ implementation — corrected source transcription

This is a plain C++17 implementation of the EARES motivational example from:

Longxin Zhang et al., *Reliability Enhancement Strategies for Workflow Scheduling Under Energy Consumption Constraints in Clouds*, IEEE Transactions on Sustainable Computing, 2024.

## Files

```text
eares-paper-exact/
├── main.cpp
├── eares.h
├── graph.cpp
├── rank.cpp
├── timing.cpp
├── energy.cpp
├── reliability.cpp
├── scheduler.cpp
└── README.md
```

There is **no CMake** and no `.hpp` file. The single `.h` file only contains shared declarations and constants so the `.cpp` files can be compiled directly with `g++`.

## Build on Windows / VS Code

Open PowerShell in this folder and run:

```powershell
g++ -std=c++17 main.cpp graph.cpp rank.cpp timing.cpp energy.cpp reliability.cpp scheduler.cpp -o eares.exe
.\eares.exe
```

## Paper equations implemented

- Eq. (1)-(3): communication cost and average bandwidth.
- Eq. (4): execution time at normalized frequency.
- Eq. (7): task energy.
- Eq. (10)-(13): checkpoint count, interval, best execution time, worst execution time.
- Eq. (14)-(18): Poisson transient-fault reliability and workflow reliability.
- Eq. (19): URank.
- Eq. (23)-(29): deadline partitioning.
- Eq. (30)-(36): energy partitioning and dynamic `Egiven(ti)`.
- Algorithm 1: workflow initialization.
- Algorithm 2: deadline partitioning.
- Algorithm 3: energy partitioning and VM/frequency selection.

## Important source limitations / paper inconsistencies

The motivational example gives the VM power parameters, the WCET table, `D=110`, `Egoal=80`, `b=1 Gbps`, `Oc=0.1`, and the frequency precision, but it does not give one numerical value for the fault-rate sensitivity `d` or for each VM's `lambda_fmax` used for that example. The paper's later Table V gives ranges for experimental settings, not a single motivational-example value.

Therefore the code does **not** silently invent those values. `DEFAULT_LAMBDA_FMAX` and `DEFAULT_FAULT_SENSITIVITY` are explicit constants in `eares.h` and can be changed.

Also, the paper's motivational Table IV contains numerical values that are not all exactly reproducible from Tables II/III and the printed equations. The implementation follows the printed equations and the workflow data rather than hard-coding Table IV's final VM assignments.

The paper has an internal wording conflict: Definition 7 / Algorithm 1 say nondescending URank, while the motivational example explicitly schedules in descending URank order. This implementation follows the motivational example and therefore sorts URank in descending order.

Eq. (10) is implemented with the paper's printed ceiling operation: N = ceil((Ku/Oc) × ET) − 1.
