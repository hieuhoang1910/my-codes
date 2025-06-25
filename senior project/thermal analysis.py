import math

def full_thermal_analysis(
    Ti_C=25, 
    Te_C=40, 
    mdot=0.00039751, 
    cp=842, 
    D=0.0006, 
    L=0.02,
    k_gas=0.014,
    pr=0.91,
    nu=1.44e-6,
    rho=10.55,
    k_steel=13.4,
    k_petg=0.29
):
    # STEP 1: Heat required to raise gas temperature
    Q_dot = mdot * cp * (Te_C - Ti_C)  # W

    # STEP 2: Flow properties and geometry
    A_s = math.pi * D * L                          # Inner pipe surface area (convection)
    A_c = math.pi * (D ** 2) / 4                   # Cross-sectional flow area
    V = mdot / (rho * A_c)                         # Flow velocity

    # STEP 3: Heat transfer coefficient (turbulent, fully developed)
    Re = V * D / nu
    f = (0.79 * math.log(Re) - 1.64) ** -2
    Nu = ((f / 8) * (Re - 1000) * pr) / (1 + 12.7 * (f / 8) ** 0.5 * (pr ** (2 / 3) - 1))
    h = Nu * k_gas / D

    # STEP 4: Find wall temperature using LMTD closed-form
    deltaT_lm = Q_dot / (h * A_s)
    delta_T = Te_C - Ti_C  # Make positive
    exp_term = math.exp(delta_T / deltaT_lm)
    Ts_C = (Te_C * exp_term - Ti_C) / (exp_term - 1)  # °C

    # STEP 5: Conduction from wall to nichrome surface (PETG + steel)
    r_inner = 0.00032            # Inner radius (steel wall, gas side)
    r_steel_outer = 0.00082     # Outer radius of steel / inner radius of PETG
    r_petg_outer = 0.0021       # Outer PETG radius (nichrome contact)
    lr = 0.05

    R_steel = math.log(r_steel_outer / r_inner) / (2 * math.pi * k_steel * lr)
    R_petg = math.log(r_petg_outer / r_steel_outer) / (2 * math.pi * k_petg * lr)
    R_total = R_steel + R_petg

    T_wire_C = 25 + Q_dot * R_total  # Nichrome surface temperature

    # Package results
    results = {
        "Q_dot (W)": Q_dot,
        "Velocity (m/s)": V,
        "Reynolds Number": Re,
        "Nusselt Number": Nu,
        "h (W/m^2·K)": h,
        "Delta T_lm (°C)": deltaT_lm,
        "T_surface_wall_petg (°C)": Ts_C,
        "R_total(K/W)": R_total,
        "T_wire_surface (°C)": T_wire_C
    }

    

    return results


# Run and display
results = full_thermal_analysis()
for key, value in results.items():
    print(f"{key}: {value:.2f}")

    
# === ADDITIONAL ANALYSIS: Electrical current, voltage, and power for nichrome wire ===

# Given nichrome properties
rho_nichrome = 1.10e-6       # Ω·m
h_natural_air = 10           # W/m^2·K
T_ambient_C = 25             # °C
T_surface_C = results["T_wire_surface (°C)"]  # Reuse output from thermal model

# Wire geometry
wire_diameter = 0.001 #0.00065      # meters
wire_length = 0.05           # meters
wire_radius = wire_diameter / 2
A_wire = math.pi * wire_radius**2
P_wire = 2 * math.pi * wire_radius

# Required current to reach surface temp
I_wire = math.sqrt((T_surface_C - T_ambient_C) * h_natural_air * P_wire * A_wire / rho_nichrome)

# Electrical resistance, voltage, power
R_wire = rho_nichrome * wire_length / A_wire
V_wire = I_wire * R_wire
P_wire_total = V_wire * I_wire

# Display
print("\n=== Electrical Requirements to Reach Surface Temp ===")
print(f"Required Current (A): {I_wire:.2f}")
print(f"Wire Resistance (Ω): {R_wire:.4f}")
print(f"Required Voltage (V): {V_wire:.3f}")
print(f"Total Power (W): {P_wire_total:.3f}")