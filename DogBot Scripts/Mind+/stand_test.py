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

sendSkillStr('krest', .5)

rotateJoints('I', [BRK, 50, BLK, 50, FRK, 40, FLK, 40, FRA, 30, FLA, 30, BRA, 30, BLA, 30], 0)

time.sleep(5)
closePort()