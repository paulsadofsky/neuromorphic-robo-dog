import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('FPGA/lif-neuron/Output Tests/voltage.csv')

time = df['Time']
voltage = df['Voltage']

plt.figure(figsize=(10, 6))
plt.plot(time, voltage)
plt.xlabel('Time')
plt.grid(True)

plt.savefig('FPGA/lif-neuron/Output Tests/plot.png')
plt.show()