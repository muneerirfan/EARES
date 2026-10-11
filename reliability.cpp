#include "eares.h"

int factorialInt(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double poissonProbability(int g, double lambda, double executionTimeValue) {
    // Eq. (14).
    double x = lambda * executionTimeValue;
    return exp(-x) * pow(x, g) / static_cast<double>(factorialInt(g));
}

double taskReliability(int taskId, int vmId, double frequency) {
    double et = executionTime(taskId, vmId, frequency);
    double interval = checkpointInterval(et);
    double etWorst = worstExecutionTime(et);

    // Eq. (15).
    double denominator = vms[vmId].fMax - vms[vmId].fLow;
    double ratio = denominator == 0.0
                 ? 0.0
                 : (vms[vmId].fMax - frequency) / denominator;
    double lambda = vms[vmId].lambdaFMax
                  * pow(10.0, DEFAULT_FAULT_SENSITIVITY * ratio);

    // Eqs. (16)-(17).
    double reliability = 0.0;
    for (int g = 0; g <= KU; ++g) {
        double pg = poissonProbability(g, lambda, etWorst);
        double recovered = exp(-lambda * interval);
        reliability += pg * recovered;
    }

    return reliability;
}

double workflowReliabilityValue() {
    double result = 1.0;
    for (const auto& task : tasks) {
        if (task.selectedVM < 0) {
            return 0.0;
        }
        result *= task.selectedReliability;
    }
    return result;
}
