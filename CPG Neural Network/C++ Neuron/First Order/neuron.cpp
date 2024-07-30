#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

// Threshold and reset voltages and input current for the membrane
double v_thresh = 20;
double v_reset = 0;
double v_mem = v_reset;
double input_current = 10;

// Set value of time constant
double tc = 1000;

// Set the time step to calculate for
double dt = 1;

int main() {
    ofstream outfile;
    outfile.open("voltage.csv");
    outfile << "Time,Voltage\n";

    // Runs program for ten spikes
    int counter = 0;
    while (counter < 1000) {
        v_mem += ((input_current*20 - v_mem)/1024);
        cout << v_mem << " ";

        outfile << counter << "," << v_mem << "\n";
    
        // Check for spike
        if (v_mem >= v_thresh) {
            // Reset the membrane potential
            v_mem = v_reset;
            cout << "\nSpike" << endl;
        }
        cout << "\n";

        counter++;
    }

    outfile.close();

    return 0;
}


