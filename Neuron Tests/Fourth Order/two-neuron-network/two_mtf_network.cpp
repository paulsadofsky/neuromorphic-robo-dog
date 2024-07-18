#include "mtfneuron.h"
#include "two_mtf_network.h"
#include <fstream>
#include <cmath>

// Default constructor that dynamically allocates the two neurons for the network
TwoMTFNetwork::TwoMTFNetwork() {
    FL = new MTFNeuron();
    FR = new MTFNeuron();
}

// Default destructor that unallocates the two neurons declared to prevent memory leaks
TwoMTFNetwork::~TwoMTFNetwork() {
    delete FL;
    delete FR;
}

// Accessor functions that return the pointers to each neuron in the network
MTFNeuron* TwoMTFNetwork::getFL() { return FL; }
MTFNeuron* TwoMTFNetwork::getFR() { return FR; }

// Sets the dt of each neuron
void TwoMTFNetwork::setTimeStep(double t) {
    FL->setTimeStep(t);
    FR->setTimeStep(t);
}

double TwoMTFNetwork::sigmoidSynapseFunction(double voltage, double steepness, double centerPosition) {
    double k = steepness * (voltage - centerPosition);
    return 1.0 / (1.0 + std::exp(-k)); 
}

// Calculates the values of each neuron in the network
void TwoMTFNetwork::calculateNetwork(double timesteps) {
    double asynMatrix[1][2] = {{1.0, 1.0}};
    double dsynMatrix[1][2] = {{1.0, 1.0}};
    double b = 2.0; 
    

    /*
    vm1, vf1, vs1, vus1, vm2, vf2, vs2, vus2, vm3, vf3, vs3, vus3, vm4, vf4, vs4, vus4 =S
    # Iapp
    I1=Iapp[0]+asyn[1][0]*sf(vs2,b,dsyn[1][0])+asyn[2][0]*sf(vs3,b,dsyn[2][0])+asyn[3][0]*sf(vs4,b,dsyn[3][0])
    I2=Iapp[1]+asyn[0][1]*sf(vs1,b,dsyn[0][1])+asyn[2][1]*sf(vs3,b,dsyn[2][1])+asyn[3][1]*sf(vs4,b,dsyn[3][1])
    I3=Iapp[2]+asyn[0][2]*sf(vs1,b,dsyn[0][2])+asyn[1][2]*sf(vs2,b,dsyn[1][2])+asyn[3][2]*sf(vs4,b,dsyn[3][2])
    I4=Iapp[3]+asyn[0][3]*sf(vs1,b,dsyn[0][3])+asyn[1][3]*sf(vs2,b,dsyn[1][2])+asyn[2][3]*sf(vs3,b,dsyn[2][3])
    */
    std::vector<double> synapseCurrent1(2);
    std::vector<double> synapseCurrent2(2);

    for (int i = 0; i < timesteps; i++) {
        synapseCurrent1 = (-1.5) + (asynMatrix[0][0]) * sigmoidSynapseFunction(FR->getValues(), b, dsynMatrix[0][0]);
        synapseCurrent2 = (-1.5) + (asynMatrix[0][1]) * sigmoidSynapseFunction(FL->getValues(), b, dsynMatrix[0][1]);

        FL->calculateValues(timesteps, synapseCurrent1);
        FR->calculateValues(timesteps, synapseCurrent2);
    }
}

// Prints the calculated values to CSV format
void TwoMTFNetwork::exportToCSV() {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("network-membrane-voltage.csv");
    outfile << "Time,VoltageFL,VoltageFR,VoltageBL,VoltageBR\n";
    std::vector<double> valFL = FL->getValues();
    std::vector<double> valFR = FR->getValues();

    for (int i = 0; i < valFL.size(); i++) {
        outfile << i << "," << valFL[i]
                     << "," << valFR[i]
                     <<  "\n";
    }

    outfile.close();
}

// Prints the calculated values to CSV format with an offset to remove beginning values
void TwoMTFNetwork::exportToCSV(int offset) {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("network-membrane-voltage.csv");
    outfile << "Time,VoltageFL,VoltageFR,VoltageBL,VoltageBR\n";
    std::vector<double> valFL = FL->getValues();
    std::vector<double> valFR = FR->getValues();

    for (int i = 0; i < valFL.size(); i++) {
        if (i >= offset) {
            outfile << i-offset << "," << valFL[i]
                        << "," << valFR[i]
                        <<  "\n";
        }
    }

    outfile.close();
}