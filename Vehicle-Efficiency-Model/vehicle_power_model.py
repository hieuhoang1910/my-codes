mass = 1500
g = 9.81
Crr = 0.015
Cd = 0.3
A = 2.2
rho = 1.225
v = 25

F_rr = Crr * mass * g
F_drag = 0.5 * rho * A * Cd * v**2
F_total = F_rr + F_drag

P = F_total * v
P_kw = P / 1000

print(f"Rolling Resistance Force: {F_rr:.2f} N")
print(f"Aerodynamic Drag Force: {F_drag:.2f} N")
print(f"Total Power Required: {P_kw:.2f} kW")
