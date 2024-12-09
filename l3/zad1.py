from matplotlib import pyplot as plt
import math
import random
import numpy as np
from scipy.stats import norm
# (1 lub 2) i 3
def calculate_y(n):
    generated_numbers = list()
    for i in range(n):
        generated_numbers.append(random.uniform(0.0, 1.0))
    Y = (sum(generated_numbers) - n/2) / math.sqrt(n/12)
    return Y

def generate(n, count):
    generated_numbers = list()
    for i in range(count):
        generated_numbers.append(calculate_y(n))
    return generated_numbers

def main():
    n=20
    count = 100000
    numbers_method = generate(n, count)

    # Rysujemy histogram częstości
    plt.hist(numbers_method, density=True, color='blue', range=(-4.0, 4.0), bins=100, label='normalna z metody')
    # Wartości na osi X histogramu.
    dziedzina = np.linspace(-4.0, 4.0, count)
    # Rysujemy również teoretyczną funkcję gęstości prawodpodobieństwa.
    plt.plot(dziedzina, norm.pdf(dziedzina), 'k-', label='teoretyczna')
    plt.xlabel('Wartość')
    plt.ylabel('Gęstość prawdopodobieństwa')
    plt.legend(loc='best')
    plt.savefig('./l3/histogram_z1.pdf', format='pdf')
    plt.show()

if __name__ == "__main__":
    main()