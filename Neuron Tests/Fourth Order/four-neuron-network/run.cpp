#include <iostream>
#include <cmath>
#include <fstream>
#include "mtfneuron.cpp"
#include "four_mtf_network.cpp"
using namespace std;

int main() {
    FourMTFNetwork network;

    // Adjusts alpha values of FR and BL to create a shift in paterns
    network.getFR()->setDelta(0, 0, -1.0, -1.0);
    network.getBL()->setDelta(0, 0, -1.0, -1.0);

    network.calculateNetwork(14000);
    network.exportToCSV();
    
    return 0;
}