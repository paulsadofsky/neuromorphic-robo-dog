#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

int main() {
    // Threshold voltage to count as spike
    double thresh = 1.0;

    // Opening input file to read in angles from csv file 
        ifstream inputFile("network-membrane-voltage.csv");

    // If input file is not valid closing
        if(!inputFile.is_open()){
            return 1;
        }

    // Opening output file to print data to determine if spike or not
        ofstream outfile;
        outfile.open("spikes.txt");

    // Reading in values from network membrane voltage csv and entering values into a vector
    vector<double> time;
    vector<double> frontLeft;
    vector<double> frontRight;
    vector<double> backLeft;
    vector<double> backRight;

    string line;
    string strValue = "";
    int numOfCommas = 0;
    double value;

    // Reading in first line which is the title of each column, not needed for vectors
    getline(inputFile,line);

    // Loop for every voltage value
    while(getline(inputFile,line)) 
    {
        strValue = "";
        numOfCommas = 0;

        // Parses through each character of each line to extract values
        for(int i = 0; i< line.length();i++)
        {
            if(line[i] != ',' && numOfCommas < 4)
                strValue += line[i];
            else if(numOfCommas == 4)
            {
                strValue = line.substr(i,line.length());
                value = stod(strValue);
                backRight.push_back(value);
                strValue = "";
                i = line.length();
            }
            else
            {    
                value = stod(strValue);
                numOfCommas++;
                switch(numOfCommas)
                {
                    case 1: 
                        time.push_back(value);
                        break;
                    case 2:
                        frontLeft.push_back(value);
                        break;
                    case 3:
                        frontRight.push_back(value);
                        break;
                    case 4:
                        backLeft.push_back(value);
                        break;    
                }

                strValue = "";
            } 
        }
    }

    inputFile.close();

    // Checks a rising edge over the declared threshold voltage and outputs to the spike file
    bool activation[4] = {0,0,0,0};
    int nullCounter = 0;
    int numActivativations = 0;
    vector<int> zeroActivation;
    outfile << "activations: {";
    for(int i = 1; i < time.size(); i++) {
        activation[0] = (frontLeft[i-1] < thresh && frontLeft[i] > thresh);
        activation[1] = (frontRight[i-1] < thresh && frontRight[i] > thresh);
        activation[2] = (backLeft[i-1] < thresh && backLeft[i] > thresh);
        activation[3] = (backRight[i-1] < thresh && backRight[i] > thresh);

        // If the activations are all zero, then a counter increments and outputs the number of zero activations at the next neuron firing
        if(!activation[0] && !activation[1] && !activation[2] && !activation[3]) {
            nullCounter++;
        }
        else {
            
            zeroActivation.push_back(nullCounter);
            nullCounter = 0;

            numActivativations++;
            outfile << ",{";
            for (int j = 0; j < 4; j++) {
                outfile << activation[j] << ",";
            }
            outfile << "}";
        }
    }
    outfile << "}\nzeroActivations: {";
    for(int i = 1; i < zeroActivation.size(); i++) {
        outfile << zeroActivation[i] << ",";
    }
    outfile << zeroActivation[zeroActivation.size()-1] << "}\n";

    outfile << "Number of activations: " << numActivativations << endl;
    outfile << "Number of non-activation cycles: " << numActivativations+1;

    outfile.close();
    
    return 0;
}
