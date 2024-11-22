from matplotlib import pyplot as plt
import numpy as np

filename = r"Laby\przygotowawcze\l1\zad4_times.txt"
generatory, czasy = np.loadtxt(filename, unpack=True)

# Do opisania osi X docelowo posłużą nazwy miast,
# ale do prawidłowego skonstruowania tej osi konieczny jest
# ciąg liczb - tutaj (0, 1, 2).
indeksy = np.arange(len(generatory))
# Względna szerokość słupka - przy dwóch seriach danych powninna wynosić 0,5 lub nieco mniej. 
szer_slupka = 0.5
# Wykres słupkowy pierwszej serii danych.
plt.bar(indeksy, czasy, szer_slupka)
# Opisy osi X
plt.xticks(indeksy, generatory, rotation=45)
plt.ylabel('Czas')
plt.show()