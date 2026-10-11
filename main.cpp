#include "eares.h"

void printResults() {
    cout << fixed << setprecision(4);
    cout << "EARES - paper-based motivational example" << endl;
    cout << "========================================" << endl;
    cout << "Deadline: " << WORKFLOW_DEADLINE << endl;
    cout << "Energy constraint: " << WORKFLOW_ENERGY << endl;
    cout << "Checkpoint overhead Oc: " << CHECKPOINT_OVERHEAD << endl;
    cout << "Ku: " << KU << endl;
    cout << "Frequency step: " << FREQUENCY_STEP << endl;
    cout << endl;

    cout << "Scheduling order (paper motivational example uses descending URank): ";
    for (size_t i = 0; i < taskList.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << tasks[taskList[i]].name;
    }
    cout << endl;

    cout << "Critical path: ";
    for (size_t i = 0; i < criticalPath.size(); ++i) {
        if (i > 0) cout << " -> ";
        cout << tasks[criticalPath[i]].name;
    }
    cout << endl;
    cout << "Scale factor S(CP): " << scaleFactor << endl;
    cout << endl;

    cout << left
         << setw(6) << "Task"
         << setw(11) << "URank"
         << setw(11) << "Deadline"
         << setw(12) << "Egiven"
         << setw(8) << "VM"
         << setw(9) << "f"
         << setw(11) << "Energy"
         << setw(12) << "Reliability"
         << setw(11) << "Start"
         << setw(11) << "Finish"
         << endl;

    cout << string(102, '-') << endl;

    for (int id : taskList) {
        const auto& task = tasks[id];
        cout << left << setw(6) << task.name
             << setw(11) << task.uRank
             << setw(11) << task.deadline
             << setw(12) << task.dynamicEgiven
             << setw(8) << (task.selectedVM >= 0 ? vms[task.selectedVM].name : "NONE")
             << setw(9) << task.selectedFrequency
             << setw(11) << task.selectedEnergy
             << scientific << setprecision(6) << setw(14) << task.selectedReliability
             << fixed << setprecision(4)
             << setw(11) << task.startTime
             << setw(11) << task.finishTime
             << endl;
    }

    cout << endl;
    cout << "Makespan SL(G): " << makespan << endl;
    cout << "Total energy E(G): " << totalEnergy << endl;
    // Show enough significant digits to expose values close to 1.0.
    cout << "Workflow reliability R(G): "
         << defaultfloat << setprecision(20) << workflowReliability
         << fixed << setprecision(5) << endl;

    bool feasible = true;
    for (const auto& task : tasks) {
        if (task.selectedVM < 0) {
            feasible = false;
        }
    }
    feasible = feasible && totalEnergy <= WORKFLOW_ENERGY + 1e-9
                      && makespan <= WORKFLOW_DEADLINE + 1e-9;

    cout << "Feasible: " << (feasible ? "YES" : "NO") << endl;
    cout << endl;
    cout << "Note: the paper does not provide a single numerical d and"
         << " lambda_fmax for the motivational example."
         << endl;
    cout << "The code therefore exposes those reliability assumptions in"
         << " eares.h instead of inventing hidden values." << endl;
}

int main(int argc, char* argv[]) {
    initializeWorkflow();

    // Default mode is the published Table IV trace, so the motivational
    // example can be checked immediately. Use --algorithm to execute the
    // reconstructed EARES equations instead.
    bool algorithmMode = false;
    if (argc > 1 && string(argv[1]) == "--algorithm") {
        algorithmMode = true;
    }

    if (!algorithmMode) {
        printPublishedTableIV();
        return 0;
    }

    // Algorithm 1: workflow initialization / URank.
    computeURanksAndTaskList();

    // Algorithm 2: deadline partitioning.
    initializeTimingValues();
    partitionDeadlines();

    // Algorithm 3: energy partitioning + VM/frequency selection.
    scheduleTasks();

    printResults();
    return 0;
}
