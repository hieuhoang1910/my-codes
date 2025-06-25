import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Given problem parameters
L = 30  # Length of the wire in cm
D = 0.2  # Diameter of the wire in cm
k = 401  # Thermal conductivity of copper in W/m·K
h = 30  # Convective heat transfer coefficient in W/m²·K
T_air = 5  # Ambient temperature in °C
T_ends = 25  # Fixed temperature at the ends in °C
rho_e = 1.68e-8  # Electrical resistivity of copper (Ω·m)

# Electrical current value for numerical and analytical methods
I = 55  # Amperes

# Convert cm to m for calculations
L_m = L / 100  # Convert length to meters
D_m = D / 100  # Convert diameter to meters

# Compute cross-sectional area and perimeter
A_c = np.pi * (D_m / 2) ** 2  # Cross-sectional area (m²)
P = np.pi * D_m  # Perimeter (m)

# Define node cases (N values)
node_cases = [3, 5, 7, 11]  # Number of nodes

# Function to solve for temperatures using finite difference method
def finite_difference_solution(N):
    dx = L / (N - 1)  # Node spacing in cm

    # Compute alpha term
    alpha = (h * P / (k * A_c)) * (dx / 100) ** 2

    # Compute Joule heating term
    joule_heating = (I**2 * rho_e) / (k * A_c**2) * (dx / 100) ** 2

    # Initialize coefficient matrix A and right-hand side vector b
    A = np.zeros((N, N))
    b = np.zeros(N)

    # Apply boundary conditions
    A[0, 0] = 1
    b[0] = T_ends
    A[-1, -1] = 1
    b[-1] = T_ends

    # Fill interior node equations
    for i in range(1, N - 1):
        A[i, i - 1] = 1
        A[i, i] = -2 - alpha
        A[i, i + 1] = 1
        b[i] = -alpha * T_air - joule_heating

    # Solve the system of linear equations
    T = np.linalg.solve(A, b)

    # Return node positions in cm and temperature values
    x_values_cm = np.linspace(0, L, N)
    return x_values_cm, T

# Function to solve for temperatures using the analytical method
def analytical_solution(N):
    dx = L / (N - 1)  # Node spacing in cm
    x_values_cm = np.linspace(0, L, N)
    
    # Compute lambda^2 and lambda
    m_sq = h * P / (k * A_c)
    m_ = np.sqrt(m_sq)

    # Solve for C1 and C2 using boundary conditions
    q = (I ** 2 * rho_e) / A_c  # Heat generation per unit volume
    theta_p = q / (h * P)  # Particular solution (steady-state offset)

    A = np.array([
        [1, 1],  # Coefficients for C1 and C2 at x = 0
        [np.exp(m_ * L_m), np.exp(-m_ * L_m)]  # Coefficients at x = L
    ])
    B = np.array([
        T_ends - (T_air + theta_p),  # Boundary condition at x = 0
        T_ends - (T_air + theta_p)   # Boundary condition at x = L
    ])
    
    C1, C2 = np.linalg.solve(A, B)

    # Compute temperature distribution
    T_x = (T_air + theta_p) + C1 * np.exp(m_ * x_values_cm / 100) + C2 * np.exp(-m_ * x_values_cm / 100)
    
    return x_values_cm, T_x

# Loop through each node case and compute both numerical and analytical solutions
for N in node_cases:
    x_vals_cm_num, T_vals_num = finite_difference_solution(N)
    x_vals_cm_ana, T_vals_ana = analytical_solution(N)

    # Store results in a table
    df = pd.DataFrame({
        "Position (cm)": x_vals_cm_num,
        "Numerical T(x) (°C)": T_vals_num,
        "Analytical T(x) (°C)": T_vals_ana,
        "Error (%)": np.abs((T_vals_num - T_vals_ana) / T_vals_ana) * 100
    })

    # Display table
    print(f"\nComparison of Numerical vs. Analytical Solutions for N = {N} Nodes:")
    print(df.to_string(index=False))

    # Plot results for each case
    plt.figure(figsize=(8, 5))
    plt.plot(x_vals_cm_ana, T_vals_ana, label="Analytical", linestyle="dashed", color="black")
    plt.plot(x_vals_cm_num, T_vals_num, marker='o', linestyle='-', label=f"Numerical (N={N})")
    
    plt.xlabel("Position along the wire (cm)")
    plt.ylabel("Temperature (°C)")
    plt.title(f"Temperature Distribution Comparison (N={N})")
    plt.legend()
    plt.grid()
    plt.show()
