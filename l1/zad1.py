from matplotlib import pyplot as plt
import numpy as np

class Generator:
    def __init__(self,a,m,c=0):
        self.a=a
        self.m=m
        self.c=c
        self.numbers = None
    
    def generate(self, seed, count):
        self.numbers = list()
        prev=seed
        self.numbers.append(prev)
        for i in range(count):
            x = (self.a*prev + self.c) % self.m
            self.numbers.append(x)
            prev = x
    
    def save_to_file(self, step=1):
        filename = f'numbers_step_{step}_a_{self.a}.txt'
        file = open(filename, "w")
        for i in range(len(self.numbers) - step):
            line = f'{self.numbers[i]} {self.numbers[i+step]}\n'
            file.write(line)
        file.close()

    def plot(self, step=1):
        filename = f'numbers_step_{step}_a_{self.a}.txt'
        x, x_n = np.loadtxt(filename, unpack=True)
        plt.figure()
        plt.plot(x, x_n, '.') 
        plt.grid(linestyle=':')
        plt.xlabel('x_n')
        plt.ylabel(f'x_n+{step}')
        plt.title(f'Wykres rozproszenia: krok={step}, a={self.a}')
        plt.show()

    def plot_autocor(self):
        data = np.array(self.numbers, dtype=np.float32)
        plt.figure()
        max = len(self.numbers) - 1
        plt.acorr(data, maxlags=max)
        plt.grid(linestyle=':')
        plt.xlabel('Przesunięcia')
        plt.title(f'Autokorelacja: a={self.a}')
        plt.show()


def main():
    generator1 = Generator(a=17, m=pow(2,13)-1, c=0)
    generator1.generate(seed=6,count=2500)
    generator2 = Generator(a=85, m=pow(2,13)-1, c=0)
    generator2.generate(seed=6,count=2500)

    generator1.save_to_file(step=1)
    generator1.plot(step=1)
    generator1.save_to_file(step=2)
    generator1.plot(step=2)

    generator2.save_to_file(step=1)
    generator2.plot(step=1)
    generator2.save_to_file(step=2)
    generator2.plot(step=2)

if __name__ == "__main__":
    main()