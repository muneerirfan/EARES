#include "eares.h"

void computeURanksAndTaskList() {
    // Eq. (19): URank(ti) = max_succ(URank(tj) + CCij)
    //            + (1/m) * sum_k ET(ti, vm_k^fmax).
    for (int i = TASK_COUNT - 1; i >= 0; --i) {
        double averageExecution = 0.0;
        for (int k = 0; k < VM_COUNT; ++k) {
            averageExecution += tasks[i].wcetAtFmax[k];
        }
        averageExecution /= static_cast<double>(VM_COUNT);
        tasks[i].avgET = averageExecution;

        double successorTerm = 0.0;
        for (int successor : tasks[i].successors) {
            successorTerm = max(successorTerm,
                tasks[successor].uRank + communicationCost(i, successor));
        }
        tasks[i].uRank = successorTerm + averageExecution;
    }

    taskList.resize(TASK_COUNT);
    iota(taskList.begin(), taskList.end(), 0);
    stable_sort(taskList.begin(), taskList.end(), [](int a, int b) {
        if (fabs(tasks[a].uRank - tasks[b].uRank) > 1e-9) {
            return tasks[a].uRank > tasks[b].uRank;
        }
        return a < b;
    });
}
