import numpy as np
import matplotlib.pyplot as plt

N = 10000
mu_x = 10
sigma_x = 1

x_samples = np.random.normal(mu_x, sigma_x, N)

def response(x):
    return 2*x**2 + 3*x + 5

y_samples = response(x_samples)

mean_y = np.mean(y_samples)
std_y = np.std(y_samples)

print(f"Mean Response: {mean_y:.2f}")
print(f"Std Deviation: {std_y:.2f}")

plt.hist(y_samples, bins=50, alpha=0.7)
plt.title("Monte Carlo RSM Output Distribution")
plt.xlabel("Response")
plt.ylabel("Frequency")
plt.grid(True)
plt.show()
