#ifndef FOUR_MTF_NETWORK_H
#define FOUR_MTF_NETWORK_H

#include "mtfneuron.h"

class FourMTFNetwork {
    private:
        MTFNeuron *FR, *FL, *BR, *BL;
        double timestep;
        double synapseCurrents[4][4];
    public:
        FourMTFNetwork();
        ~FourMTFNetwork();

        MTFNeuron* getFR();
        MTFNeuron* getFL();
        MTFNeuron* getBR();
        MTFNeuron* getBL();

        void setTimeStep(double);

        void calculateNetwork(double);

        double sigmoidSynapseFunction(double, double, double);

        void exportToCSV();
        void exportToCSV(int);
};

#endif