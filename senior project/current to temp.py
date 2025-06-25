import numpy as np
import matplotlib.pyplot as plt

# === Problem Setup ===

# Wire and environment properties
L_cm = 5                     # Wire length in cm
D_mm = 1.0                  # Diameter in mm

# --- Choose Material ---
# Nichrome
k = 11.3
rho_e = 1.10e-6

# Constantan
#k = 22
#rho_e = 4.9e-7

# Stainless Steel 304 (optional)
#k = 16
#rho_e = 7e-7

h = 10                       # Convective heat transfer coefficient (W/m²·K)
T_air = 25                   # Ambient temperature (°C)
T_ends = 60                  # Fixed temp at both ends (°C)
I_input = 1.5                # Input current (A) — now FIXED

# === Geometry conversion ===
L = L_cm / 100               # Length in meters
D = D_mm / 1000              # Diameter in meters
r = D / 2
A_c = np.pi * r**2           # Cross-sectional area
P = 2 * np.pi * r            # Perimeter

# === Calculate resistance and voltage ===
R_wire = rho_e * L / A_c     # Resistance from geometry
V_required = I_input * R_wire  # Voltage needed to drive that current
P_total = V_required * I_input  # Total power

print(f"Fixed Current: {I_input:.2f} A")
print(f"Required Voltage: {V_required:.3f} V")
print(f"Total Power Dissipated: {P_total:.3f} W")

# === Analytical solution setup ===
m_sq = h * P / (k * A_c)
m = np.sqrt(m_sq)

def solve_constants(I):
    q = (I ** 2 * rho_e) / A_c           # Heat generation per volume
    theta_p = q / (h * P)                # Particular solution
    A = np.array([
        [1, 1],  # At x = 0
        [np.exp(m * L), np.exp(-m * L)]  # At x = L
    ])
    B = np.array([
        T_ends - (T_air + theta_p),
        T_ends - (T_air + theta_p)
    ])
    C1, C2 = np.linalg.solve(A, B)
    return C1, C2, theta_p

def analytical_solution(I):
    C1, C2, theta_p = solve_constants(I)
    x_vals_cm = np.linspace(0, L_cm, 100)
    x_vals_m = x_vals_cm / 100
    T_x = (T_air + theta_p) + C1 * np.exp(m * x_vals_m) + C2 * np.exp(-m * x_vals_m)
    return x_vals_cm, T_x, C1, C2, theta_p

# === Solve and Plot ===
x_vals_cm, T_vals, C1, C2, theta_p = analytical_solution(I_input)

print(f"\n=== Temperature Profile at {I_input} A ===")
print(f"C1 = {C1:.6f}")
print(f"C2 = {C2:.6f}")
print(f"Theta_p = {theta_p:.6f}")
print(f"Max Temperature: {np.max(T_vals):.2f} °C")

# === Plot the result ===
plt.figure(figsize=(8, 5))
plt.plot(x_vals_cm, T_vals, label=f'I = {I_input:.2f} A, V ≈ {V_required:.2f} V')
plt.xlabel("Position along the wire (cm)")
plt.ylabel("Temperature (°C)")
plt.title("Temperature Distribution Along Wire (Constant Current)")
plt.grid()
plt.legend()
plt.tight_layout()
plt.show()
