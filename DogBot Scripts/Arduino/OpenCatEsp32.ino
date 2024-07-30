// THE BEGINNING IS ALL PROVIDED STARTED CODE
// DOWN BELOW IS THE ACTUAL FUNCTION

//***********************
#define BITTLE  // Petoi 9 DOF robot dog: 1 on head + 8 on leg
// #define NYBBLE // Petoi 11 DOF robot cat: 2 on head + 1 on tail + 8 on leg
// #define CUB

// #define BiBoard_V0_1  //ESP32 Board with 12 channels of built-in PWM for joints
#define BiBoard_V0_2
// #define BiBoard_V1_0
// #define BiBoard2  //ESP32 Board with 16 channels of PCA9685 PWM for joints
//***********************

// Send '!' token to reset the birthmark in the EEPROM so that the robot will restart to reset
// #define AUTO_INIT  //activate it to automatically reset joint and imu calibration without prompts

// you can also activate the following modes (they will diable the gyro to save programming space)
// allowed combinations: RANDOM_MIND + ULTRASONIC, RANDOM_MIND, ULTRASONIC, VOICE, CAMERA
#define VOICE                     // Petoi Grove voice module
#define ULTRASONIC                // for Petoi RGB ultrasonic distance sensor
#define PIR                       // for PIR (Passive Infrared) sensor
#define DOUBLE_TOUCH              // for double touch sensor
#define DOUBLE_LIGHT              // for double light sensor
#define DOUBLE_INFRARED_DISTANCE  // for double distance sensor
#define GESTURE                   // for Gesture module
#define CAMERA                    // for Mu Vision camera
#define QUICK_DEMO                // for quick demo
#include "src/OpenCat.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // USB serial
  Serial.setTimeout(SERIAL_TIMEOUT);
  while (Serial.available() && Serial.read())
    ;  // empty buffer
  initRobot();
  quickDemo();
}

void loop() {
#ifdef VOLTAGE
  lowBattery();
#endif
  //  //— read environment sensors (low level)
  readEnvironment();
  dealWithExceptions();  // low battery, fall over, lifted, etc.
  if (!tQueue->cleared()) {
    tQueue->popTask();
  } else {
    readSignal();
  }
#ifdef NEOPIXEL_PIN
  playLight();
#endif
  reaction();
}

//---------------------------------------------------------------//
//                         EXECUTED CODE                         //
//---------------------------------------------------------------//

