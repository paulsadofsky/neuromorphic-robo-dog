#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

// Input current for the membrane
double v_mem = -1;
double ext_current = -1.5;

// Set value of time step
double dt = 0.5;

// Sets the three time constant values, with each increasing index correlating to f, s and us respectively
// Initiallizes each voltage with the same respective index assignment
double tau_x[3] = {1, 50, 2500.0};
double v_x[3] = {0, 0, 0};

// Sets the four alpha and delta values, with each increasing index correlating to fn, sp, sn and usp respectively
// Initiallizes each current with the same respective index assignment
double alpha_x[4] = {-2.0, 2.0, -1.5, 1.5};
double delta_x[4] = {0, 0, -1.5, -1.5};
double i_x[4];
double i_sum = 0.0;
double v_temp = 0.0;

int main() {
    // Initiallize output .csv file
    ofstream outfile;
    outfile.open("membrane-voltage.csv");
    outfile << "Time,Voltage,\n";

    for (int a = 0; a < 14000; a++) {
        i_sum = 0;

        // Calculates each value of v_x
        for(int k = 0; k < 3; k++) {
            v_x[k] += (dt/tau_x[k])*(v_mem - v_x[k]);
        }

        for (int i = 0; i < 4; i++) {
            // Sets the appropriate value of v_x and it's corresponding sign
            if (i <= 1) { v_temp = v_x[i]; }
            else { v_temp = v_x[i-1]; }
            
            i_x[i] = alpha_x[i]*tanh(v_temp - delta_x[i]);

            // Sums the current
            i_sum += i_x[i];
        }

        // Calculates the new v membrane and prints to display
        v_mem += dt * (ext_current - v_mem - i_sum);
        std::cout << v_mem << " ";
        std::cout << "\n";
        if (a >= 4000) {
            outfile << a-4000 << "," << v_mem << "\n";
        }
    }

    outfile.close();
    return 0;
}