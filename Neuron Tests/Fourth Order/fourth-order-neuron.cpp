#include <iostream>
#include <cmath>
using namespace std;

// Threshold and reset voltages and input current for the membrane
double v_thresh = 2;
double v_reset = -2;
double v_mem = v_reset;
double ext_current = -2;

// Set value of C and time step
double c = 0.0009;
double dt = 12;

// Sets the three time constant values, with each increasing index correlating to f, s and us respectively
// Initiallizes each voltage with the same respective index assignment
double tau_x[3] = {5.0, 250.0, 12500.0};
double v_x[3];

// Sets the four alpha, beta, and delta values, with each increasing index correlating to fn, sp, sn and usp respectively
// Initiallizes each current with the same respective index assignment
double alpha_x[4] = {1.0, 1.0, 1.0, 1.0};
double beta_x[4] = {1.0, 1.0, 1.0, 1.0};
double delta_x[4] = {1.0, 1.0, 1.0, 1.0};
double i_x[4];
double i_sum = 0.0;
double v_temp = 0.0;

int main() {
    // Calculates each value of i_x and the summation
    for (int i = 0; i < 4; i++) {
        switch (i) {
            case (0): { v_temp = -v_x[0]; }
            case (1): { v_temp = v_x[1]; }
            case (2): { v_temp = -v_x[1]; }
            case (3): { v_temp = v_x[2]; }
        }
        
        double lower_bound = -(alpha_x[i]/beta_x[i]) + delta_x[i];
        double upper_bound = (alpha_x[i]/beta_x[i]) + delta_x[i];
        if (v_temp < lower_bound) {
            i_x[i] = -alpha_x[i];
        }
        else if (v_temp > upper_bound) {
            i_x[i] = alpha_x[i];
        }
        else {
            
            i_x[i] = beta_x[i]*(v_temp - delta_x[i]);
        }

        i_sum += i_x[i];

        // Calculates each value of v_x
        if (i != 3) {
            v_x[i] += (tau_x[i]/dt)*(v_mem - v_x[i]);
        }
    }

    v_mem += (dt / c) * (ext_current - v_mem - i_sum);
    cout << v_mem << " ";

    // Check for spike and resets the membrane potential if spike occurs
    if (v_mem >= v_thresh) {
        v_mem = v_reset;
        cout << "\nSpike" << endl;
    }
    cout << "\n";

    return 0;
}