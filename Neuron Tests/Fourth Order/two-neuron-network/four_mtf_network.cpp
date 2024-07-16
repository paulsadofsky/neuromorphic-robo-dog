#include "mtfneuron.h"
#include "four_mtf_network.h"
#include <fstream>

// Default constructor that dynamically allocates the four neurons for the network
FourMTFNetwork::FourMTFNetwork() {
    FL = new MTFNeuron();
    FR = new MTFNeuron();
}

// Default destructor that unallocates the four neurons declared to prevent memory leaks
FourMTFNetwork::~FourMTFNetwork() {
    delete FL;
    delete FR;
}

// Accessor functions that return the pointers to each neuron in the network
MTFNeuron* FourMTFNetwork::getFL() { return FL; }
MTFNeuron* FourMTFNetwork::getFR() { return FR; }

// Sets the dt of each neuron
void FourMTFNetwork::setTimeStep(double t) {
    FL->setTimeStep(t);
    FR->setTimeStep(t);
}

// Calculates the values of each neuron in the network
void FourMTFNetwork::calculateNetwork(double timesteps) {
    FL->calculateValues(timesteps);
    FR->calculateValues(timesteps);
}

// Prints the calculated values to CSV format
void FourMTFNetwork::exportToCSV() {
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
void FourMTFNetwork::exportToCSV(int offset) {
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