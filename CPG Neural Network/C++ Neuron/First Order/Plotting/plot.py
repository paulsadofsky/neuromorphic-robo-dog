import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('Neuron Tests/First Order/voltage.csv')

time = df['Time']
voltage = df['Voltage']

plt.figure(figsize=(10, 6))
plt.plot(time, voltage)
plt.xlabel('Time')
plt.grid(True)

plt.savefig('Neuron Tests/First Order/Plotting/plot.png')
plt.show()