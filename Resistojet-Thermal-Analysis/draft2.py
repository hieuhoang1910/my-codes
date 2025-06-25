import numpy as np

# Given problem parameters
L = 30  # Length of the wire in centimeters (30 cm)
D = 0.2  # Diameter of the wire in centimeters (2 mm)
k = 401  # Thermal conductivity of copper in W/m·K
h = 30  # Convective heat transfer coefficient in W/m²·K
T_air = 5  # Ambient temperature in °C
T_ends = 25  # Fixed temperature at the ends in °C
rho = 1.68e-8  # Electrical resistivity of copper (Ω·m)

# Electrical current values to consider
currents = [0, 40, 55]  # Amperes

# Convert cm to m for calculations
L_m = L / 100  # Convert length to meters
D_m = D / 100  # Convert diameter to meters

# Compute cross-sectional area and perimeter
A_c = np.pi * (D_m / 2) ** 2  # Cross-sectional area (m²)
P = np.pi * D_m  # Perimeter (m)

# Compute m^2 and m
m_sq = h * P / (k * A_c)
m_ = np.sqrt(m_sq)

# Function to solve for C1 and C2 using boundary conditions
def solve_constants(I):
    q = (I ** 2 * rho) / A_c  # Heat generation per unit volume
    theta_p = q / (h * P)  # Particular solution (steady-state offset)
    
    # Solve for C1 and C2 using boundary conditions T(0) = 25, T(L) = 25
    A = np.array([
        [1, 1],  # Coefficients for C1 and C2 at x = 0
        [np.exp(m_ * L_m), np.exp(-m_ * L_m)]  # Coefficients at x = L
    ])
    B = np.array([
        T_ends - (T_air + theta_p),  # Boundary condition at x = 0
        T_ends - (T_air + theta_p)   # Boundary condition at x = L
    ])

    C1, C2 = np.linalg.solve(A, B)
    
    return C1, C2, theta_p

# Define the function to generate the T(x) equation as a formatted string
def generate_temperature_equation(I):
    C1, C2, theta_p = solve_constants(I)
    equation = f"T(x) = ({T_air + theta_p:.3f}) + ({C1:.3f}) * exp({m_:.5f} * x) + ({C2:.3f}) * exp(-{m_:.5f} * x)"
    return equation

# Generate and print the equations for each current
for I in currents:
    eq = generate_temperature_equation(I)
    print(f"\nFor I = {I} A:")
    print(eq)
