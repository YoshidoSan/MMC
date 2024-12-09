from matplotlib import pyplot as plt
import math
import random
import numpy as np
from scipy.stats import norm

def f(x):
    y = (5/12)*(1 + pow((x-1),4))
    return y 

def calculate_elimination(a,b):
    searching = True
    while(searching):
        u1 = random.uniform(a,b)
        u2 = random.uniform(0.0,10/12)
        if ( u2 < f(u1) ):
            X = u1
            searching = False    
    return X

def generate_elimination(a,b, count):
    generated_numbers = list()
    for i in range(count):
        generated_numbers.append(calculate_elimination(a,b))
    return generated_numbers

def generate_superposition(a,b, count):
    generated_numbers = list()
    for i in range(count):
        u1 = random.uniform(a,b)
        u2 = random.uniform(0.0,1.0)
        if (u2 < 5/6):
            X = u1
        elif (u2 >= 5/6):
            z = (u1 - 1.0)
            if z < 0:
                pow = -((-z)**(1/5))
            elif z > 0:
                pow = z**(1/5)
            else:
                pow = 0
            X = 1.0 + pow
        generated_numbers.append(X)
    return generated_numbers

def main():
    a=0.0
    b=2.0
    count = 100000
    numbers_elimination = generate_elimination(a,b, count)
    numbers_superposition = generate_superposition(a,b, count)

    # Rysujemy histogram częstości
    plt.hist(numbers_elimination, density=True, color='blue', range=(a,b), bins=100, label='z metody eliminacji')
    # Wartości na osi X histogramu.
    dziedzina = np.linspace(a, b, count)
    # Rysujemy również teoretyczną funkcję gęstości prawodpodobieństwa.
    plt.plot(dziedzina, f(dziedzina), 'k-', label='teoretyczna')
    plt.xlabel('Wartość')
    plt.ylabel('Gęstość prawdopodobieństwa')
    plt.legend(loc='best')
    plt.savefig('./l3/histogram_z3_elim.pdf', format='pdf')
    plt.show()

    # Rysujemy histogram częstości
    plt.hist(numbers_superposition, density=True, color='blue', range=(a,b), bins=100, label='z metody superpozycji')
    # Wartości na osi X histogramu.
    dziedzina = np.linspace(a, b, count)
    # Rysujemy również teoretyczną funkcję gęstości prawodpodobieństwa.
    plt.plot(dziedzina, f(dziedzina), 'k-', label='teoretyczna')
    plt.xlabel('Wartość')
    plt.ylabel('Gęstość prawdopodobieństwa')
    plt.legend(loc='best')
    plt.savefig('./l3/histogram_z3_superpoz.pdf', format='pdf')
    plt.show()

if __name__ == "__main__":
    main()