import matplotlib.pyplot as plt

# Create figure and axis
fig, ax = plt.subplots(figsize=(8, 3))

# Define wire dimensions
wire_length = 30  # cm
wire_diameter = 0.2  # cm (2 mm)

# Draw the wire (horizontal line)
ax.plot([0, wire_length], [0, 0], color="saddlebrown", lw=4, label="Copper Wire")

# Draw boundary conditions
ax.text(0, 0.5, r"$T(x=0) = 25^\circ C$", fontsize=12, color='blue')
ax.text(wire_length, 0.5, r"$T(x=L) = 25^\circ C$", fontsize=12, color='blue')

# Indicate wire diameter
ax.plot([wire_length / 2, wire_length / 2], [-0.5, 0.5], 'k--', lw=1)
ax.text(wire_length / 2 + 1, 0.2, r"$d = 2 mm$", fontsize=12)

# Indicate wire length
ax.plot([0, wire_length], [-1, -1], 'k-', lw=1)
ax.text(wire_length / 2, -1.5, r"$L = 30 cm$", fontsize=12, ha='center')

# Add convective heat transfer arrows
for x in range(5, wire_length, 5):
    ax.arrow(x, -0.3, 0, -0.5, head_width=0.3, head_length=0.3, fc='blue', ec='blue')

ax.text(wire_length / 2, -2, r"$T_{\infty} = 5^\circ C, h = 30 W/m^2K$", fontsize=12, color='blue', ha='center')

# Labels
ax.set_xlim(-5, wire_length + 5)
ax.set_ylim(-3, 3)
ax.set_xticks([])
ax.set_yticks([])
ax.axis('off')

# Show plot
plt.show()
