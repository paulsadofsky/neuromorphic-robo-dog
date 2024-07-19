#include "mtfneuron.h"
#include "four_mtf_network.h"
#include <fstream>
#include <cmath>
using namespace std;

// Default constructor that dynamically allocates the four neurons for the network
FourMTFNetwork::FourMTFNetwork() {
    FR = new MTFNeuron();
    FL = new MTFNeuron();
    BR = new MTFNeuron();
    BL = new MTFNeuron();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++){
            synapseCurrents[i][j] = 0;
        }
    }
}

// Default destructor that unallocates the four neurons declared to prevent memory leaks
FourMTFNetwork::~FourMTFNetwork() {
    delete FR;
    delete FL;
    delete BR;
    delete BL;
}

// Accessor functions that return the pointers to each neuron in the network
MTFNeuron* FourMTFNetwork::getFR() { return FR; }
MTFNeuron* FourMTFNetwork::getFL() { return FL; }
MTFNeuron* FourMTFNetwork::getBR() { return BR; }
MTFNeuron* FourMTFNetwork::getBL() { return BL; }

// Sets the dt of each neuron
void FourMTFNetwork::setTimeStep(double t) {
    FR->setTimeStep(t);
    FL->setTimeStep(t);
    BR->setTimeStep(t);
    BL->setTimeStep(t);
}

double FourMTFNetwork::sigmoidSynapseFunction(double voltage, double steepness, double centerPosition) {
    double k = steepness * (voltage - centerPosition);
    return 1.0 / (1.0 + std::exp(-k)); 
}

// Calculates the values of each neuron in the network
void FourMTFNetwork::calculateNetwork(double timesteps) {
    FR->clearVoltage();
    FL->clearVoltage();
    BR->clearVoltage();
    BL->clearVoltage();

    double asynMatrix[4][4] = {{ 0,   -0.3, -0.3, -0.3},    // FR
                               {-0.3,  0,   -0.3, -0.3},    // FL
                               {-0.3, -0.3,  0,   -0.3},    // BR
                               {-0.3, -0.3, -0.3,  0  },};  // BL

    double dsynMatrix[4][4] = {{-1, -1, -1, -1},            // FR
                               {-1, -1, -1, -1},            // FL
                               {-1, -1, -1, -1},            // BR
                               {-1, -1, -1, -1}};           // BL
    double b = 2.0; 

    for (int i = 0; i < timesteps; i++) {
        synapseCurrents[0][1] = (asynMatrix[0][1]) * sigmoidSynapseFunction(FR->getMembraneVoltage(), b, dsynMatrix[0][1]);
        synapseCurrents[0][2] = (asynMatrix[0][2]) * sigmoidSynapseFunction(FR->getMembraneVoltage(), b, dsynMatrix[0][2]);
        synapseCurrents[0][3] = (asynMatrix[0][3]) * sigmoidSynapseFunction(FR->getMembraneVoltage(), b, dsynMatrix[0][3]);

        synapseCurrents[1][0] = (asynMatrix[1][0]) * sigmoidSynapseFunction(FL->getMembraneVoltage(), b, dsynMatrix[1][0]);
        synapseCurrents[1][2] = (asynMatrix[1][2]) * sigmoidSynapseFunction(FL->getMembraneVoltage(), b, dsynMatrix[1][2]);
        synapseCurrents[1][3] = (asynMatrix[1][3]) * sigmoidSynapseFunction(FL->getMembraneVoltage(), b, dsynMatrix[1][3]);

        synapseCurrents[2][0] = (asynMatrix[2][0]) * sigmoidSynapseFunction(BR->getMembraneVoltage(), b, dsynMatrix[2][0]);
        synapseCurrents[2][1] = (asynMatrix[2][1]) * sigmoidSynapseFunction(BR->getMembraneVoltage(), b, dsynMatrix[2][1]);
        synapseCurrents[2][3] = (asynMatrix[2][3]) * sigmoidSynapseFunction(BR->getMembraneVoltage(), b, dsynMatrix[2][3]);

        synapseCurrents[3][0] = (asynMatrix[3][0]) * sigmoidSynapseFunction(BL->getMembraneVoltage(), b, dsynMatrix[3][0]);
        synapseCurrents[3][1] = (asynMatrix[3][1]) * sigmoidSynapseFunction(BL->getMembraneVoltage(), b, dsynMatrix[3][1]);
        synapseCurrents[3][2] = (asynMatrix[3][2]) * sigmoidSynapseFunction(BL->getMembraneVoltage(), b, dsynMatrix[3][2]);

        FR->calculateValue(synapseCurrents[1][0] + synapseCurrents[2][0] + synapseCurrents[3][0]);
        FL->calculateValue(synapseCurrents[0][1] + synapseCurrents[2][1] + synapseCurrents[3][1]);
        BR->calculateValue(synapseCurrents[0][2] + synapseCurrents[1][2] + synapseCurrents[3][2]);
        BL->calculateValue(synapseCurrents[0][3] + synapseCurrents[1][3] + synapseCurrents[2][3]);
    }
}

// Prints the calculated values to CSV format
void FourMTFNetwork::exportToCSV() {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("network-membrane-voltage.csv");
    outfile << "Time,VoltageFR,VoltageFL,VoltageBR,VoltageBL\n";
    std::vector<double> valFR = FR->getValues();
    std::vector<double> valFL = FL->getValues();
    std::vector<double> valBR = BR->getValues();
    std::vector<double> valBL = BL->getValues();

    for (int i = 0; i < valFL.size(); i++) {
        outfile << i << "," << valFR[i]
                     << "," << valFL[i]
                     << "," << valBR[i]
                     << "," << valBL[i]
                     <<  "\n";
    }

    outfile.close();
}

// Prints the calculated values to CSV format with an offset to remove beginning values
void FourMTFNetwork::exportToCSV(int offset) {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("network-membrane-voltage.csv");
    outfile << "Time,VoltageFR,VoltageFL,VoltageBR,VoltageBL\n";
    std::vector<double> valFL = FR->getValues();
    std::vector<double> valFR = FL->getValues();
    std::vector<double> valBR = BR->getValues();
    std::vector<double> valBL = BL->getValues();

    for (int i = 0; i < valFL.size(); i++) {
        if (i >= offset) {
            outfile << i-offset << "," << valFR[i]
                        << "," << valFL[i]
                        << "," << valBR[i]
                        << "," << valBL[i]
                        <<  "\n";
        }
    }

    outfile.close();
}