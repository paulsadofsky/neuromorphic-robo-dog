#ifndef MTFNEURON_H
#define MTFNEURON_H

#include <vector>

class MTFNeuron {
    private:
        double vmem, vx[3];
        double ax[4], dx[4], b, tau[3];
        double exti, dt, synapseCurrent;
        bool usePWL;
        int asynMatrix[2];
        int dsynMatrix[2];
        std::vector<double> calculatedVal;
    public:
        MTFNeuron();
        ~MTFNeuron();
        double sigmoidSynapseFunction(double voltage, double steepness, double centerPosition);
        void setAlpha(double, double, double, double);
        void setDelta(double, double, double, double);
        void setExtCurrent(double);
        void setTimeStep(double);
        void setTau(double, double, double);
        void setInitV(double, double, double, double);
        void setBeta(double);
        void setSynapseCurrent(double);
        void togglePWL(bool);
        void calculateValues(int);
        std::vector<double> getValues();
        void exportToCSV();
};

#endif