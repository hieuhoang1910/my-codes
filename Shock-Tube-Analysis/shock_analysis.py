import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

sensitivity = 10.27
ambient_pressure = 14.6959

filename = "78.csv"
data = pd.read_csv(filename)
time = data.iloc[:, 0] / 1000
volt1 = data.iloc[:, 1]
volt2 = data.iloc[:, 2]

P1 = volt1 / sensitivity
P2 = volt2 / sensitivity

window = 5
P1_smooth = P1.rolling(window=window).mean()
P2_smooth = P2.rolling(window=window).mean()

plt.plot(time, P1_smooth, label="Sensor 1")
plt.plot(time, P2_smooth, label="Sensor 2")
plt.xlabel("Time (s)")
plt.ylabel("Pressure (psi)")
plt.title("Shock Tube Pressure Data")
plt.legend()
plt.grid(True)
plt.show()
