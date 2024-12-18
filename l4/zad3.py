import numpy as np
import math
from matplotlib import pyplot as plt

# całka 0-pi/2 sin = -cos 0-pi/2 = - cos pi/2 - - cos 0 = 1
def f(x):
    return np.sin(x)

def mc_basic(f, a, b, N):
    # generacja liczb
    nums = np.random.uniform(a, b, N)
    # całka
    y = [f(num) for num in nums]
    y_m = np.sum(y)/N
    I_method = abs(a-b)*y_m
    # błąd całki (rzeczywista dla założeń wynosi 1)
    error = abs(I_method - 1)
    # odchylenie standardowe
    std_J = 1/np.sqrt((N*(N-1))) * np.sqrt( np.sum(np.square(y)) - (1/N)*np.square(np.sum(y)) )
    std_I = abs(a-b)*std_J
    # niepewność
    uncertain = 2*std_I
    return I_method, error, std_I, uncertain

def mc_basic_swap(f, a, b, N):
    # x = sqrt(z) czyli z = square(x)
    z_min = np.square(a)
    z_max = np.square(b)
    # generacja liczb
    nums = np.random.uniform(z_min, z_max, N)
    # całka
    y = y = f(np.sqrt(nums))/(2*np.sqrt(nums))
    y_m = np.sum(y)/N
    I_method = abs(z_min-z_max)*y_m
    # błąd całki (rzeczywista dla założeń wynosi 1)
    error = abs(I_method - 1)
    # odchylenie standardowe
    std_J = 1/np.sqrt((N*(N-1))) * np.sqrt( np.sum(np.square(y)) - (1/N)*np.square(np.sum(y)) )
    std_I = abs(z_min-z_max)*std_J
    # niepewność
    uncertain = 2*std_I
    return I_method, error, std_I, uncertain

def main():
    N = 100
    a = 0
    b = math.pi/2
    I_values = list()
    I_values_swap = list()
    uncertains = list()
    uncertains_swap = list()
    errors = list()
    errors_swap = list()
    N_list = list()
    while (N<10000):
        I_method, error, std_I, uncertain = mc_basic(f, a, b, N)
        I_method_swap, error_swap, std_I_swap, uncertain_swap = mc_basic_swap(f, a, b, N)

        I_values.append(I_method)
        I_values_swap .append(I_method_swap)
        uncertains.append(uncertain)
        uncertains_swap.append(uncertain_swap)
        errors.append(error)
        errors_swap.append(error_swap)
        N_list.append(N)
        N+=10
    plt.scatter(N_list, I_values, label='metoda podstawowa')
    plt.scatter(N_list, I_values_swap, label='zamiana zmiennych')
    plt.ylabel('Wartość')
    plt.xlabel('Liczba próbek')
    plt.legend(loc='best')
    #plt.savefig('./l4/z3_I.pdf', format='pdf')
    plt.show()

    plt.plot(N_list, uncertains, label='niepewności metoda podstawowa')
    plt.plot(N_list, uncertains_swap, label='niepewności zamiana zmiennych')
    plt.plot(N_list, errors, label='błąd metoda podstawowa')
    plt.plot(N_list, errors_swap, label='błąd zamiana zmiennych')
    plt.ylabel('Wartość')
    plt.xlabel('Liczba próbek')
    plt.legend(loc='best')
    #plt.savefig('./l4/z3_rest.pdf', format='pdf')
    plt.show()


if __name__ == "__main__":
    main()