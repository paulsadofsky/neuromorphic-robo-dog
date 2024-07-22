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
    // outfile.open("spikes.csv");
    // outfile << "t,FL,FR,BL,BR";



    // reading in values from network membrane voltage csv and entering values into a vector
    vector<double> time;
    vector<double> frontLeft;
    vector<double> frontRight;
    vector<double> backLeft;
    vector<double> backRight;

    string line;
    string value = "";
    int numOfCommas = 0;

    // reading in first line which is the title of each column, not needed for vectors
    getline(inputFile,line);

    while(getline(inputFile,line)) 
    {
        for(int i = 0; i < line.length(); i++)
        {
            if(line[i] != ',' && numOfCommas < 4)
                value += line[i];
            if(numOfCommas == 4)
            {
                value = line.substr(i,line.length());
                backLeft.push_back(stod(value));

                value = "";
            }
            else
            {    
                numOfCommas++;
                switch(numOfCommas)
                {
                    case 1: 
                        time.push_back(stod(value));
                        break;
                    case 2:
                        frontLeft.push_back(stod(value));
                        break;
                    case 3:
                        frontRight.push_back(stod(value));
                        break;
                    case 4:
                        backLeft.push_back(stod(value));
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



    //outfile.close();
    inputFile.close();


    return 0;
}

