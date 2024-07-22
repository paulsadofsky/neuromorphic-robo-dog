#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;



int main() {

    // opening input file to read in angles from csv file 
        ifstream inputFile("network-membrane-voltage.csv");

    // if input file is not valid closing
        if(!inputFile.is_open()){
            return 1;
        }

    // opening output file to print data to determine if spike or not
        ofstream outfile;
        outfile.open("spikes.csv");
        outfile << "t"<<"FR"<<"BR"<<"FL"<<"BL\n";

    // reading in values from network membrane voltage csv and entering values into a vector
    vector<double> time;
    vector<double> frontLeft;
    vector<double> frontRight;
    vector<double> backLeft;
    vector<double> backRight;

    string line;
    string strValue = "";
    int numOfCommas = 0;
    double value;

    // reading in first line which is the title of each column, not needed for vectors
    getline(inputFile,line);

    while(getline(inputFile,line)) 
    {
        strValue = "";
        numOfCommas = 0;
        for(int i = 0; i< line.length();i++)
        {

            if(line[i] != ',' && numOfCommas < 4)
                strValue += line[i];
            if(numOfCommas == 4)
            {
                strValue = line.substr(i,line.length());
                value = stod(strValue);
                backLeft.push_back(value);
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
                        frontRight.push_back(value);
                        break;
                    case 3:
                        backRight.push_back(value);
                        break;
                    case 4:
                        frontLeft.push_back(value);
                        break;    
                }

                value = "";
            } 
        }
    }

    for (int i = 0; i < time.size(); i++) {
        cout << time[i] << ","
             << frontLeft[i] << ","
             << frontRight[i] << ","
             << backLeft[i] << ","
             << backRight[i] << "\n";
    }



    outfile.close();
    inputFile.close();


    return 0;
}
