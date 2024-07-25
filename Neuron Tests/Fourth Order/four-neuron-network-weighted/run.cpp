#include <iostream>
#include <cmath>
#include <fstream>
#include "mtfneuron.cpp"
#include "four_mtf_network.cpp"
using namespace std;

int main() {
    FourMTFNetwork network;

    network.calculateNetwork(15000);
    network.exportToCSV(5000);
    
    return 0;
}