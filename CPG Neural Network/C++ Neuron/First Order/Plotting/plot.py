import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('CPG Neural Network/C++ Neuron/First Order/voltage.csv')

time = df['Time']
voltage = df['Voltage']

plt.figure(figsize=(10, 6))
plt.plot(time, voltage)
plt.xlabel('Time')
plt.grid(True)

plt.savefig('CPG Neural Network/C++ Neuron/First Order/Plotting/plot.png')
plt.show()