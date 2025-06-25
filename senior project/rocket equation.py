# --- Thruster Exit Velocity Calculator ---
# Based on vacuum thrust measurement

# Given thrust in N (from vacuum test)
thrust_N = 0.01876  # 18.76 mN

# Estimated mass flow rate (in kg/s)
# Update this based on your test
mass_flow_rate = 0.00039751  # example value

# Calculate exit velocity using rocket equation
exit_velocity = thrust_N / mass_flow_rate

# Output result
print("=== Thruster Exit Velocity ===")
print(f"Thrust: {thrust_N:.5f} N")
print(f"Mass Flow Rate: {mass_flow_rate:.8f} kg/s")
print(f"Exit Velocity: {exit_velocity:.2f} m/s")

import math

# Constants for R-134a
gamma = 1.25                     # specific heat ratio (assumed)
R = 81.5                         # J/kg-K, specific gas constant for R-134a

# Option 1: Given chamber temp, calculate exit velocity
Tc_c = 40                                  # in Celsius
T_c = Tc_c + 273                        # in Kelvin
ve_from_temp = math.sqrt((2 * gamma / (gamma - 1)) * R * T_c)

# Option 2: Given desired velocity, calculate required temp
desired_ve = 500                 # m/s
T_required = (desired_ve**2 * (gamma - 1)) / (2 * gamma * R) - 273

# Output
print("=== Rocket Nozzle Velocity Calculator ===")
print(f"Given T_c = {T_c} K --> Exit Velocity = {ve_from_temp:.2f} m/s")
print(f"To reach v_e = {desired_ve} m/s --> Required T_c = {T_required:.2f} C")
