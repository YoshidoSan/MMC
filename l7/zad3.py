import random 
import math
import numpy as np
from scipy.stats import norm, t
from matplotlib import pyplot as plt

def generate_t_student(micro, sigma, v):
    num = t.rvs(v, micro, sigma, size=1)[0]
    return num

def generate_uniform(a, b):
    return random.uniform(a,b)

def generate_gauss(micro, sigma):
    return random.normalvariate(micro, sigma)

def generate_arc_sin(a, b):
    r = random.uniform(0,1)
    eta = ((a+b)/2) + ((b-a)/2)*math.sin(2*math.pi)*r
    return eta

def generate_trapezoid(a, b, d):
    r_1 = random.uniform(0,1)
    r_2 = random.uniform(0,1)
    eta = a + ((b-a)/2)*((1+d)*r_1 + (1-d)*r_2)
    return eta

def calculate_error(K):
    X1 = generate_t_student(50000623, 25, 18)
    X2 = generate_t_student(215, 6, 24)
    X3 = generate_t_student(0, 4, 5)
    X4 = generate_t_student(0, 7, 8)
    X5 = generate_uniform(9.5*pow(10,-6), 1.35*pow(10,-5))
    X6 = generate_gauss(-0.1, 0.2)
    X7 = generate_arc_sin(-0.5, 0.5)
    X8 = generate_trapezoid(-1*pow(10, -6), 1*pow(10,-6), 1*pow(10,-7))
    X9 = generate_trapezoid(-0.05, 0.05, 0.025)
    Y = X1 + X2 + X3 + X4 - X1*(X8*(X6 + X7) + X5*X9) - K
    return Y


def main():
    M = 100000
    errors = []
    for i in range(M):
        Y = calculate_error(50*pow(10,6))
        errors.append(Y)
    
    estimator = np.mean(errors)
    print(f'Estymator Y: {estimator}')
    
    sum = 0
    for error in errors:
        sum += pow((error - estimator),2)
    std = math.sqrt((1/(M-1))*sum)
    print(f'Odchylenie standardowe: {std}')

    p = 0.99
    q = int(p*M)
    sorted_errors = sorted(errors)
    r_star = int((M-q)/2)
    y_min = sorted_errors[r_star+1]
    y_max = sorted_errors[r_star+q+1]
    dis = [y_min, y_max]
    print(f'Przedział rozproszenia: {dis}')

    x = np.linspace(estimator - 4*std, estimator + 4*std, 1000)
    y = norm.pdf(x, estimator, std)
    plt.hist(errors, bins=100, density=True)
    plt.plot(x,y)
    plt.axvline(dis[0], color='r', label='Przedział rozproszenia')
    plt.axvline(dis[1], color='r')
    plt.xlabel('Wartości niepewności')
    plt.ylabel('Gęstość prawdopodobieństwa')
    plt.legend(loc='best')
    plt.show()

if __name__ == "__main__":
    main()