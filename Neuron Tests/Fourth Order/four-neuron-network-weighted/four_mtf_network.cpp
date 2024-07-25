#include "mtfneuron.h"
#include "four_mtf_network.h"
#include <fstream>
#include <cmath>
using namespace std;

// Default constructor that dynamically allocates the four neurons for the network
FourMTFNetwork::FourMTFNetwork() {
    FL = new MTFNeuron();
    FR = new MTFNeuron();
    BL = new MTFNeuron();
    BR = new MTFNeuron();
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            synapseCurrents[i][j] = 0;
        }
    }
}

// Default destructor that unallocates the four neurons declared to prevent memory leaks
FourMTFNetwork::~FourMTFNetwork() {
    delete FL;
    delete FR;
    delete BL;
    delete BR;
}

// Accessor functions that return the pointers to each neuron in the network
MTFNeuron* FourMTFNetwork::getFL() { return FL; }
MTFNeuron* FourMTFNetwork::getFR() { return FR; }
MTFNeuron* FourMTFNetwork::getBL() { return BL; }
MTFNeuron* FourMTFNetwork::getBR() { return BR; }

// Sets the dt of each neuron
void FourMTFNetwork::setTimeStep(double t) {
    FL->setTimeStep(t);
    FR->setTimeStep(t);
    BL->setTimeStep(t);
    BR->setTimeStep(t);
}

double FourMTFNetwork::sigmoidSynapseFunction(double voltage, double steepness, double centerPosition) {
    double k = steepness * (voltage - centerPosition);
    return 1.0 / (1.0 + std::exp(-k)); 
}

// Calculates the values of each neuron in the network
void FourMTFNetwork::calculateNetwork(double timesteps) {
    FL->clearVoltage();
    FR->clearVoltage();
    BL->clearVoltage();
    BR->clearVoltage();

    double asynMatrix[4][4] = {{ 0,   -0.3, -0.3,  0.3},    // FL
                               {-0.3,  0,   -0.3, -0.3},    // FR
                               {-0.3, -0.3,  0,   -0.3},    // BL
                               { 0.3, -0.3, -0.3,  0  }};   // BR

    double dsynMatrix[4][4] = {{ 0, -1, -1, -1},            // FL
                               {-1,  0, -1, -1},            // FR
                               {-1, -1,  0, -1},            // BL
                               {-1, -1, -1,  0}};           // BR
    double b = 2.0; 

    double synFL, synFR, synBL, synBR;

    for (int i = 0; i < timesteps; i++) {
        // Calculate outbound synapse currents from FL neuron
        synapseCurrents[0][1] = (asynMatrix[0][1]) * sigmoidSynapseFunction(FL->getMembraneVoltage(), b, dsynMatrix[0][1]);
        synapseCurrents[0][2] = (asynMatrix[0][2]) * sigmoidSynapseFunction(FL->getMembraneVoltage(), b, dsynMatrix[0][2]);
        synapseCurrents[0][3] = (asynMatrix[0][3]) * sigmoidSynapseFunction(FL->getMembraneVoltage(), b, dsynMatrix[0][3]);

        // Calculate outbound synapse currents from FR neuron
        synapseCurrents[1][0] = (asynMatrix[1][0]) * sigmoidSynapseFunction(FR->getMembraneVoltage(), b, dsynMatrix[1][0]);
        synapseCurrents[1][2] = (asynMatrix[1][2]) * sigmoidSynapseFunction(FR->getMembraneVoltage(), b, dsynMatrix[1][2]);
        synapseCurrents[1][3] = (asynMatrix[1][3]) * sigmoidSynapseFunction(FR->getMembraneVoltage(), b, dsynMatrix[1][3]);

        // Calculate outbound synapse currents from BL neuron
        synapseCurrents[2][0] = (asynMatrix[2][0]) * sigmoidSynapseFunction(BL->getMembraneVoltage(), b, dsynMatrix[2][0]);
        synapseCurrents[2][1] = (asynMatrix[2][1]) * sigmoidSynapseFunction(BL->getMembraneVoltage(), b, dsynMatrix[2][1]);
        synapseCurrents[2][3] = (asynMatrix[2][3]) * sigmoidSynapseFunction(BL->getMembraneVoltage(), b, dsynMatrix[2][3]);

        // Calculate outbound synapse currents from BR neuron
        synapseCurrents[3][0] = (asynMatrix[3][0]) * sigmoidSynapseFunction(BR->getMembraneVoltage(), b, dsynMatrix[3][0]);
        synapseCurrents[3][1] = (asynMatrix[3][1]) * sigmoidSynapseFunction(BR->getMembraneVoltage(), b, dsynMatrix[3][1]);
        synapseCurrents[3][2] = (asynMatrix[3][2]) * sigmoidSynapseFunction(BR->getMembraneVoltage(), b, dsynMatrix[3][2]);

        // Calculate the summation of inbound currents for each neuron
        synFL = synapseCurrents[1][0] + synapseCurrents[2][0] + synapseCurrents[3][0];
        synFR = synapseCurrents[0][1] + synapseCurrents[2][1] + synapseCurrents[3][1];
        synBL = synapseCurrents[0][2] + synapseCurrents[1][2] + synapseCurrents[3][2];
        synBR = synapseCurrents[0][3] + synapseCurrents[1][3] + synapseCurrents[2][3];

        // Calculate new membrane voltage of each neuron using synapse currents
        FL->calculateValue(synFL);
        FR->calculateValue(synFR);
        BL->calculateValue(synBL);
        BR->calculateValue(synBR);
    }
}

// Prints the calculated values to CSV format
void FourMTFNetwork::exportToCSV() {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("network-membrane-voltage.csv");
    outfile << "Time,VoltageFL,VoltageFR,VoltageBL,VoltageBR\n";
    std::vector<double> valFL = FL->getValues();
    std::vector<double> valFR = FR->getValues();
    std::vector<double> valBL = BL->getValues();
    std::vector<double> valBR = BR->getValues();

    for (int i = 0; i < valFL.size(); i++) {
        outfile << i << "," << valFL[i]
                     << "," << valFR[i]
                     << "," << valBL[i]
                     << "," << valBR[i]
                     <<  "\n";
    }

    outfile.close();
}

// Prints the calculated values to CSV format with an offset to remove beginning values
void FourMTFNetwork::exportToCSV(int offset) {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("network-membrane-voltage.csv");
    outfile << "Time,VoltageFL,VoltageFR,VoltageBL,VoltageBR\n";
    std::vector<double> valFL = FL->getValues();
    std::vector<double> valFR = FR->getValues();
    std::vector<double> valBL = BL->getValues();
    std::vector<double> valBR = BR->getValues();

    for (int i = 0; i < valFL.size(); i++) {
        if (i >= offset) {
            outfile << i-offset << "," << valFL[i]
                        << "," << valFR[i]
                        << "," << valBL[i]
                        << "," << valBR[i]
                        <<  "\n";
        }
    }

    outfile.close();
}