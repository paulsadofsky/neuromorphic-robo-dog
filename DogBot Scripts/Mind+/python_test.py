#  -*- coding: UTF-8 -*-

# MindPlus
# Python
import time
from PetoiRobot import *


autoConnect()
rotateJoints('I', [14, 20, 15, 20, 10, 10, 11, 10], 0.2)
for i in range (10):
    rotateJoints('I', [14, -10, 15, -10], 0)
    rotateJoints('I', [0, 10], 0)
    rotateJoints('I', [14, 10, 15, 10], 0)
    rotateJoints('I', [0, -10], 0)
time.sleep(1)
closePort()