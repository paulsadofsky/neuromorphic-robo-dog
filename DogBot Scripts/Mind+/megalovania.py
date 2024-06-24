#  -*- coding: UTF-8 -*-

# MindPlus
# Python
import time
from PetoiRobot import *


autoConnect()
play('b', absValList(12, 13), 0.05)
play('b', absValList(12, 13), 0.05)
play('b', absValList(24, 5), 0.05)
play('b', absValList(19, 5), 0.05)
play('b', absValList(18, 10), 0.05)
play('b', absValList(17, 10), 0.05)
play('b', absValList(15, 10), 0.05)
play('b', absValList(12, 13), 0.05)
play('b', absValList(15, 13), 0.05)
play('b', absValList(17, 13), 0.05)
time.sleep(1)
closePort()