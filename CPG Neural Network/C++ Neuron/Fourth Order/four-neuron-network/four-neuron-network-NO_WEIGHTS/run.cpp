#include <iostream>
#include <cmath>
#include <fstream>
#include "mtfneuron.cpp"
#include "four_mtf_network.cpp"
using namespace std;

int main() {
    FourMTFNetwork network;

    // Adjusts external current values of FR and BL to create a shift in paterns
    network.getFR()->setExtCurrent(-2.0);
    network.getBL()->setExtCurrent(-2.0);

    network.calculateNetwork(20000);
    network.exportToCSV(3000);
    
    return 0;
}