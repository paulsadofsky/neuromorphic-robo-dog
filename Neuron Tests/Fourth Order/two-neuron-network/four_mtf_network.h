#ifndef FOUR_MTF_NETWORK_H
#define FOUR_MTF_NETWORK_H

#include "mtfneuron.h"

class FourMTFNetwork {
    private:
        MTFNeuron *FL, *FR, *BL, *BR;
        double timestep;
    public:
        FourMTFNetwork();
        ~FourMTFNetwork();

        MTFNeuron* getFL();
        MTFNeuron* getFR();

        void setTimeStep(double);

        void calculateNetwork(double);

        void exportToCSV();
        void exportToCSV(int);
};

#endif