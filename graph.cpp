#include "eares.h"

vector<Task> tasks;
vector<VM> vms;
vector<Edge> edges;
vector<int> taskList;
vector<int> criticalPath;
double scaleFactor = 0.0;
double totalEnergy = 0.0;
double workflowReliability = 1.0;
double makespan = 0.0;

const Edge* findEdge(int from, int to) {
    for (const auto& edge : edges) {
        if (edge.from == from && edge.to == to) {
            return &edge;
        }
    }
    return nullptr;
}

double communicationCost(int from, int to) {
    const Edge* edge = findEdge(from, to);
    if (edge == nullptr) {
        return 0.0;
    }
    return edge->weight / BANDWIDTH;
}

void initializeWorkflow() {
    vms = {
        {"vm1", 0.03, 0.80, 2.90, 0.26, 1.00, DEFAULT_LAMBDA_FMAX},
        {"vm2", 0.04, 0.80, 2.50, 0.26, 1.00, DEFAULT_LAMBDA_FMAX},
        {"vm3", 0.07, 1.00, 2.50, 0.29, 1.00, DEFAULT_LAMBDA_FMAX}
    };

    // Table II: WCET at fmax for the motivational example.
    const double wcet[VM_COUNT][TASK_COUNT] = {
        {14, 13, 11, 13, 12, 13, 7, 5, 18, 21},
        {16, 19, 13,  8, 13, 16,15,11, 12,  7},
        { 9, 18, 19, 17, 10,  9,11,14, 20, 16}
    };

    tasks.clear();
    for (int i = 0; i < TASK_COUNT; ++i) {
        Task task;
        task.id = i;
        task.name = "t" + to_string(i + 1);
        task.wcetAtFmax.resize(VM_COUNT);
        for (int k = 0; k < VM_COUNT; ++k) {
            task.wcetAtFmax[k] = wcet[k][i];
        }
        tasks.push_back(task);
    }

    // Fig. 1 communication graph. b_bar = 1 Gbps in the example,
    // so the numerical edge weights are also the average CC values.
    edges = {
        {0, 1, 18},
        {0, 2, 12},
        {0, 3,  9},
        {0, 4, 11},
        {0, 5, 14},
        {1, 7, 19},
        {1, 8, 16},
        {2, 6, 23},
        {3, 7, 27},
        {3, 8, 23},
        {4, 8, 13},
        {5, 7, 15},
        {6, 9, 17},
        {7, 9, 11},
        {8, 9, 13}
    };

    for (auto& task : tasks) {
        task.predecessors.clear();
        task.successors.clear();
    }

    for (const auto& edge : edges) {
        tasks[edge.from].successors.push_back(edge.to);
        tasks[edge.to].predecessors.push_back(edge.from);
    }
}
