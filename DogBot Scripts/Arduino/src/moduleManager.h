#ifdef VOICE
#include "voice.h"
#endif

#ifdef VOICE_LD3320
#include "voiceLD3320.h"
#endif

#ifdef CAMERA
#include "camera.h"
#endif

#ifdef ULTRASONIC
#include "ultrasonic.h"
#endif

#ifdef GESTURE
#include "gesture.h"
#endif

#ifdef PIR
#include "pir.h"
#endif

#ifdef DOUBLE_TOUCH
#include "doubleTouch.h"
#endif

#ifdef DOUBLE_LIGHT
#include "doubleLight.h"
#endif

#ifdef DOUBLE_INFRARED_DISTANCE
#include "doubleInfraredDistance.h"
#endif

#ifdef OTHER_MODULES
#endif

#ifdef ALL_RANDOM
#else
// #define GYRO_PIN 0
#endif

int8_t indexOfModule(char moduleName) {
  for (byte i = 0; i < sizeof(moduleList) / sizeof(char); i++)
    if (moduleName == moduleList[i])
      return i;
  return -1;
}
bool moduleActivatedQfunction(char moduleCode) {
  return moduleActivatedQ[indexOfModule(moduleCode)];
}

int8_t activeModuleIdx() {
  for (byte i = 0; i < sizeof(moduleList) / sizeof(char); i++)
    if (moduleActivatedQ[i])
      return i;
  return -1;
}

void initModule(char moduleCode) {
  switch (moduleCode) {
    case EXTENSION_GROVE_SERIAL:
      {
#ifdef BiBoard_V1_0
        Serial2.begin(115200, SERIAL_8N1, 9, 10);
#else
        Serial2.begin(115200, SERIAL_8N1, UART_RX2, UART_TX2);
#endif
        Serial2.setTimeout(SERIAL_TIMEOUT);
        PTL("Start Serial 2");
        break;
      }
#ifdef VOICE
    case EXTENSION_VOICE:
      {
        voiceSetup();
        break;
      }
#endif
#ifdef ULTRASONIC
    case EXTENSION_ULTRASONIC:
      {
        loadBySkillName("sit");
        rgbUltrasonicSetup();
        break;
      }
#endif
#ifdef DOUBLE_TOUCH
    case EXTENSION_DOUBLE_TOUCH:
      {
        loadBySkillName("sit");
        touchSetup();
        break;
      }
#endif
#ifdef DOUBLE_LIGHT
    case EXTENSION_DOUBLE_LIGHT:
      {
        loadBySkillName("sit");
        doubleLightSetup();
        break;
      }
#endif
#ifdef DOUBLE_IR_DISTANCE
    case EXTENSION_DOUBLE_IR_DISTANCE:
      {
        loadBySkillName("sit");
        doubleInfraredDistanceSetup();
        break;
      }
#endif
#ifdef PIR
    case EXTENSION_PIR:
      {
        pirSetup();
        break;
      }
#endif
#ifdef GESTURE
    case EXTENSION_GESTURE:
      {
        loadBySkillName("sit");
        gestureSetup();
        break;
      }
#endif
#ifdef CAMERA
    case EXTENSION_CAMERA:
      {
        loadBySkillName("sit");
        if (!cameraSetup()) {
          int i = indexOfModule(moduleCode);
          PTH("- disable", moduleNames[i]);
          moduleActivatedQ[i] = false;
          i2c_eeprom_write_byte(EEPROM_MODULE_ENABLED_LIST + i, false);
        }
        break;
      }
#endif
#ifdef QUICK_DEMO
    case EXTENSION_QUICK_DEMO:
      {
        break;
      }
#endif
  }
}

