#ifndef MTFNEURON_H
#define MTFNEURON_H

#include <vector>

class MTFNeuron {
    private:
        double vmem, vx[3];
        double ax[4], dx[4], b, tau[3];
        double appi, exti, dt;
        bool usePWL;
        std::vector<double> calculatedVal;
    public:
        MTFNeuron();
        ~MTFNeuron();
        void setAlpha(double, double, double, double);
        void setDelta(double, double, double, double);
        void setExtCurrent(double);
        void setTimeStep(double);
        void setTau(double, double, double);
        void setInitV(double, double, double, double);
        void setBeta(double);
        void setSynapseCurrent(double);
        void togglePWL(bool);
        void calculateValue(double);
        std::vector<double> getValues();
        double getAppliedCurrent();
        double getExtCurrent();
        double getMembraneVoltage();
        void exportToCSV();
        void clearVoltage();
};

#endif