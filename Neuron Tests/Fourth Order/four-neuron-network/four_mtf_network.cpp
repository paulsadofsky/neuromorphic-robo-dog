#include "mtfneuron.h"
#include "four_mtf_network.h"
#include <fstream>


FourMTFNetwork::FourMTFNetwork() {
    FL = new MTFNeuron();
    FR = new MTFNeuron();
    BL = new MTFNeuron();
    BR = new MTFNeuron();
}

FourMTFNetwork::~FourMTFNetwork() {
    delete FL;
    delete FR;
    delete BL;
    delete BR;
}

MTFNeuron* FourMTFNetwork::getFL() {
    return FL;
}

MTFNeuron* FourMTFNetwork::getFR() {
    return FR;
}

MTFNeuron* FourMTFNetwork::getBL() {
    return BL;
}

MTFNeuron* FourMTFNetwork::getBR() {
    return BR;
}

void FourMTFNetwork::setTimeStep(double t) {
    FL->setTimeStep(t);
    FR->setTimeStep(t);
    BL->setTimeStep(t);
    BR->setTimeStep(t);
}

void FourMTFNetwork::calculateNetwork(double timesteps) {
    FL->calculateValues(timesteps);
    FR->calculateValues(timesteps);
    BL->calculateValues(timesteps);
    BR->calculateValues(timesteps);
}

void FourMTFNetwork::exportToCSV() {
    // Initiallize output .csv file
    std::ofstream outfile;
    outfile.open("../network-membrane-voltage.csv");
    outfile << "Time,VoltageFL,VoltageFR,VoltageBL,VoltageBR\n";
    std::vector<double> valFL = FL->getValues();
    std::vector<double> valFR = FR->getValues();
    std::vector<double> valBL = BL->getValues();
    std::vector<double> valBR = BR->getValues();

    for (int i = 0; i < valFL.size(); i++) {
        outfile << i << "," << valFL[i]
                     << "," << valFR[i]
                     << "," << valBL[i]
                     << "," << valBR[i] <<  "\n";
    }

    outfile.close();
}