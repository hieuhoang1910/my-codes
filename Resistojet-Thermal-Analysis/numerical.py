import numpy as np

# Given problem parameters
L = 30  # Length of the wire in cm
D = 0.2  # Diameter of the wire in cm
k = 401  # Thermal conductivity of copper in W/m·K
h = 30  # Convective heat transfer coefficient in W/m²·K
T_air = 5  # Ambient temperature in °C
T_ends = 25  # Fixed temperature at the ends in °C
rho_e = 1.68e-8  # Electrical resistivity of copper (Ω·m)

# Electrical current value for the numerical method
I = 55  # Amperes

# Convert cm to m for calculations
L_m = L / 100  # Convert length to meters
D_m = D / 100  # Convert diameter to meters
dx = L / 2  # Node spacing in cm

# Compute cross-sectional area and perimeter
A_c = np.pi * (D_m / 2) ** 2  # Cross-sectional area (m²)
P = np.pi * D_m  # Perimeter (m)

# Compute non-dimensional convection term
alpha = (h * P / (k * A_c)) * (dx / 100) ** 2

# Compute Joule heating term explicitly using the correct equation
joule_heating = (I**2 * rho_e) / (k * A_c**2) * (dx / 100) ** 2

# Solve for T2 using the corrected finite difference equation
T2 = ((h * P / k / A_c) * (dx / 100) ** 2 * T_air - joule_heating + T_ends + T_ends) / (2 + alpha)

# Print the numerical result
print(f"\nFor I = {I} A (3 nodes case):")
print(f"T2 (Midpoint Temperature) = {T2:.3f} °C")