void stopModule(char moduleCode) {
  switch (moduleCode) {
    case EXTENSION_GROVE_SERIAL:
      {
        Serial2.end();
        PTL("Stop Serial 2");
        break;
      }
#ifdef VOICE
    case EXTENSION_VOICE:
      {
        voiceStop();
        break;
      }
#endif
#ifdef ULTRASONIC
    case EXTENSION_ULTRASONIC:
      {
        // ultrasonicStop();   // Todo
        break;
      }
#endif
#ifdef DOUBLE_TOUCH
    case EXTENSION_DOUBLE_TOUCH:
      {
        break;
      }
#endif
#ifdef DOUBLE_LIGHT
    case EXTENSION_DOUBLE_LIGHT:
      {
        break;
      }
#endif
#ifdef DOUBLE_IR_DISTANCE
    case EXTENSION_DOUBLE_IR_DISTANCE:
      {
        break;
      }
#endif
#ifdef PIR
    case EXTENSION_PIR:
      {
        break;
      }
#endif
#ifdef GESTURE
    case EXTENSION_GESTURE:
      {
        gestureStop();
        break;
      }
#endif
#ifdef CAMERA
    case EXTENSION_CAMERA:
      {
        // cameraStop();   // Todo
        break;
      }
#endif
#ifdef QUICK_DEMO
    case EXTENSION_QUICK_DEMO:
      {
        break;
      }
#endif
  }
}
void showModuleStatus() {
  byte moduleCount = sizeof(moduleList) / sizeof(char);
  printListWithoutString((char *)moduleList, moduleCount);
  printListWithoutString(moduleActivatedQ, moduleCount);
}

void reconfigureTheActiveModule(char *moduleCode) {               // negative number will deactivate all the modules
  for (byte i = 0; i < sizeof(moduleList) / sizeof(char); i++) {  // disable unneeded modules
    if (moduleActivatedQ[i] && moduleList[i] != moduleCode[0]) {
      PTH("- disable", moduleNames[i]);
      stopModule(moduleList[i]);
      moduleActivatedQ[i] = false;
      i2c_eeprom_write_byte(EEPROM_MODULE_ENABLED_LIST + i, false);
    }
  }
  for (byte i = 0; i < sizeof(moduleList) / sizeof(char); i++) {
    if (moduleList[i] == moduleCode[0] && !moduleActivatedQ[i]) {
      PTH("+  enable", moduleNames[i]);
      initModule(moduleList[i]);
      moduleActivatedQ[i] = true;
      i2c_eeprom_write_byte(EEPROM_MODULE_ENABLED_LIST + i, true);
    }
  }
  showModuleStatus();
}

void initModuleManager() {
  byte moduleCount = sizeof(moduleList) / sizeof(char);
  PTL(moduleCount);
  for (byte i = 0; i < moduleCount; i++) {
    if (moduleActivatedQ[i]) {
      initModule(moduleList[i]);
    }
  }
  showModuleStatus();
}

void read_serial() {
  Stream *serialPort = NULL;
// String source;
#ifdef BT_SSP
  if (SerialBT.available()) {  // give BT a higher priority over wired serial
    serialPort = &SerialBT;
    // source = "BT";
  } else
#endif
    if (moduleActivatedQ[0] && Serial2.available()) {
    serialPort = &Serial2;
  } else if (Serial.available()) {
    serialPort = &Serial;
    // source = "SER";
  }
  if (serialPort) {
    token = serialPort->read();
    lowerToken = tolower(token);
    newCmdIdx = 2;
    delay(1);                                                  // leave enough time for serial read
    terminator = (token >= 'A' && token <= 'Z') ? '~' : '\n';  // capitalized tokens use binary encoding for long data commands
                                                               //'~' ASCII code = 126; may introduce bug when the angle is 126 so only use angles <= 125
    serialTimeout = (token == T_SKILL_DATA || lowerToken == T_BEEP) ? SERIAL_TIMEOUT_LONG : SERIAL_TIMEOUT;
    lastSerialTime = millis();
    do {
      if (serialPort->available()) {
        // long current = millis();
        // PTH(source, current - lastSerialTime);
        do {
          if ((token == T_SKILL || lowerToken == T_INDEXED_SIMULTANEOUS_ASC || lowerToken == T_INDEXED_SEQUENTIAL_ASC) && cmdLen >= spaceAfterStoringData || cmdLen > BUFF_LEN) {
            PTH("Cmd Length: ", cmdLen);
            PTF("OVF");
            beep(5, 100, 50, 5);
            do {
              serialPort->read();
            } while (serialPort->available());
            printToAllPorts(token);
            token = T_SKILL;
            strcpy(newCmd, "up");
            cmdLen = 2;
            return;
          }
          newCmd[cmdLen++] = serialPort->read();
        } while (serialPort->available());
        lastSerialTime = millis();
      }
    } while (newCmd[cmdLen - 1] != terminator && long(millis() - lastSerialTime) < serialTimeout);  // the lower case tokens are encoded in ASCII and can be entered in Arduino IDE's serial monitor
                                                                                                    // if the terminator of the command is set to "no line ending" or "new line", parsing can be different
                                                                                                    // so it needs a timeout for the no line ending case
    // PTH("* " + source, long(millis() - lastSerialTime));
    if (!(token >= 'A' && token <= 'Z') || token == 'X' || token == 'R' || token == 'W') {  // serial monitor is used to send lower cased tokens by users
                                                                                            // delete the unexpected '\r' '\n' if the serial monitor sends line ending symbols
      leftTrimSpaces(newCmd);                                                               // allow space between token and parameters, such as "k sit"
      for (int i = cmdLen - 1; i >= 0; i--) {                                               // delete the '/r' and '/n' if the serial monitor is configured to send terminators
        if ((newCmd[i] == '\n') || (newCmd[i] == '\r')) {
          newCmd[i] = '\0';
          cmdLen--;
        }
      }
    }
    cmdLen = (newCmd[cmdLen - 1] == terminator) ? cmdLen - 1 : cmdLen;
    newCmd[cmdLen] = (token >= 'A' && token <= 'Z') ? '~' : '\0';
    newCmdIdx = 2;
    // PTH("read_serial, cmdLen = ", cmdLen);
    // printCmdByType(token, newCmd, cmdLen);
  }
}

