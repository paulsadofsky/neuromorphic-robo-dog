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

sendSkillStr('krest', .5)

rotateJoints('I', [BRK, 50, BLK, 50, FRK, 40, FLK, 40, FRA, 30, FLA, 30, BRA, 30, BLA, 30], DELAY)

for i in range(5):
    rotateJoints('I', [BLK, 10, BLA, 40], DELAY)
    rotateJoints('I', [BLK, 60, BLA, 0, FLK, 10, FLA, 40], DELAY)
    rotateJoints('I', [FLK, 60, FLA, 0, FRA, 30, FRK, 80, BRA, 30, BRK, 80], DELAY)
    rotateJoints('I', [BLK, 50, BLA, 30, FLK, 40, FLA, 30, FRA, 30, FRK, 40, BRA, 30, BRK, 50], DELAY)

    rotateJoints('I', [BRK, 10, BRA, 40], DELAY)
    rotateJoints('I', [BRK, 60, BRA, 0, FRK, 10], DELAY)
    rotateJoints('I', [FRK, 60, FRA, 0, FLA, 30, FLK, 80, BLA, 30, BLK, 80], DELAY)
    rotateJoints('I', [BRK, 50, BRA, 30, FRK, 40, FRA, 30, FLA, 30, FLK, 40, BLA, 30, BLK, 50], DELAY)

time.sleep(1)
closePort()