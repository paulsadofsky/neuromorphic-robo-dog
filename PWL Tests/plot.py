import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv('PWL Tests/function-test.csv')

time = df['Time']
tanh_r = df['TANH']
pwl_r = df['PWL']

plt.figure(figsize=(10, 6))
plt.plot(time, tanh_r)
plt.plot(time, pwl_r)
plt.xlabel('Time')
plt.grid(True)

plt.savefig('PWL Tests/plot.png')
plt.show()