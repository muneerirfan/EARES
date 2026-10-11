#include "eares.h"

static double actualStartTime(int taskId, int vmId, const vector<double>& finishTimes) {
    if (tasks[taskId].predecessors.empty()) {
        return 0.0;
    }

    double start = 0.0;
    for (int pred : tasks[taskId].predecessors) {
        double communication = (tasks[pred].selectedVM == vmId)
                             ? 0.0
                             : communicationCost(pred, taskId);
        start = max(start, finishTimes[pred] + communication);
    }
    return start;
}

void scheduleTasks() {
    // Eqs. (30)-(36) are evaluated before scheduling.
    computeEnergyPartitions();

    vector<double> finishTimes(TASK_COUNT, 0.0);
    double consumedEnergy = 0.0;

    // Algorithm 3 processes task_list in priority order.
    for (int position = 0; position < static_cast<int>(taskList.size()); ++position) {
        int taskId = taskList[position];

        // Eq. (36): Egiven(ti) = Egiven(G) - sum(completed E)
        //             - sum(preassigned unfinished Ep).
        double given = WORKFLOW_ENERGY - consumedEnergy;
        for (int future = position + 1; future < static_cast<int>(taskList.size()); ++future) {
            given -= tasks[taskList[future]].ePreassigned;
        }
        tasks[taskId].dynamicEgiven = given;

        double bestReliability = -1.0;
        int bestVM = -1;
        double bestFrequency = 0.0;
        double bestEnergy = 0.0;
        double bestFinish = 0.0;
        double bestStart = 0.0;

        for (int vmId = 0; vmId < VM_COUNT; ++vmId) {
            for (int step = 0; ; ++step) {
                double frequency = vms[vmId].fLow + step * FREQUENCY_STEP;
                if (frequency > vms[vmId].fMax + 1e-9) {
                    break;
                }
                frequency = min(frequency, vms[vmId].fMax);

                double energy = taskEnergy(taskId, vmId, frequency);
                if (energy > given + 1e-9) {
                    continue;
                }

                double reliability = taskReliability(taskId, vmId, frequency);
                double start = actualStartTime(taskId, vmId, finishTimes);
                double finish = start + executionTime(taskId, vmId, frequency);

                // Algorithm 3, line 14: highest reliability while AFT < d(ti).
                if (reliability > bestReliability
                    && finish < tasks[taskId].deadline) {
                    bestReliability = reliability;
                    bestVM = vmId;
                    bestFrequency = frequency;
                    bestEnergy = energy;
                    bestStart = start;
                    bestFinish = finish;
                }
            }
        }

        if (bestVM < 0) {
            // Keep the task explicitly unscheduled rather than silently
            // assigning an arbitrary VM; this follows the paper's feasibility
            // requirement instead of hiding an infeasible constraint.
            tasks[taskId].selectedVM = -1;
            tasks[taskId].selectedFrequency = 0.0;
            tasks[taskId].selectedEnergy = 0.0;
            tasks[taskId].selectedReliability = 0.0;
            tasks[taskId].startTime = 0.0;
            tasks[taskId].finishTime = 0.0;
            continue;
        }

        tasks[taskId].selectedVM = bestVM;
        tasks[taskId].selectedFrequency = bestFrequency;
        tasks[taskId].selectedEnergy = bestEnergy;
        tasks[taskId].selectedReliability = bestReliability;
        tasks[taskId].startTime = bestStart;
        tasks[taskId].finishTime = bestFinish;

        finishTimes[taskId] = bestFinish;
        consumedEnergy += bestEnergy;
    }

    totalEnergy = 0.0;
    makespan = 0.0;
    for (const auto& task : tasks) {
        if (task.selectedVM >= 0) {
            totalEnergy += task.selectedEnergy;
            makespan = max(makespan, task.finishTime);
        }
    }
    workflowReliability = workflowReliabilityValue();
}
