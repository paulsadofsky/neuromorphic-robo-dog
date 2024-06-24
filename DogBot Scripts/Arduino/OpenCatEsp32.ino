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
// #define ROBOTIC_ARM               // for attaching head clip arm
#include "src/OpenCat.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // USB serial
  Serial.setTimeout(SERIAL_TIMEOUT);
  //  Serial1.begin(115200); //second serial port
  while (Serial.available() && Serial.read())
    ;  // empty buffer
  initRobot();
  quickDemo();
}

void loop() {
#ifdef VOLTAGE
  lowBattery();
#endif
  //  //—self-initiative
  //  if (autoSwitch) { //the switch can be toggled on/off by the 'z' token
  //    randomMind();//allow the robot to auto rest and do random stuff in randomMind.h
  //    powerSaver(POWER_SAVER);//make the robot rest after a certain period, the unit is seconds
  //
  //  }
  //  //— read environment sensors (low level)
  readEnvironment();
  //  //— special behaviors based on sensor events
  dealWithExceptions();  // low battery, fall over, lifted, etc.
  if (!tQueue->cleared()) {
    tQueue->popTask();
  } else {
    readSignal();
//#ifdef QUICK_DEMO
//    if (moduleList[moduleIndex] == EXTENSION_QUICK_DEMO)
//      quickDemo();
//#endif
    //  readHuman();
  }
  //  //— generate behavior by fusing all sensors and instruction
  //  decision();

  //  //— action
  //  //playSound();
#ifdef NEOPIXEL_PIN
  playLight();
#endif
  reaction();
}
//
//#ifdef QUICK_DEMO  // enter XQ in the serial monitor to activate the following section
//int prevReading = 0;
//void quickDemo() {  // this is an example that use the analog input pin ANALOG1 as a touch pad
//  // if the pin is not connected to anything, the reading will be noise
//  int currentReading = analogRead(ANALOG1);
//  if (abs(currentReading - prevReading) > 50)  // if the reading is significantly different from the previous reading
//  {
//    PT("Reading on pin ANALOG1:\t");
//    PTL(currentReading);
//    if (currentReading < 50) {                                        // touch and hold on the A2 pin until the condition is met
//      tQueue->addTask(T_BEEP, "12 4 14 4 16 2");                      // make melody
//      tQueue->addTask(T_INDEXED_SEQUENTIAL_ASC, "0 30 0 -30", 1000);  // move the neck, left shoulder, right shoulder one by one
//    } else if (abs(currentReading - prevReading) < 100) {
//      if (strcmp(lastCmd, "sit"))
//        tQueue->addTask(T_SKILL, "sit", 1000);  // make the robot sit. more tokens are defined in OpenCat.h
//    } else {
//      if (strcmp(lastCmd, "up"))
//        tQueue->addTask(T_SKILL, "up", 1000);  // make the robot stand up. more tokens are defined in OpenCat.h
//    }
//  }
//  prevReading = currentReading;
//}
//#endif

#ifdef QUICK_DEMO  // enter XQ in the serial monitor to activate the following section
void quickDemo() {
  //tQueue->addTask(T_BEEP, "12 4 14 4 16 2");                      // make melody
  //tQueue->addTask(T_SKILL, "sit", 5000);                            // make the robot sit. more tokens are defined in OpenCat.h
  
  tQueue->addTask(T_SKILL, "up", 5000);                             // make the robot stand up. more tokens are defined in OpenCat.h

  for(int i = 0; i<10; i++) {
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 10, 9, 29, 10, 51, 11, 46, 12, 33, 13, 30, 14, 4, 15, 15", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 9, 9, 31, 10, 48, 11, 47, 12, 41, 13, 29, 14, 3, 15, 16", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 10, 9, 32, 10, 43, 11, 48, 12, 40, 13, 29, 14, 3, 15, 16", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 14, 9, 33, 10, 38, 11, 48, 12, 36, 13, 30, 14, 6, 15, 17", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 15, 9, 35, 10, 34, 11, 49, 12, 35, 13, 30, 14, 13, 15, 18", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 17, 9, 38, 10, 35, 11, 51, 12, 34, 13, 28, 14, 16, 15, 18", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 19, 9, 44, 10, 36, 11, 52, 12, 33, 13, 17, 14, 14, 15, 19", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 20, 9, 42, 10, 38, 11, 52, 12, 32, 13, 16, 14, 14, 15, 20", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 22, 9, 38, 10, 39, 11, 53, 12, 31, 13, 16, 14, 14, 15, 22", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 24, 9, 35, 10, 41, 11, 59, 12, 31, 13, 16, 14, 14, 15, 16", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 25, 9, 30, 10, 42, 11, 61, 12, 30, 13, 17, 14, 14, 15, 11", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 27, 9, 26, 10, 43, 11, 59, 12, 30, 13, 19, 14, 15, 15, 8", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 28, 9, 21, 10, 44, 11, 56, 12, 30, 13, 21, 14, 15, 15, 6", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 29, 9, 15, 10, 46, 11, 52, 12, 30, 13, 27, 14, 15, 15, 5", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 31, 9, 8, 10, 47, 11, 49, 12, 29, 13, 40, 14, 16, 15, 3", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 32, 9, 10, 10, 48, 11, 45, 12, 29, 13, 40, 14, 16, 15, 3", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 33, 9, 13, 10, 48, 11, 41, 12, 30, 13, 37, 14, 17, 15, 3", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 35, 9, 15, 10, 49, 11, 35, 12, 30, 13, 36, 14, 18, 15, 10", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 38, 9, 16, 10, 51, 11, 33, 12, 28, 13, 34, 14, 18, 15, 17", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 44, 9, 19, 10, 52, 11, 35, 12, 17, 13, 33, 14, 19, 15, 16", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 42, 9, 20, 10, 52, 11, 37, 12, 16, 13, 32, 14, 20, 15, 14", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 38, 9, 21, 10, 53, 11, 39, 12, 16, 13, 32, 14, 22, 15, 14", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 35, 9, 23, 10, 59, 11, 40, 12, 16, 13, 31, 14, 16, 15, 14", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 30, 9, 25, 10, 61, 11, 42, 12, 17, 13, 30, 14, 11, 15, 14", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 26, 9, 26, 10, 59, 11, 42, 12, 19, 13, 30, 14, 8, 15, 14", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 21, 9, 28, 10, 56, 11, 44, 12, 21, 13, 30, 14, 6, 15, 15", 0);
    tQueue->addTask(T_INDEXED_SIMULTANEOUS_ASC, "8, 15, 9, 28, 10, 52, 11, 45, 12, 27, 13, 30, 14, 5, 15, 15", 0);
  }
}
#endif
