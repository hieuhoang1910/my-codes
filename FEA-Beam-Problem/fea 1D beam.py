import numpy as np

# === PARAMETERS ===
L_total = 10.0        # Length of the beam (m)
E = 210e9             # Young's modulus (Pa)
A = 0.005             # Cross-sectional area (m^2)
num_elements = 4      # Number of elements

# === DERIVED ===
num_nodes = num_elements + 1
node_coords = np.linspace(0, L_total, num_nodes)
element_length = L_total / num_elements

# === GLOBAL STIFFNESS MATRIX ===
K_global = np.zeros((num_nodes, num_nodes))

# === ELEMENT STIFFNESS MATRIX ===
k_local = (E * A / element_length) * np.array([[1, -1], [-1, 1]])

# Assemble global matrix
for i in range(num_elements):
    K_global[i:i+2, i:i+2] += k_local

# === FORCE VECTOR ===
F = np.zeros(num_nodes)
F[-1] = -1000.0  # Apply a -1000 N load at the end node

# === APPLY BOUNDARY CONDITIONS ===
# Fixed at node 0 => displacement = 0
K_reduced = K_global[1:, 1:]
F_reduced = F[1:]

# === SOLVE SYSTEM ===
u_reduced = np.linalg.solve(K_reduced, F_reduced)

# Add back the known zero displacement at the fixed node
u = np.insert(u_reduced, 0, 0.0)

# === OUTPUT ===
print("Nodal Displacements (m):")
for i, ui in enumerate(u):
    print(f"Node {i}: {ui:.6e} m")

# === Reaction force at fixed node ===
reaction = K_global[0, :] @ u
print(f"\nReaction at node 0: {reaction:.2f} N")
