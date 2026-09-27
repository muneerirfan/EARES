#include "eares.h"

int checkpointCount(double executionTime) {
    // Eq. (10): ceil((Ku/Oc) * ET) - 1.
    return max(0, static_cast<int>(ceil((static_cast<double>(KU) / CHECKPOINT_OVERHEAD) * executionTime)) - 1);
}

double checkpointInterval(double executionTime) {
    // Eq. (11): ET * (N + 1)^(-1).
    int n = checkpointCount(executionTime);
    return executionTime / static_cast<double>(n + 1);
}

double bestExecutionTime(double executionTime) {
    // Eq. (12): ET + N * Oc.
    int n = checkpointCount(executionTime);
    return executionTime + static_cast<double>(n) * CHECKPOINT_OVERHEAD;
}

double worstExecutionTime(double executionTime) {
    // Eq. (13): ETbest + Ku*L + 2*Ku*Oc.
    return bestExecutionTime(executionTime)
         + static_cast<double>(KU) * checkpointInterval(executionTime)
         + 2.0 * static_cast<double>(KU) * CHECKPOINT_OVERHEAD;
}

double executionTime(int taskId, int vmId, double frequency) {
    // Eq. (4). The example supplies WCET at fmax rather than Ii and cvmk,
    // so WCET(fmax) / f is the same normalized-frequency calculation.
    return tasks[taskId].wcetAtFmax[vmId] / frequency;
}

void initializeTimingValues() {
    for (auto& task : tasks) {
        task.avgET = accumulate(task.wcetAtFmax.begin(), task.wcetAtFmax.end(), 0.0)
                   / static_cast<double>(VM_COUNT);

        // Algorithm 1 stores the largest worst execution time among VMs.
        // Since the motivational example has no separate f_h in this line,
        // use each VM's lowest allowed frequency as the worst case.
        task.etWorstMax = 0.0;
        for (int k = 0; k < VM_COUNT; ++k) {
            double et = executionTime(task.id, k, vms[k].fLow);
            task.etWorstMax = max(task.etWorstMax, worstExecutionTime(et));
        }
    }
}

static vector<int> topologicalOrder() {
    vector<int> indegree(TASK_COUNT, 0);
    for (const auto& edge : edges) {
        ++indegree[edge.to];
    }

    vector<int> order;
    vector<int> ready;
    for (int i = 0; i < TASK_COUNT; ++i) {
        if (indegree[i] == 0) {
            ready.push_back(i);
        }
    }

    while (!ready.empty()) {
        int u = ready.back();
        ready.pop_back();
        order.push_back(u);
        for (int v : tasks[u].successors) {
            if (--indegree[v] == 0) {
                ready.push_back(v);
            }
        }
    }
    return order;
}

void partitionDeadlines() {
    // Algorithm 2, lines 1-3: initial EST/LFT values.
    vector<int> order = topologicalOrder();
    for (int id : order) {
        if (tasks[id].predecessors.empty()) {
            tasks[id].est = 0.0;
        } else {
            double value = 0.0;
            for (int pred : tasks[id].predecessors) {
                value = max(value,
                    tasks[pred].est + tasks[id].avgET + communicationCost(pred, id));
            }
            tasks[id].est = value;
        }

        // The paper prints LFT(ti)=D for an entry task and
        // EST(ti)+ETbest(ti) otherwise. In the motivational example
        // avgET is the task-level execution value used by the CP length.
        tasks[id].lft = tasks[id].predecessors.empty()
                      ? WORKFLOW_DEADLINE
                      : tasks[id].est + tasks[id].avgET;
    }

    // Lines 4-9: build CP by repeatedly taking the predecessor with
    // the largest LFT from the exit task.
    int exitTask = TASK_COUNT - 1;
    int current = exitTask;
    criticalPath.clear();
    criticalPath.push_back(current);

    while (!tasks[current].predecessors.empty()) {
        int bestPred = tasks[current].predecessors.front();
        for (int pred : tasks[current].predecessors) {
            if (tasks[pred].lft > tasks[bestPred].lft) {
                bestPred = pred;
            }
        }
        current = bestPred;
        criticalPath.push_back(current);
    }
    reverse(criticalPath.begin(), criticalPath.end());

    // Eq. (25).
    double cpLength = 0.0;
    double cpETSum = 0.0;
    for (size_t i = 0; i < criticalPath.size(); ++i) {
        int id = criticalPath[i];
        cpLength += tasks[id].avgET;
        cpETSum += tasks[id].avgET;
        if (i > 0) {
            cpLength += communicationCost(criticalPath[i - 1], id);
        }
    }

    int cpEntry = criticalPath.front();
    int cpExit = criticalPath.back();
    scaleFactor = ((tasks[cpExit].lft - tasks[cpEntry].est) - cpLength) / cpETSum;

    // Eq. (26): EET = ET + S(CP)*ET.
    for (int id : criticalPath) {
        tasks[id].eet = tasks[id].avgET + scaleFactor * tasks[id].avgET;
    }

    vector<bool> inCP(TASK_COUNT, false);
    for (int id : criticalPath) {
        inCP[id] = true;
    }

    // Lines 14-24. Recalculate CP EST using EET, then d(ti) by Eq. (27).
    for (int id : criticalPath) {
        if (tasks[id].predecessors.empty()) {
            tasks[id].est = 0.0;
        } else {
            double value = 0.0;
            for (int pred : tasks[id].predecessors) {
                double predTime = inCP[pred] ? tasks[pred].eet : tasks[pred].avgET;
                value = max(value, tasks[pred].est + predTime + communicationCost(pred, id));
            }
            tasks[id].est = value;
        }

        tasks[id].deadline = tasks[id].est + tasks[id].eet;
        tasks[id].lft = tasks[id].deadline;

        if (id == exitTask) {
            tasks[id].eet = WORKFLOW_DEADLINE - tasks[id].est;
            tasks[id].deadline = WORKFLOW_DEADLINE;
            tasks[id].lft = tasks[id].deadline;
        }
    }

    // Lines 25-29 and Eqs. (28)-(29): solve unmarked tasks from bottom up.
    vector<int> reverseOrder = order;
    reverse(reverseOrder.begin(), reverseOrder.end());

    for (int id : reverseOrder) {
        if (inCP[id]) {
            continue;
        }

        double lftValue = WORKFLOW_DEADLINE;
        if (!tasks[id].successors.empty()) {
            for (int succ : tasks[id].successors) {
                double candidate = tasks[succ].lft
                                 - tasks[succ].eet
                                 - communicationCost(id, succ);
                lftValue = min(lftValue, candidate);
            }
        }

        tasks[id].lft = lftValue;
        tasks[id].deadline = lftValue;
        tasks[id].eet = tasks[id].lft - tasks[id].est;
    }
}
