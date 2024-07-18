#include <iostream>
#include <cmath>
#include <fstream>
#include "mtfneuron.cpp"
#include "two_mtf_network.cpp"
using namespace std;

int main() {
    TwoMTFNetwork network;

    network.calculateNetwork(20000);
    network.exportToCSV();
    
    return 0;
}