#ifdef QUICK_DEMO
void quickDemo() {

  // Make the robot stand up
  tQueue->addTask(T_SKILL, "up", 1000);

  // Initializes variables for the cycles of zero activations and the neuron activations
  // COPY IN OUTPUT OF PARSING SCRIPT HERE
  int indexCounter = 36;
  int zeroActivation[indexCounter] = {202,169,165,162,161,163,189,237,633,185,154,154,152,153,156,183,245,1442,203,170,165,163,161,161,182,226,1220,179,153,153,152,160,195,279,279};
  int activation[indexCounter-1][4] = {{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{1,0,0,1,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,},{0,1,1,0,}};
  
  // Motor angles from the original open source code
  int motorAngles[27][8] = {{10, 29, 51, 46, 33, 30, 4,  15},
                            {9,  31, 48, 47, 41, 29, 3,  16},
                            {10, 31, 43, 48, 40, 29, 3,  16},
                            {14, 33, 38, 48, 36, 30, 6,  17},
                            {15, 35, 34, 49, 35, 30, 13, 18},
                            {17, 38, 35, 51, 34, 28, 16, 18},
                            {19, 44, 36, 52, 33, 17, 14, 19},
                            {20, 42, 38, 52, 32, 16, 14, 20},
                            {22, 38, 39, 53, 31, 16, 14, 22},
                            {24, 35, 41, 59, 31, 16, 14, 16},
                            {25, 30, 42, 61, 30, 17, 14, 11},
                            {27, 26, 43, 59, 30, 19, 15, 8 },
                            {28, 21, 44, 56, 30, 21, 15, 6 },
                            {29, 15, 46, 52, 30, 27, 15, 5 },
                            {31, 8,  47, 49, 29, 40, 16, 3 },
                            {32, 10, 48, 45, 29, 40, 16, 3 },
                            {33, 13, 48, 41, 30, 37, 17, 3 },
                            {35, 15, 49, 35, 30, 36, 18, 10},
                            {38, 16, 51, 33, 28, 34, 18, 17},
                            {44, 19, 52, 35, 17, 33, 19, 16},
                            {42, 20, 52, 37, 16, 32, 20, 14},
                            {38, 21, 53, 39, 16, 32, 22, 14},
                            {35, 23, 59, 40, 16, 31, 16, 14},
                            {30, 25, 61, 42, 17, 30, 11, 14},
                            {26, 26, 59, 42, 19, 30, 8,  14},
                            {21, 28, 56, 44, 21, 30, 6,  15},
                            {15, 28, 52, 45, 27, 30, 5,  15}};

  // Four integers to track which step in the walk cycle each neuron is at
  int neuronAngleCount[4] = {0,0,0,0};
  String tempArgs = "";

  // Repeats walk cycle ten times
  for (int i = 0; i < 10; i++) {
    // Iterates through each zero activation and activation
    for (int j = 0; j < indexCounter; j++) {
      // Delay the program for the zero activation time cycles
//      for (int k = 0; k < zeroActivation[j]; k++) {
//        // Do nothing (waste clock cycles)
//        delay(1);
//      }

      // ------------------------------ FL neuron activation (motor 8/12)
      
      if (activation[j][0] == 1) {
        tempArgs = "8, " + String(motorAngles[neuronAngleCount[0]][0]) + ", 12, " + String(motorAngles[neuronAngleCount[0]][4]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[0]++;

        tempArgs = "8, " + String(motorAngles[neuronAngleCount[0]][0]) + ", 12, " + String(motorAngles[neuronAngleCount[0]][4]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[0]++;
        
        tempArgs = "8, " + String(motorAngles[neuronAngleCount[0]][0]) + ", 12, " + String(motorAngles[neuronAngleCount[0]][4]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[0]++;
      }

      // ------------------------------ FR neuron activation (motor 9/13)
      
      if (activation[j][1] == 1) {
        tempArgs = "9, " + String(motorAngles[neuronAngleCount[1]][1]) + ", 13, " + String(motorAngles[neuronAngleCount[1]][5]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[1]++;

        tempArgs = "9, " + String(motorAngles[neuronAngleCount[1]][1]) + ", 13, " + String(motorAngles[neuronAngleCount[1]][5]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[1]++;

        tempArgs = "9, " + String(motorAngles[neuronAngleCount[1]][1]) + ", 13, " + String(motorAngles[neuronAngleCount[1]][5]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[1]++;
      }

      // ------------------------------ BL neuron activation (motor 11/15)
      
      if (activation[j][2] == 1) {
        tempArgs = "11, " + String(motorAngles[neuronAngleCount[2]][3]) + ", 15, " + String(motorAngles[neuronAngleCount[2]][7]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[2]++;

        tempArgs = "11, " + String(motorAngles[neuronAngleCount[2]][3]) + ", 15, " + String(motorAngles[neuronAngleCount[2]][7]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[2]++;

        tempArgs = "11, " + String(motorAngles[neuronAngleCount[2]][3]) + ", 15, " + String(motorAngles[neuronAngleCount[2]][7]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[2]++;
      }

      // ------------------------------ BR neuron activation (motor 10/14)
      
      if (activation[j][3] == 1) {
        tempArgs = "10, " + String(motorAngles[neuronAngleCount[3]][2]) + ", 14, " + String(motorAngles[neuronAngleCount[3]][6]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[3]++;

        tempArgs = "10, " + String(motorAngles[neuronAngleCount[3]][2]) + ", 14, " + String(motorAngles[neuronAngleCount[3]][6]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[3]++;

        tempArgs = "10, " + String(motorAngles[neuronAngleCount[3]][2]) + ", 14, " + String(motorAngles[neuronAngleCount[3]][6]);
        tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
        neuronAngleCount[3]++;
      }

      // Resets the counters to 0 if index 26 (step 27) is reached

      if(neuronAngleCount[0] == 26) { neuronAngleCount[0] = 0; }
      if(neuronAngleCount[1] == 26) { neuronAngleCount[1] = 0; }
      if(neuronAngleCount[2] == 26) { neuronAngleCount[2] = 0; }
      if(neuronAngleCount[3] == 26) { neuronAngleCount[3] = 0; }
    }
  }
}
#endif
