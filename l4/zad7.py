import random 
import math
import numpy as np
from scipy.stats import qmc
from matplotlib import pyplot as plt

def f(x):
    return math.sin(x)

# ciąg sobola czyli liczby quasilosowe
sobol = qmc.Sobol(d=2, scramble=False, seed=1)

def head_tails(f, a, b, n, quasi, use_quasi=False):
    heads = 0
    for _ in range(n):
        if use_quasi is True:
            x, y = qmc.scale(quasi.random(1), [a, 0.0], [b, 1.0])[0]
        else:
            x = random.uniform(a, b)
            y = random.uniform(0, 1)
        if y <= f(x):
            heads += 1
    est_val = (heads/n)*abs(a - b)
    error = abs(est_val - 2)
    return est_val, error

def main():
    a = 0
    b = math.pi
    n = 10000
    experiment_num = 50
    error_list = list()
    error_list_quasi = list()
    for _ in range(experiment_num):
        est_val, error = head_tails(f, a, b, n, sobol)
        error_list.append(error)
        est_val_quasi, error_quasi = head_tails(f, a, b, n, sobol, use_quasi=True)
        error_list_quasi.append(error_quasi)

    plt.plot(np.linspace(1, experiment_num,  experiment_num), error_list, label='błąd pseudo losowe')
    plt.plot(np.linspace(1, experiment_num,  experiment_num), np.linspace(1, 1, experiment_num)*sum(error_list)/experiment_num, label='średni błąd pseudo losowe')
    plt.plot(np.linspace(1, experiment_num,  experiment_num), error_list_quasi, label='błąd quasi losowe')
    plt.plot(np.linspace(1, experiment_num,  experiment_num), np.linspace(1, 1, experiment_num)*sum(error_list_quasi)/experiment_num, label='średni błąd quasi losowe')
    plt.xlabel('Number próby')
    plt.ylabel('Wartość')
    plt.legend(loc='best')
    #plt.savefig('./l4/z7.pdf', format='pdf')
    plt.show()


if __name__ == "__main__":
    main()