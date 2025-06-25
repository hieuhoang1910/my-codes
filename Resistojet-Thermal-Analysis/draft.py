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
markers = ['s', 'o', '^', 'x']  # Markers for scatter plot

# Lists to store center temperatures for Figure 4
T_center_numerical = []
T_center_analytical = []

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
def analytical_solution():
    x_values_cm = np.linspace(0, L, 100)  # Smooth curve for analytical solution
    
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

# Get analytical solution
x_vals_cm_ana, T_vals_ana = analytical_solution()
T_center_analytical.append(np.interp(L/2, x_vals_cm_ana, T_vals_ana))  # Center temp of analytical

# Plot analytical solution
plt.figure(figsize=(8, 5))
plt.plot(x_vals_cm_ana, T_vals_ana, label="Analytical", linestyle="dashed", color="black")

# Loop through each node case, compute both numerical and analytical solutions, and plot them
for idx, N in enumerate(node_cases):
    x_vals_cm_num, T_vals_num = finite_difference_solution(N)

    # Get temperature at the center node
    center_idx = np.argmin(np.abs(x_vals_cm_num - L/2))
    T_center_numerical.append(T_vals_num[center_idx])

    # Store results in a DataFrame for tabular display
    df = pd.DataFrame({
        "Position (cm)": x_vals_cm_num,
        "Numerical T(x) (°C)": T_vals_num,
        "Analytical T(x) (°C)": np.interp(x_vals_cm_num, x_vals_cm_ana, T_vals_ana),
        "Error (%)": np.abs((T_vals_num - np.interp(x_vals_cm_num, x_vals_cm_ana, T_vals_ana)) / np.interp(x_vals_cm_num, x_vals_cm_ana, T_vals_ana)) * 100
    })

    # Display table for each node case
    print(f"\nComparison of Numerical vs. Analytical Solutions for N = {N} Nodes:")
    print(df.to_string(index=False))

    # Scatter plot numerical points
    plt.scatter(x_vals_cm_num, T_vals_num, label=f"Numerical (N={N})", marker=markers[idx], s=50)

# Finalize the first plot (Figure 3)
plt.xlabel("Position along the wire (cm)")
plt.ylabel("Temperature (°C)")
plt.title("Temperature Distribution: Numerical vs. Analytical")
plt.legend()
plt.grid()
plt.show()

# Generate Figure 4: Comparison of Center Temperature vs. Number of Nodes
plt.figure(figsize=(7, 4))
plt.plot(node_cases, T_center_numerical, marker='o', linestyle='-', label="Numerical", color="blue")
plt.axhline(y=T_center_analytical[0], color='black', linestyle='dashed', label="Analytical")

plt.xlabel("Number of Nodes")
plt.ylabel("Temperature at Center (°C)")
#plt.title("Center Temperature vs. Number of Nodes")
plt.xticks(node_cases)  # Ensure correct x-axis values
plt.legend()
plt.grid()
plt.show()
