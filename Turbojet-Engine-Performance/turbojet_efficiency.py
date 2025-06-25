P1, T1 = 101.3, 288.15
P2, T2 = 300, 500
P3, T3 = 290, 1100
P4, T4 = 120, 800

gamma = 1.4
cp = 1005

isentropic_T2 = T1 * (P2/P1)**((gamma-1)/gamma)
eta_compressor = (isentropic_T2 - T1) / (T2 - T1)

isentropic_T4 = T3 * (P4/P3)**((gamma-1)/gamma)
eta_turbine = (T3 - T4) / (T3 - isentropic_T4)

eta_overall = eta_turbine * eta_compressor

print(f"Compressor Efficiency: {eta_compressor:.3f}")
print(f"Turbine Efficiency: {eta_turbine:.3f}")
print(f"Overall Efficiency: {eta_overall:.3f}")
