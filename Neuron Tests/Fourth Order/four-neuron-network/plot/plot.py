import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('Neuron Tests/Fourth Order/network-membrane-voltage.csv')

time = df['Time']
voltageFL = df['VoltageFL']
voltageFR = df['VoltageFR']
voltageBL = df['VoltageBL']
voltageBR = df['VoltageBR']

fig = plt.figure(figsize=(30, 24))

plt.title('Bio-Mimetic Neuron Simulation in C++')

plt.subplot(4, 1, 1)
plt.plot(time, voltageFL)
plt.xlabel('Time')
plt.ylabel('Membrane Voltage (FRONT LEFT)')
plt.grid(True)

plt.subplot(4, 1, 2)
plt.plot(time, voltageFR)
plt.xlabel('Time')
plt.ylabel('Membrane Voltage (FRONT RIGHT)')
plt.grid(True)

plt.subplot(4, 1, 3)
plt.plot(time, voltageBL)
plt.xlabel('Time')
plt.ylabel('Membrane Voltage (BACK LEFT)')
plt.grid(True)

plt.subplot(4, 1, 4)
plt.plot(time, voltageBR)
plt.xlabel('Time')
plt.ylabel('Membrane Voltage (BACK RIGHT)')
plt.grid(True)

plt.savefig('network-plot.png')
plt.show()