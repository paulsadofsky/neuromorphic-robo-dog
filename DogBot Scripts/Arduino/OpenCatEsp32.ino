// modify the model and board definitions
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

// enter XQ in the serial monitor to activate the following section
#ifdef QUICK_DEMO
void quickDemo() {

  // Make the robot stand up
  tQueue->addTask(T_SKILL, "up", 5000);

  String clock = "_736_1001_202_1001_169_1001_165_1001_162_1001_161_1001_163_1001_189_1001_237_1001_633_0110_185_0110_154_0110_154_0110_152_0110_153_0110_156_0110_183_0110_245_0110_1442_1001_203_1001_170_1001_165_1001_163_1001_161_1001_161_1001_182_1001_226_1001_1220_0110_179_0110_153_0110_153_0110_152_0110_160_0110_195_0110_279_0110_401_";
  int zeroActivation[100];
  int activation[100][4];
  int indexCounter = 0;
  
  for (int i = 0; i < clock.length(); i+=9) {
    zeroActivation[indexCounter] = clock.substring(i+1,i+4).toInt();
    activation[indexCounter][0] = clock.substring(i+5,i+6).toInt();
    activation[indexCounter][1] = clock.substring(i+6,i+7).toInt();
    activation[indexCounter][2] = clock.substring(i+7,i+8).toInt();
    activation[indexCounter][3] = clock.substring(i+8,i+9).toInt();
    indexCounter++;
  }
  zeroActivation[indexCounter] = clock.substring(clock.length()-4, clock.length()-1).toInt();

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

  int zeroCounter = 0;
  int neuronAngleCount[4] = {0,0,0,0};
  String tempArgs = "";
  
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < indexCounter; j++) {
      for (int k = 0; k < zeroActivation[j]; k++) {
        // Do nothing (waste clock cycles)
      }
      
      if (activation[j][0] == 1) {  // FL neuron activation
        for (int k = 0; k < 3; k++) {
          tempArgs = "8, " + String(motorAngles[neuronAngleCount[0]][0]) + ", 9, " + String(motorAngles[neuronAngleCount[0]][1]);
          tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
          neuronAngleCount[0]++;
        }
      }
      
      if (activation[j][1] == 1) {  // FR neuron activation
        for (int k = 0; k < 3; k++) {
          tempArgs = "10, " + String(motorAngles[neuronAngleCount[1]][2]) + ", 11, " + String(motorAngles[neuronAngleCount[1]][3]);
          tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
          neuronAngleCount[1]++;
        }
      }
      
      if (activation[j][2] == 1) {  // BL neuron activation
        for (int k = 0; k < 3; k++) {
          tempArgs = "12, " + String(motorAngles[neuronAngleCount[2]][4]) + ", 13, " + String(motorAngles[neuronAngleCount[2]][5]);
          tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
          neuronAngleCount[2]++;
        }
      }
      
      if (activation[j][3] == 1) {  // BR neuron activation
        for (int k = 0; k < 3; k++) {
          tempArgs = "14, " + String(motorAngles[neuronAngleCount[3]][6]) + ", 15, " + String(motorAngles[neuronAngleCount[3]][7]);
          tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, tempArgs.c_str(), 0);
          neuronAngleCount[3]++;
        }
      }

      for (int k = 0; k < 4; k++) {
        if(neuronAngleCount[k] == 27) {
          neuronAngleCount[k] = 0;
        }
      }
    }
  }
}
#endif
