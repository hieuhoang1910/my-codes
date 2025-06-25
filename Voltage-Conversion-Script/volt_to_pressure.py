import pandas as pd

sensitivity = 10.27

data = pd.read_csv("sensor_data.csv")
voltage = data["Voltage (mV)"]

pressure = voltage / sensitivity

data["Pressure (psi)"] = pressure
data.to_csv("converted_pressure.csv", index=False)

print("Conversion complete. Output saved to 'converted_pressure.csv'")
