#ifndef EARES_H
#define EARES_H

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

const int TASK_COUNT = 10;
const int VM_COUNT = 3;
const double WORKFLOW_DEADLINE = 110.0;
const double WORKFLOW_ENERGY = 80.0;
const double BANDWIDTH = 1.0;
const double CHECKPOINT_OVERHEAD = 0.1;
const int KU = 2;
const double FREQUENCY_STEP = 0.01;

// The paper gives the motivational-example VM power parameters and f_low.
// It does not specify a single numerical d or lambda_fmax for that example.
// These defaults are therefore explicit, configurable assumptions.
const double DEFAULT_LAMBDA_FMAX = 1e-6;
const double DEFAULT_FAULT_SENSITIVITY = 1.0;

struct Edge {
    int from;
    int to;
    double weight;
};

struct VM {
    string name;
    double pInd;
    double cEf;
    double m;
    double fLow;
    double fMax;
    double lambdaFMax;
};

struct Task {
    int id;
    string name;
    vector<int> predecessors;
    vector<int> successors;
    vector<double> wcetAtFmax;
    double avgET = 0.0;
    double uRank = 0.0;
    double etWorstMax = 0.0;
    double est = 0.0;
    double lft = 0.0;
    double eet = 0.0;
    double deadline = 0.0;
    double eMax = 0.0;
    double eMin = 0.0;
    double eLevel = 0.0;
    double ePreassigned = 0.0;

    int selectedVM = -1;
    double selectedFrequency = 0.0;
    double selectedEnergy = 0.0;
    double selectedReliability = 0.0;
    double startTime = 0.0;
    double finishTime = 0.0;
};

extern vector<Task> tasks;
extern vector<VM> vms;
extern vector<Edge> edges;
extern vector<int> taskList;
extern vector<int> criticalPath;
extern double scaleFactor;
extern double totalEnergy;
extern double workflowReliability;
extern double makespan;

void initializeWorkflow();
const Edge* findEdge(int from, int to);
double communicationCost(int from, int to);

void computeURanksAndTaskList();

int checkpointCount(double executionTime);
double checkpointInterval(double executionTime);
double bestExecutionTime(double executionTime);
double worstExecutionTime(double executionTime);
double executionTime(int taskId, int vmId, double frequency);
void initializeTimingValues();
void partitionDeadlines();

void computeEnergyPartitions();
double taskEnergy(int taskId, int vmId, double frequency);

int factorialInt(int n);
double poissonProbability(int g, double lambda, double executionTime);
double taskReliability(int taskId, int vmId, double frequency);
double workflowReliabilityValue();

void scheduleTasks();
void printResults();

#endif
