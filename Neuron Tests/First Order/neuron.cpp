#include <iostream>
#include <cmath>
using namespace std;

// Threshold and reset voltages and input current for the membrane
double v_thresh = -55;
double v_reset = -70;
double v_mem = v_reset;
double input_current = 1;

// Set values of R and C to calculate the time constant
double r = 1.0;
double c = 0.0009;
double tc = 1/(r*c);

// Set the time step to calculate for
double dt = 12;

int main() {
    // Runs program for ten spikes
    int spike_counter = 0;
    while (spike_counter < 10) {
        v_mem += (dt / tc) * (input_current * r - v_mem);
        cout << v_mem << " ";
    
        // Check for spike
        if (v_mem >= v_thresh) {
            // Reset the membrane potential
            v_mem = v_reset;
            cout << "\nSpike" << endl;
            spike_counter++;
        }
        cout << "\n";
    }

    return 0;
}


