import numpy as np
from matplotlib import pyplot as plt

def load_from_files(chorzy, ozdrowiali, podatni):
    lista_chorzy = []
    lista_ozdrowiali = []
    lista_podatni = []

    with open(chorzy) as file:
        for line in file:
            values = [int(item) for item in line.strip().split()]
            lista_chorzy.append(values)

    with open(ozdrowiali) as file:
        for line in file:
            values = [int(item) for item in line.strip().split()]
            lista_ozdrowiali.append(values)

    with open(podatni) as file:
        for line in file:
            values = [int(item) for item in line.strip().split()]
            lista_podatni.append(values)

    return lista_chorzy, lista_ozdrowiali, lista_podatni

def plot_results(lista_chorzy, lista_ozdrowiali, lista_podatni):
    fig, ax = plt.subplots(3,1,figsize=(8,15))
    time = np.arange(0, len(lista_chorzy[0]), 1)
    
    for sim in lista_chorzy:
        ax[0].plot(time, sim, color='r')
    ax[0].set_title('Chorzy')
    ax[0].set_xlabel('Dzień')
    ax[0].set_ylabel('Liczba chorych')
    ax[0].grid(True)

    for sim in lista_ozdrowiali:
        ax[1].plot(time, sim, color='b')
    ax[1].set_title('Ozdrowiali')
    ax[1].set_xlabel('Dzień')
    ax[1].set_ylabel('Liczba ozdrowiałych')
    ax[1].grid(True)

    for sim in lista_podatni:
        ax[2].plot(time, sim, color='g')
    ax[2].set_title('Podatni')
    ax[2].set_xlabel('Dzień')
    ax[2].set_ylabel('Liczba podatnych')
    ax[2].grid(True)

    fig.tight_layout()
    plt.show()

def plot_map(file, title):
    with open(file, 'r') as f:
        data = [line.strip().split() for line in f]

    x_coords = []
    y_coords = []
    colors = []
    for y,row in enumerate(data):
        for x, value in enumerate(row):
            x_coords.append(x)
            y_coords.append(y)
            colors.append(int(value))
    
    plt.figure(figsize=(3, 3))
    scatter = plt.scatter(x_coords, y_coords, c=colors, cmap='viridis', marker='s')
    plt.colorbar(scatter, ticks=[0, 1, 2], label='Values')
    plt.gca().invert_yaxis()
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title(title)
    plt.show()

def main():
    # zaladuj dane 
    chorzy, ozdrowiali, podatni = load_from_files('l5/wyniki/bazowe/chorzy_kazdego_dnia.txt', 'l5/wyniki/bazowe/ozdrowiali_kazdego_dnia.txt', 'l5/wyniki/bazowe/podatni_kazdego_dnia.txt')
    # wykresy danych
    plot_results(chorzy, ozdrowiali, podatni)
    # wykresy mapy
    plot_map('l5/wyniki/bazowe/mapa_0.txt', 'Dzień 0')
    plot_map('l5/wyniki/bazowe/mapa_20.txt', 'Dzień 20')
    plot_map('l5/wyniki/bazowe/mapa_40.txt', 'Dzień 40')
    plot_map('l5/wyniki/bazowe/mapa_60.txt', 'Dzień 60')
    plot_map('l5/wyniki/bazowe/mapa_80.txt', 'Dzień 80')
    plot_map('l5/wyniki/bazowe/mapa_100.txt', 'Dzień 100')
    plot_map('l5/wyniki/bazowe/mapa_final.txt', 'Dzień 120')
    # paramy ruchu - 0.1, 0.3, 0.5 0.7
if __name__ == "__main__":
    main()