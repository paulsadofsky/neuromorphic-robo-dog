# MindPlus
# Python
import time
from PetoiRobot import *

autoConnect()

FRK = 13
FLK = 12
BRK = 14
BLK = 15
FRA = 9
FLA = 8
BRA = 10
BLA = 11

DELAY = 0

def moveVector(val1, val2, val3, val4, val5, val6, val7, val8):
    rotateJoints('I', [FLA, val1, FRA, val2, BRA, val3, BLA, val4, FLK, val5, FRK, val6, BRK, val7, BLK, val8], 0)

sendSkillStr('krest', .5)

rotateJoints('I', [BRK, 50, BLK, 50, FRK, 40, FLK, 40, FRA, 30, FLA, 30, BRA, 30, BLA, 30], DELAY)

for i in range(5):
    moveVector(10, 29, 51, 46, 33, 30, 4, 15)
    moveVector(9, 31, 48, 47, 41, 29, 3, 16)
    moveVector(10,  32,  43,  48,  40,  29,   3,  16)
    moveVector(14,  33,  38,  48,  36,  30,   6,  17)
    moveVector(15,  35,  34,  49,  35,  30,  13,  18)
    moveVector(17,  38,  35,  51,  34,  28,  16,  18)
    moveVector(19,  44,  36,  52,  33,  17,  14,  19)
    moveVector(20,  42,  38,  52,  32,  16,  14,  20)
    moveVector(22,  38,  39,  53,  31,  16,  14,  22)
    moveVector(24,  35,  41,  59,  31,  16,  14,  16)
    moveVector(25,  30,  42,  61,  30,  17,  14,  11)
    moveVector(27,  26,  43,  59,  30,  19,  15,   8)
    moveVector(28,  21,  44,  56,  30,  21,  15,   6)
    moveVector(29,  15,  46,  52,  30,  27,  15,   5)
    moveVector(31,   8,  47,  49,  29,  40,  16,   3)
    moveVector(32,  10,  48,  45,  29,  40,  16,   3)
    moveVector(33,  13,  48,  41,  30,  37,  17,   3)
    moveVector(35,  15,  49,  35,  30,  36,  18,  10)
    moveVector(38,  16,  51,  33,  28,  34,  18,  17)
    moveVector(44,  19,  52,  35,  17,  33,  19,  16)
    moveVector(42,  20,  52,  37,  16,  32,  20,  14)
    moveVector(38,  21,  53,  39,  16,  32,  22,  14)
    moveVector(35,  23,  59,  40,  16,  31,  16,  14)
    moveVector(30,  25,  61,  42,  17,  30,  11,  14)
    moveVector(26,  26,  59,  42,  19,  30,   8,  14)
    moveVector(21,  28,  56,  44,  21,  30,   6,  15)
    moveVector(15,  28,  52,  45,  27,  30,   5,  15)

sendSkillStr('krest', .5)

time.sleep(1)
closePort()