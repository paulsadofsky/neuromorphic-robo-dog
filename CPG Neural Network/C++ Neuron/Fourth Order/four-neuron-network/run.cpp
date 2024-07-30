#include <iostream>
#include <cmath>
#include <fstream>
#include "mtfneuron.cpp"
#include "four_mtf_network.cpp"
using namespace std;

int main() {
    FourMTFNetwork network;

    network.calculateNetwork(4750);
    network.exportToCSV(2800);
    
    return 0;
}