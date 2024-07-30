import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('CPG Neural Network/C++ Neuron/Fourth Order/two-neuron-network/network-membrane-voltage.csv')

# Gets each column of data from the CSV for each voltage at each timestep
time = df['Time']
voltageFL = df['VoltageFL']
voltageFR = df['VoltageFR']


# Creates large figure to fit all plots
fig = plt.figure(figsize=(20, 18))

plt.title('Bio-Mimetic Neuron Simulation in C++')

# Plot each voltage for the front left and front right
plt.subplot(2, 1, 1)
plt.plot(time, voltageFL)
plt.xlabel('Time')
plt.ylabel('Membrane Voltage (FRONT LEFT)')
plt.grid(True)

plt.subplot(2, 1, 2)
plt.plot(time, voltageFR)
plt.xlabel('Time')
plt.ylabel('Membrane Voltage (FRONT RIGHT)')
plt.grid(True)

plt.savefig('CPG Neural Network/C++ Neuron/Fourth Order/two-neuron-network/plot/network-plot.png')