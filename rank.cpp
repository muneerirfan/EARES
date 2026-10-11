#include "eares.h"

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

void computeURanksAndTaskList() {
    // Eq. (19). Calculate URank from exit toward entry.
    vector<int> order = topologicalOrder();
    reverse(order.begin(), order.end());

    for (int taskId : order) {
        double averageExecution = 0.0;

        for (int k = 0; k < VM_COUNT; ++k) {
            averageExecution +=
                tasks[taskId].wcetAtFmax[k];
        }

        averageExecution /= static_cast<double>(VM_COUNT);
        tasks[taskId].avgET = averageExecution;

        double successorTerm = 0.0;

        for (int successor : tasks[taskId].successors) {
            successorTerm = max(
                successorTerm,
                tasks[successor].uRank +
                communicationCost(taskId, successor)
            );
        }

        tasks[taskId].uRank =
            successorTerm + averageExecution;
    }

    // The motivational example explicitly uses descending URank.
    taskList.resize(TASK_COUNT);
    iota(taskList.begin(), taskList.end(), 0);

    stable_sort(
        taskList.begin(),
        taskList.end(),
        [](int a, int b) {
            if (fabs(tasks[a].uRank - tasks[b].uRank) > 1e-9) {
                return tasks[a].uRank > tasks[b].uRank;
            }
            return a < b;
        }
    );
}