void readSignal() {
  moduleIndex = activeModuleIdx();
#ifdef IR_PIN
  read_infrared();  //  newCmdIdx = 1
#endif
  read_serial();  //  newCmdIdx = 2
#ifdef BT_BLE
  detectBle();  //  newCmdIdx = 3;
  readBle();
#endif

#ifdef VOICE
  if (moduleList[moduleIndex] == EXTENSION_VOICE)
    read_voice();
#endif

  long current = millis();
  if (newCmdIdx)
    idleTimer = millis() +
#ifdef DOUBLE_INFRARED_DISTANCE
                0
#else
                IDLE_TIME
#endif
      ;
  else if (token != T_CALIBRATE && token != T_SERVO_FOLLOW && token != T_SERVO_FEEDBACK && current - idleTimer > 0) {
    if (moduleIndex == -1)  // no active module
      return;

#ifdef CAMERA
    if (moduleList[moduleIndex] == EXTENSION_CAMERA)
      read_camera();
#endif
#ifdef ULTRASONIC
    if (moduleList[moduleIndex] == EXTENSION_ULTRASONIC) {
      readRGBultrasonic();
    }
#endif
#ifdef GESTURE
    if (moduleList[moduleIndex] == EXTENSION_GESTURE)
      read_gesture();
#endif
#ifdef PIR
    if (moduleList[moduleIndex] == EXTENSION_PIR)
      read_PIR();
#endif
#ifdef DOUBLE_TOUCH
    if (moduleList[moduleIndex] == EXTENSION_DOUBLE_TOUCH)
      read_doubleTouch();
#endif
#ifdef DOUBLE_LIGHT
    if (moduleList[moduleIndex] == EXTENSION_DOUBLE_LIGHT)
      read_doubleLight();
#endif
#ifdef DOUBLE_INFRARED_DISTANCE
    if (moduleList[moduleIndex] == EXTENSION_DOUBLE_IR_DISTANCE)
      read_doubleInfraredDistance();  // has some bugs
#endif
#ifdef TOUCH0
    read_touch();
#endif
    // powerSaver -> 4
    // other -> 5
    // randomMind -> 100

    if (autoSwitch) {
      randomMind();             // make the robot do random demos
      powerSaver(POWER_SAVER);  // make the robot rest after a certain period, the unit is seconds
    }
  }
}

// — read human sensors (top level) —
void readHuman() {
#ifdef TOUCH0
  read_touch();
#endif
}
// — generate behavior by fusing all sensors and instruction
String decision() {
  return "";
}
