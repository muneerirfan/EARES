#include "eares.h"

// Published motivational-example trace from Table IV.
// These values are intentionally kept separate from the algorithmic
// calculation because the paper's published Table II/III inputs and
// printed equations do not reproduce every Table IV number exactly.

struct PublishedRow {
    double urank;
    double deadline;
    double egiven;
    int vm;
    double frequency;
    double energy;
    double reliability;
};

static const PublishedRow TABLE_IV[TASK_COUNT] = {
    {108.00,  13.89,  7.3581, 3, 0.81, 7.3388, 0.9948},
    { 77.00,  39.67, 12.9878, 1, 1.00,10.7900, 0.9990},
    { 80.00,   9.55,  9.5554, 1, 1.00, 9.1300, 0.9983},
    { 80.00,  39.33,  9.2609, 2, 1.00, 5.9200, 0.9984},
    { 69.00,  52.16,  8.6926, 2, 0.92, 8.5954, 0.9964},
    { 63.33,  56.37,  7.2177, 3, 0.79, 7.1170, 0.9945},
    { 42.67,  83.89,  8.1410, 1, 1.00, 5.8100, 0.9989},
    { 35.67,  80.84,  8.6197, 1, 1.00, 4.1500, 0.9992},
    { 44.33,  77.89, 11.3501, 2, 1.00, 8.8800, 0.9976},
    { 14.67, 110.00, 12.7162, 1, 1.00, 5.1800, 0.9986}
};

// The paper's task order is:
// t1, t3, t4, t2, t5, t6, t9, t7, t8, t10.
static const int PAPER_TASK_ORDER[TASK_COUNT] = {
    0, 2, 3, 1, 4, 5, 8, 6, 7, 9
};

void printPublishedTableIV() {
    cout << fixed << setprecision(4);
    cout << "EARES - published motivational example (Table IV)\n";
    cout << "==================================================\n";
    cout << "Deadline: " << WORKFLOW_DEADLINE << "\n";
    cout << "Energy constraint: " << WORKFLOW_ENERGY << "\n";
    cout << "Checkpoint overhead Oc: " << CHECKPOINT_OVERHEAD << "\n";
    cout << "Frequency precision: " << FREQUENCY_STEP << "\n\n";

    cout << "Task order: ";
    for (int i = 0; i < TASK_COUNT; ++i) {
        if (i) cout << ", ";
        cout << tasks[PAPER_TASK_ORDER[i]].name;
    }
    cout << "\nCritical path: t1 -> t2 -> t9 -> t10\n";
    cout << "Scale factor S(CP): 0.03\n\n";

    cout << left
         << setw(6)  << "Task"
         << setw(11) << "URank"
         << setw(11) << "d(ti)"
         << setw(12) << "Egiven"
         << setw(8)  << "VM"
         << setw(9)  << "f"
         << setw(11) << "E(ti)"
         << setw(12) << "R(ti)"
         << '\n';
    cout << string(80, '-') << '\n';

    for (int id : PAPER_TASK_ORDER) {
        const PublishedRow& r = TABLE_IV[id];
        cout << left
             << setw(6)  << tasks[id].name
             << setw(11) << r.urank
             << setw(11) << r.deadline
             << setw(12) << r.egiven
             << setw(8)  << ("vm" + to_string(r.vm))
             << setw(9)  << r.frequency
             << setw(11) << r.energy
             << setw(12) << r.reliability
             << '\n';
    }

    cout << '\n';
    cout << "E(G) = 72.9112 <= Egoal(G) = 80.0000\n";
    cout << "R(G) = 0.9751\n";
    cout << "SL(G) = 82.11\n\n";
}
