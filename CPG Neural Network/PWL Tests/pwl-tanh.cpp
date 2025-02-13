// Testing file to find close approximation to tanh function using a piecewise linear

#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

double alpha= -2.0;
double delta = -1.5;
double tanh_r, pwl_r;

int main() {
    // Initiallize output .csv file
    ofstream outfile;
    outfile.open("function-test.csv");
    outfile << "Time,TANH,PWL\n";

    for (int a = -20; a < 21; a++) {

        double lower_bound = -1 + delta;
        double upper_bound = 1 + delta;
        if (a < lower_bound) {
            pwl_r = -1*alpha;
        }
        else if (a > upper_bound) {
            pwl_r = alpha;
        }
        else {
            pwl_r = alpha*(a - delta);
        }

        tanh_r = alpha*tanh(a - delta);

        outfile << a << "," << tanh_r << "," << pwl_r << "\n";
    }

    outfile.close();
    return 0;
}
