import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("makemap/map006.txt", dtype="int")
plt.imshow(data, cmap="Reds")
plt.colorbar()
plt.show()