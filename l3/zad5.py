from matplotlib import pyplot as plt
import math
import random
import numpy as np
from scipy.stats import cauchy
from scipy.stats import triang

def generate_cauchy(count):
    generated_numbers = list()
    for i in range(count):
        X = random.normalvariate(0.0, 1.0)
        Y = random.normalvariate(0.0, 1.0)
        Z = X/Y
        generated_numbers.append(Z)
    return generated_numbers

def generate_triangle(a, b, count):
    generated_numbers = list()
    for i in range(count):
        X = random.normalvariate(a/2, b/2)
        Y = random.normalvariate(a/2, b/2)
        Z = X + Y
        generated_numbers.append(Z)
    return generated_numbers

def main():
    a=-20.0
    b=20.0
    count = 100000

    numbers_cauchy = generate_cauchy(count)
    # Rysujemy histogram częstości
    plt.hist(numbers_cauchy, density=True, color='blue', range=(a, b), bins=100, label='cauchyego z metody')
    # Wartości na osi X histogramu.
    dziedzina = np.linspace(a, b, count)
    # Rysujemy również teoretyczną funkcję gęstości prawodpodobieństwa.
    plt.plot(dziedzina, cauchy.pdf(dziedzina), 'k-', label='teoretyczna')
    plt.xlabel('Wartość')
    plt.ylabel('Gęstość prawdopodobieństwa')
    plt.legend(loc='best')
    plt.savefig('./l3/histogram_z5_cauchy.pdf', format='pdf')
    plt.show()

    numbers_triangle = generate_triangle(a,b,count)
    # Rysujemy histogram częstości
    plt.hist(numbers_triangle, density=True, color='blue', range=(a, b), bins=100, label='trójkątna z metody')
    # Wartości na osi X histogramu.
    dziedzina = np.linspace(-a, b, count)
    # Rysujemy również teoretyczną funkcję gęstości prawodpodobieństwa.
    plt.plot(dziedzina, triang.pdf(dziedzina, c=0.5), 'k-', label='teoretyczna')
    plt.xlabel('Wartość')
    plt.ylabel('Gęstość prawdopodobieństwa')
    plt.legend(loc='best')
    plt.savefig('./l3/histogram_z5_triangle.pdf', format='pdf')
    plt.show()






if __name__ == "__main__":
    main()