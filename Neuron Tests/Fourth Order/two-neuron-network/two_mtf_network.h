#ifndef TWO_MTF_NETWORK_H
#define TWO_MTF_NETWORK_H

#include "mtfneuron.h"

class TwoMTFNetwork {
    private:
        MTFNeuron *FL, *FR;
        double timestep;
    public:
        TwoMTFNetwork();
        ~TwoMTFNetwork();

        MTFNeuron* getFL();
        MTFNeuron* getFR();

        void setTimeStep(double);

        void calculateNetwork(double);

        double sigmoidSynapseFunction(double, double, double);

        void exportToCSV();
        void exportToCSV(int);
};

#endif