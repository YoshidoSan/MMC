from matplotlib import pyplot as plt
import numpy as np

class GeneratorWH:
    def __init__(self):
        self.x=None
        self.y=None
        self.z=None
        self.numbers = None
    
    def generate(self, seed, count):
        self.x = list()
        self.y = list()
        self.z = list()
        self.numbers = list()
        prev_x=seed[0]
        prev_y=seed[1]
        prev_z=seed[2]
        self.x.append(prev_x)
        self.y.append(prev_y)
        self.z.append(prev_z)
        for i in range(count):
            x = 171*prev_x % 30269
            y = 172*prev_y % 30307
            z = 170*prev_z % 30323
            k = (x/30269 + y/30307 + z/30323) % 1
            prev_x=x
            prev_y=y
            prev_z=z
            self.numbers.append(k)
            self.x.append(x)
            self.y.append(y)
            self.z.append(z)
    
    def save_to_file(self, step=1):
        filename = f'wh_numbers_step_{step}.txt'
        file = open(filename, "w")
        for i in range(len(self.numbers) - step):
            line = f'{self.numbers[i]} {self.numbers[i+step]}\n'
            file.write(line)
        file.close()

    def plot(self, step=1):
        filename = f'wh_numbers_step_{step}.txt'
        k, k_n = np.loadtxt(filename, unpack=True)
        plt.figure()
        plt.plot(k, k_n, '.') 
        plt.grid(linestyle=':')
        plt.xlabel('k_n')
        plt.ylabel(f'k_n+{step}')
        plt.title(f'Wykres rozproszenia: krok={step}')
        plt.show()


def main():
    generatorwh1 = GeneratorWH()
    generatorwh1.generate(seed=[3,3,3],count=2500)

    generatorwh1.save_to_file(step=1)
    generatorwh1.plot(step=1)
    generatorwh1.save_to_file(step=2)
    generatorwh1.plot(step=2)

if __name__ == "__main__":
    main()