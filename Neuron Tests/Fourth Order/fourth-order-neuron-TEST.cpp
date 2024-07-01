#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

// Input current for the membrane
double v_mem = -1;
double ext_current = -1.5;

// Set value of C and time step
double c = 1.0;
double dt = 0.001;

// Sets the three time constant values, with each increasing index correlating to f, s and us respectively
// Initiallizes each voltage with the same respective index assignment
double tau_x[3] = {5.0, 250.0, 12500.0};
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

    for (int a = 0; a < 10000; a++) {
        i_sum = 0;

        // Calculates each value of v_x
        for(int k = 0; k < 3; k++) {
            v_x[k] += (tau_x[k]/dt)*(v_mem - v_x[k]);
        }
        
        for (int i = 0; i < 4; i++) {
            // Sets the appropriate value of v_x and it's corresponding sign
            if (i <= 1) { v_temp = v_x[i]; }
            else { v_temp = v_x[i-1]; }
            
            i_x[i] = alpha_x[i]*tanh(v_temp - delta_x[i]);

            // Negates the current of the 0th and 2nd current for fast negative (fn) and slow negative (sn)
            if (i == 0 || i == 2) {
                i_x[i] = i_x[i] * -1;
            }

            // Sums the current
            i_sum += i_x[i];
        }

        // Calculates the new v membrane and prints to display
        v_mem += (dt / c) * (ext_current - v_mem - i_sum);
        std::cout << v_mem << " ";
        std::cout << "\n";
        outfile << a << "," << v_mem << "\n";
    }

    outfile.close();
    return 0;
}