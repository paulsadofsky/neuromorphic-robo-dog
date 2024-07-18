#include "mtfneuron.h"
#include "two_mtf_network.h"
#include <fstream>
#include <cmath>
using namespace std;

// Default constructor that dynamically allocates the two neurons for the network
TwoMTFNetwork::TwoMTFNetwork() {
    FL = new MTFNeuron();
    FR = new MTFNeuron();
    synapseCurrents[0][0] = 0;
    synapseCurrents[0][1] = 0;
    synapseCurrents[1][0] = 0;
    synapseCurrents[1][1] = 0;
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
    FL->clearVoltage();
    FR->clearVoltage();
    
    double asynMatrix[2][2] = {{0, -0.3},
                               {-1, 0}};
    double dsynMatrix[2][2] = {{-1, -1},
                               {-1, -1}};
    double b = 2.0; 

    for (int i = 0; i < timesteps; i++) {
        synapseCurrents[0][1] = (asynMatrix[0][1]) * sigmoidSynapseFunction(FR->getMembraneVoltage(), b, dsynMatrix[0][1]);
        synapseCurrents[1][0] = (asynMatrix[1][0]) * sigmoidSynapseFunction(FL->getMembraneVoltage(), b, dsynMatrix[1][0]);

        FL->calculateValue(synapseCurrents[0][1]);
        FR->calculateValue(synapseCurrents[1][0]);
    }
}

// Prints the calculated values to CSV format
void TwoMTFNetwork::exportToCSV() {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("network-membrane-voltage.csv");
    outfile << "Time,VoltageFL,VoltageFR\n";
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
    outfile << "Time,VoltageFL,VoltageFR\n";
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