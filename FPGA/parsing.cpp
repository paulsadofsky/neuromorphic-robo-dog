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
        if(!inputFile.is_open())
        {
            return 1;
        }


    // // opening output file to print data to determine if spike or not
    //     ofstream outfile;
    //     outfile.open("spikes.csv");
    //     outfile << "t\tFR\tBR\tFL\tBL\n";



    // reading in values from network membrane voltage csv and entering values into a vector
    vector<double> time;
    vector<double> frontRight;
    vector<double> backRight;
    vector<double> frontLeft;
    vector<double> backLeft;

    string line;
    string value = "";
    int numOfCommas = 0;

    // reading in first line which is the title of each column, not needed for vectors
    getline(inputFile,line);

    while(getline(inputFile,line)) 
    {
        for(int i = 0; i< line.length();i++)
        {
            value = "";
            if(line[i] != ',')
                value += line[i];
            else
            {    
                numOfCommas++;
                time.push_back(value);
            }

                
            
        }
    }



    //outfile.close();
    inputFile.close();


    return 0;
}

