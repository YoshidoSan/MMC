from matplotlib import pyplot as plt
import numpy as np


def plot_from_file_z1(filename):
    val, L, E_mean, E_ghost, M_mean, T_mean = np.loadtxt(filename, unpack=True)
    # wykresy
    fig, axs = plt.subplots(1,2,figsize=(10,5))
    # mag od temp
    axs[0].plot(T_mean, M_mean)
    axs[0].set_xlabel('Temperatura')
    axs[0].set_ylabel('Magnetyzacja')
    axs[0].set_title('Średnia magnetyzacja od temperatury')
    # energ od temp
    axs[1].plot(T_mean, E_mean)
    axs[1].set_xlabel('Temperatura')
    axs[1].set_ylabel('Energia układu')
    axs[1].set_title('Średnia energia układu od temperatury')
    # pokaz
    plt.tight_layout()
    plt.show()

def plot_from_file_z2(filename):
    val, L, E_mean, M_mean = np.loadtxt(filename, unpack=True)
    # wykresy
    fig, axs = plt.subplots(1,2,figsize=(10,5))
    # mag od temp
    axs[0].plot(val, M_mean)
    axs[0].set_xlabel('Temperatura')
    axs[0].set_ylabel('Magnetyzacja')
    axs[0].set_title('Średnia magnetyzacja od temperatury')
    # energ od temp
    axs[1].plot(val, E_mean)
    axs[1].set_xlabel('Temperatura')
    axs[1].set_ylabel('Energia układu')
    axs[1].set_title('Średnia energia układu od temperatury')
    # pokaz
    plt.tight_layout()
    plt.show()

def compare_from_files(filename1, filename2):
    val1, L1, E_mean1, E_ghost1, M_mean1, T_mean1 = np.loadtxt(filename1, unpack=True)
    val2, L2, E_mean2, M_mean2 = np.loadtxt(filename2, unpack=True)
    # wykresy
    fig, axs = plt.subplots(1,2,figsize=(10,5))
    # mag od temp
    axs[0].plot(T_mean1, M_mean1, label='mikrokanoniczny')
    axs[0].plot(val2, M_mean2, label='kanoniczny')
    axs[0].set_xlabel('Temperatura')
    axs[0].set_ylabel('Magnetyzacja')
    axs[0].set_title('Średnia magnetyzacja od temperatury')
    axs[0].legend(loc="upper right")
    # energ od temp
    axs[1].plot(T_mean1, E_mean1, label='mikrokanoniczny')
    axs[1].plot(val2, E_mean2, label='kanoniczny')
    axs[1].set_xlabel('Temperatura')
    axs[1].set_ylabel('Energia układu')
    axs[1].set_title('Średnia energia układu od temperatury')
    axs[1].legend(loc="lower right")
    # pokaz
    plt.tight_layout()
    plt.show()



def main():
    plot_from_file_z1("l6/zad1_1.txt")
    plot_from_file_z1("l6/zad1_2.txt")
    plot_from_file_z1("l6/zad1_3.txt")

    plot_from_file_z2("l6/zad2_1.txt")
    plot_from_file_z2("l6/zad2_2.txt")
    plot_from_file_z2("l6/zad2_3.txt")

    compare_from_files("l6/zad1_1.txt", "l6/zad2_1.txt")
    compare_from_files("l6/zad1_2.txt", "l6/zad2_2.txt")
    compare_from_files("l6/zad1_3.txt", "l6/zad2_3.txt")


if __name__ == "__main__":
    main()