#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string clock = "_736_1001_202_1001_169_1001_165_1001_162_1001_161_1001_163_1001_189_1001_237_1001_633_0110_185_0110_154_0110_154_0110_152_0110_153_0110_156_0110_183_0110_245_0110_1442_1001_203_1001_170_1001_165_1001_163_1001_161_1001_161_1001_182_1001_226_1001_1220_0110_179_0110_153_0110_153_0110_152_0110_160_0110_195_0110_279_0110_401_";
    string tempclock = clock;
    int zeroActivation[100];
    int activation[100][4];
    int indexCounter = 0;

    int startInd, endInd;
    while (tempclock.length() > 2) {
        startInd = tempclock.find('_');
        endInd = tempclock.find('_', 1);
        zeroActivation[indexCounter] = stoi(tempclock.substr(startInd+1, endInd-startInd-1));
        tempclock = tempclock.substr(endInd);

        if (tempclock.length() > 2) {
            startInd = tempclock.find('_');
            endInd = tempclock.find('_', 1);
            activation[indexCounter][0] = stoi(tempclock.substr(startInd+1, 1));
            activation[indexCounter][1] = stoi(tempclock.substr(startInd+2, 1));
            activation[indexCounter][2] = stoi(tempclock.substr(startInd+3, 1));
            activation[indexCounter][3] = stoi(tempclock.substr(startInd+4, 1));
            tempclock = tempclock.substr(endInd);
        }

        indexCounter++;
    }

    for (int i = 0; i < indexCounter; i++) {
        cout << zeroActivation[i] << " | ";
        for (int j = 0; j < 4; j++) {
            cout << activation[i][j];
        }
        cout << endl;
    }

    return 0;
}