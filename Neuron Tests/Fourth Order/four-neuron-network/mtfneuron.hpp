#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "mtfneuron.h"
using namespace std;

MTFNeuron::MTFNeuron() {
    vmem = -1;
    vx[0] = 0;
    vx[1] = 0;
    vx[2] = 0;
    exti = -1.5;
    dt = 0.5;
    ax[0] = -2.0;
    ax[1] = 2.0;
    ax[2] = -1.5;
    ax[3] = 1.5;
    dx[0] = 0;
    dx[1] = 0;
    dx[2] = -1.5;
    dx[3] = 1.5;
    tau[0] = 1;
    tau[1] = 50;
    tau[2] = 2500;
    usePWL = false;
}

void MTFNeuron::setAlpha(double fn, double sp, double sn, double usp) {
    ax[0] = fn;
    ax[1] = sp;
    ax[2] = sn;
    ax[3] = usp;
}

void MTFNeuron::setDelta(double fn, double sp, double sn, double usp) {
    dx[0] = fn;
    dx[1] = sp;
    dx[2] = sn;
    dx[3] = usp;
}

void MTFNeuron::setBeta(double beta) {
    b = beta;
}

void MTFNeuron::setExtCurrent(double i) {
    exti = i;
}

void MTFNeuron::setTimeStep(double t) {
    dt = t;
}

void MTFNeuron::setInitV(double mem, double f, double s, double us) {
    vmem = mem;
    vx[0] = f;
    vx[1] = s;
    vx[2] = us;
}

void MTFNeuron::setTau(double f, double s, double us) {
    tau[0] = f;
    tau[1] = s;
    tau[2] = us;
}

void MTFNeuron::togglePWL(bool togPWL) {
    usePWL = togPWL;
}

void MTFNeuron::calculateValues(int timesteps) {
    double i_x[4];
    double i_sum = 0.0;
    double v_temp = 0.0;

    calculatedVal.clear();

    // Initiallize output .csv file
    ofstream outfile;
    outfile.open("../membrane-voltage.csv");
    outfile << "Time,Voltage,\n";

    for (int a = 0; a < timesteps; a++) {
        i_sum = 0;

        // Calculates each value of v_x
        for(int k = 0; k < 3; k++) {
            vx[k] += (dt/tau[k])*(vmem - vx[k]);
        }

        for (int i = 0; i < 4; i++) {
            // Sets the appropriate value of v_x and it's corresponding sign
            if (i <= 1) { v_temp = vx[i]; }
            else { v_temp = vx[i-1]; }
            
            if (usePWL) {
                // PWL FUNCTION
                double lower_bound = -(ax[i]/b) + dx[i];
                double upper_bound = (ax[i]/b) + dx[i];
                if (v_temp < lower_bound) {
                    i_x[i] = -abs(ax[i]);
                }
                else if (v_temp > upper_bound) {
                    i_x[i] = abs(ax[i]);
                }
                else {
                    i_x[i] = b*(v_temp - dx[i]);
                }
            }
            else {
                // TANH FUNCTION
                i_x[i] = ax[i]*tanh(v_temp - dx[i]);
            }

            // Sums the current
            i_sum += i_x[i];
        }

        // Calculates the new v membrane and prints to display
        vmem += dt * (exti - vmem - i_sum);
        calculatedVal.push_back(vmem);

        outfile << a << "," << vmem << "\n";
    }

    outfile.close();
}