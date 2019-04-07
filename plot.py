import numpy as np
import matplotlib.pyplot as plt

data = np.fromfile("log.txt", '<f8')

plt.plot(data)
plt.show()