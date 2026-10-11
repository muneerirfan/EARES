#include "eares.h"

// Eq. (7): E(ti,vm_k^f) = (Pind + Cef*f^m) * ET(ti,vm_k^f).
double taskEnergy(int taskId, int vmId, double frequency) {
    double et = executionTime(taskId, vmId, frequency);
    return (vms[vmId].pInd
          + vms[vmId].cEf * pow(frequency, vms[vmId].m)) * et;
}

void computeEnergyPartitions() {
    // Eqs. (30)-(31).
    for (auto& task : tasks) {
        task.eMax = -numeric_limits<double>::infinity();
        task.eMin = numeric_limits<double>::infinity();

        for (int k = 0; k < VM_COUNT; ++k) {
            task.eMax = max(task.eMax, taskEnergy(task.id, k, vms[k].fMax));
            task.eMin = min(task.eMin, taskEnergy(task.id, k, vms[k].fLow));
        }
    }

    // Eqs. (32)-(33).
    double eMaxG = 0.0;
    double eMinG = 0.0;
    for (const auto& task : tasks) {
        eMaxG += task.eMax;
        eMinG += task.eMin;
    }

    // Eq. (34).
    for (auto& task : tasks) {
        task.eLevel = (task.eMin + task.eMax) / (eMinG + eMaxG);
    }

    // Eq. (35).
    for (auto& task : tasks) {
        task.ePreassigned = min(
            task.eMax,
            (WORKFLOW_ENERGY - eMinG) * task.eLevel + task.eMin
        );
    }
}
