import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('CPG Neural Network/C++ Neuron/Fourth Order/single-neuron-network/membrane-voltage.csv')

time = df['Time']
voltage = df['Voltage']

plt.figure(figsize=(10, 6))
plt.plot(time, voltage)
plt.xlabel('Time')
plt.ylabel('Membrane Voltage (Vmem) [a.u.]')
plt.title('Bio-Mimetic Neuron Simulation')
plt.grid(True)

#plt.annotate('Spiking', xy=(2500, -4), xytext=(2500, -4.5), arrowprops=dict(arrowstyle='<->'))
# plt.annotate('Bursting', xy=(12500, -4), xytext=(12500, -4.5), arrowprops=dict(arrowstyle='<->'))

plt.savefig('CPG Neural Network/C++ Neuron/Fourth Order/single-neuron-network/plot/plot.png')
plt.show()