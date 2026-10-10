# EARES workflow scheduling example

This single-file C++17 implementation accompanies the motivational example in Longxin Zhang, Minghui Ai, Ke Liu, Jianguo Chen, and Kenli Li, "Reliability Enhancement Strategies for Workflow Scheduling Under Energy Consumption Constraints in Clouds," *IEEE Transactions on Sustainable Computing*, vol. 9, no. 2, pp. 155-169, 2024, doi: [10.1109/TSUSC.2023.3314759](https://doi.org/10.1109/TSUSC.2023.3314759). It uses Fig. 1 and Tables II-IV as its example inputs and reference outputs.

## Build and run

With MinGW g++ in PowerShell:

```powershell
make
.\eares_example.exe
```

This uses MinGW/MSYS GNU Make and g++. Or compile directly:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o eares_example.exe
```

## Output modes

The default run calculates deadlines, energy budgets, and a schedule using the paper's equations. Use `--deadlines table --energy table` to supply the per-task limits printed in Table IV while computing the VM/frequency choices. Use `--reference` to print Table IV and its summary verbatim; reference mode is a transcription for comparison, not a computed result.

The computed schedule and summary can differ from Table IV. `--comm` includes inter-VM communication time in the scheduler's finish-time check.

## Reproducibility notes

- The task execution times, graph, VM power parameters, deadline, and energy goal are transcribed in `example_data.cpp`. Communication delay is data volume divided by the configured bandwidth.
- The paper does not publish the example's fault-rate parameters `lambda_fmax` or sensitivity `d`. Table V gives only experiment ranges for `lambda_fmax` and no value for `d`; computed reliabilities therefore depend on the explicit illustrative assumptions in `example_data.cpp`.
- Table III specifies VM2 `Cef = 0.80`, while its Table IV energy values match `Cef = 0.70`. The implementation uses the Table III value in computed mode and preserves Table IV values only in reference mode.
- Pass `--deadlines table --energy table` to use Table IV's task deadlines and energy allowances as constraints. The checkpoint equations affect execution time and reliability, so computed results need not duplicate every Table IV entry. The rounded task energies in Table IV sum to 72.9111, while the paper reports 72.9112.
- The exact makespan `82.11` and workflow reliability `0.9751` are paper-reported values in reference mode. Computed mode reports metrics from the implemented equations and the explicit input assumptions.

For a new workflow or an independent evaluation, replace the example data and provide independently justified reliability parameters. The motivational example cannot be an exact independent reproduction without the missing fault parameters and a resolution of the VM2 `Cef` discrepancy.
