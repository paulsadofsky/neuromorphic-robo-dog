import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('Neuron Tests/Fourth Order/four-neuron-network-weighted/network-membrane-voltage.csv')

# Gets each column of data from the CSV for each voltage at each timestep
time = df['Time']
voltageFL = df['VoltageFL']
voltageFR = df['VoltageFR']
voltageBL = df['VoltageBL']
voltageBR = df['VoltageBR']

# Creates large figure to fit all plots
fig = plt.figure(figsize=(20, 18))

plt.title('Bio-Mimetic Neuron Simulation in C++')

# Plot each voltage for the front left and front right
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

plt.savefig('Neuron Tests/Fourth Order/four-neuron-network-weighted/plot/network-plot.png')