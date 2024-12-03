from matplotlib import pyplot as plt
import numpy as np
from scipy.stats import norm
import math

count, pi, sigma = np.loadtxt('l2/zad3.txt', unpack=True)
plt.errorbar(count, pi, sigma, fmt="ro")
plt.plot([count[0], count[-1]], [math.pi, math.pi], "k:")
plt.xscale('log')
plt.xlabel("próby")
plt.ylabel("y")
plt.legend(["wartość pi", "aproksymacja pi"])
plt.show()