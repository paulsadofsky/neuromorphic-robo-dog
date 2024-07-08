#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "mtfneuron.h"
using namespace std;

// Default constructor that sets the values to the ones provided in python-neuron-source.py file
MTFNeuron::MTFNeuron() {
    setAlpha(-2.0, 2.0, -1.5, 1.5);
    setDelta(0, 0, -1.5, -1.5);
    setTau(1, 50, 2500);
    setInitV(-1, 0, 0, 0);
    setExtCurrent(-1.5);
    setTimeStep(0.5);
    togglePWL(false);
}

// Default constructor
MTFNeuron::~MTFNeuron() {
    calculatedVal.clear();
}

// Sets alpha values of neuron
void MTFNeuron::setAlpha(double fn, double sp, double sn, double usp) {
    ax[0] = fn;
    ax[1] = sp;
    ax[2] = sn;
    ax[3] = usp;
}

// Sets delta values of neuron
void MTFNeuron::setDelta(double fn, double sp, double sn, double usp) {
    dx[0] = fn;
    dx[1] = sp;
    dx[2] = sn;
    dx[3] = usp;
}

// Sets beta value of neuron
void MTFNeuron::setBeta(double beta) {
    b = beta;
}

// Sets external current of neuron
void MTFNeuron::setExtCurrent(double i) {
    exti = i;
}

// Sets timestep of neuron
void MTFNeuron::setTimeStep(double t) {
    dt = t;
}

// Sets initial voltages (membrane voltage, V_f, V_s, V_us) of neuron
void MTFNeuron::setInitV(double mem, double f, double s, double us) {
    vmem = mem;
    vx[0] = f;
    vx[1] = s;
    vx[2] = us;
}

// Sets the time constants (f, s , us) of neuron
void MTFNeuron::setTau(double f, double s, double us) {
    tau[0] = f;
    tau[1] = s;
    tau[2] = us;
}

// Sets the boolean variable that switches between PWL and tanh model
void MTFNeuron::togglePWL(bool togPWL) {
    usePWL = togPWL;
}

// Calculating the values based on the equations in the ICONS paper
void MTFNeuron::calculateValues(int timesteps) {
    double i_x[4];
    double i_sum = 0.0;
    double v_temp = 0.0;

    calculatedVal.clear();

    for (int i = 0; i < timesteps; i++) {
        i_sum = 0;

        // Calculates each value of v_x
        for(int j = 0; j < 3; j++) {
            vx[j] += (dt/tau[j])*(vmem - vx[j]);
        }

        for (int k = 0; k < 4; k++) {
            // Sets the appropriate value of v_x and it's corresponding sign
            if (k <= 1) { v_temp = vx[k]; }
            else { v_temp = vx[k-1]; }
            
            if (usePWL) {
                // PWL FUNCTION
                double lower_bound = -(ax[k]/b) + dx[k];
                double upper_bound = (ax[k]/b) + dx[k];
                if (v_temp < lower_bound) {
                    i_x[k] = -1*ax[k];
                }
                else if (v_temp > upper_bound) {
                    i_x[k] = ax[k];
                }
                else {
                    i_x[k] = b*(v_temp - dx[k]);
                }
            }
            else {
                // TANH FUNCTION
                i_x[k] = ax[k]*tanh(v_temp - dx[k]);
            }

            // Sums the current
            i_sum += i_x[k];
        }

        // Calculates the new v membrane and prints to display
        vmem += dt * (exti - vmem - i_sum);
        calculatedVal.push_back(vmem);
    }
}

// Accessor function for the vector of calculated values
std::vector<double> MTFNeuron::getValues() {
    return calculatedVal;
}

// Writes calculated neuron values to CSV for individual testing
void MTFNeuron::exportToCSV() {
    // Initiallize output .csv file
    ofstream outfile;
    outfile.open("membrane-voltage.csv");
    outfile << "Time,Voltage,\n";

    for (int i = 0; i < calculatedVal.size(); i++) {
        outfile << i << "," << calculatedVal[i] << "\n";
    }

    outfile.close();
}