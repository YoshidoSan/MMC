#include <iostream>
#include "model_isinga.h"
#include "fstream"
using namespace std;

/** 
 * @file
 * Główna funkcja programu. 
 * Tworzy model próbki ferromagnetycznej o danej wielkości, wprowadza do niego 
 * zadaną ilość energii (układ mikrokanoniczny) i oblicza statystyki 
 * (temperatura, magnetyzacja itp.)
 * Obecna postać ma charakter czysto demonstracyjny, 
 * ponieważ uwzględnia tylko jedną wielkość siatki i jedną wartość energii docelowej.
 * <b>
 * Przy realizacji zadań z instrukcji należy użyć zagnieżdżonej pętli po wielkościach
 * siatki i energiach docelowych (Zadanie 1 – układ mikrokanoniczny), względnie po
 * wielkościach siatki i temperaturach (Zadanie 2 – układ kanoniczny).
 * </b>
 */

void zadanie1(string filename, int E_start, int E_end, int L, int step){
	// plik tekstowy
	std::ofstream file;
	file.open(filename);
	for(int e=E_start; e<=E_end; e+=step){
		// symulacja modelu
		ModelIsinga ising(L, e);
		ising.doprowadzenie_do_stanu_rownowagi(1000);
		ising.zliczanie_srednich(1000);
		// zapisanie do pliku w jednej linii
		file << e << " " << L << " " << ising.podaj_srednia_energie_ukladu() << " ";
		file << ising.podaj_srednia_energie_duszka() << " " << ising.podaj_srednia_magnetyzacje() << " ";
		file << ising.podaj_temperature() << std::endl;
	}
	// zamknięcie pliku
	file.close();
}

void zadanie2(string filename, float T_start, float T_end, int L, float step){
	// plik tekstowy
	std::ofstream file;
	file.open(filename);
	// najpierw gęsto
	for(float t=T_start; t<=3.0; t+=step/5.0){
		// symulacja modelu
		ModelIsinga ising(L, t);
		ising.doprowadzenie_do_stanu_rownowagi(1000);
		ising.zliczanie_srednich(1000);
		// zapisanie do pliku w jednej linii
		file << t << " " << L << " " << ising.podaj_srednia_energie_ukladu() << " ";
		file << ising.podaj_srednia_magnetyzacje() << std::endl;
	}
	// potem normalnie
	for(float t=3.0; t<=T_end; t+=step){
		// symulacja modelu
		ModelIsinga ising(L, t);
		ising.doprowadzenie_do_stanu_rownowagi(1000);
		ising.zliczanie_srednich(1000);
		// zapisanie do pliku w jednej linii
		file << t << " " << L << " " << ising.podaj_srednia_energie_ukladu() << " ";
		file << ising.podaj_srednia_magnetyzacje() << std::endl;
	}
	// zamknięcie pliku
	file.close();
}



int main(int argc, char *argv[])
{
	zadanie1("zad1_1.txt", -184, -24, 10, 8);
	zadanie1("zad1_2.txt", -768, -32, 20, 32);
	zadanie1("zad1_3.txt", -3072, -128, 40, 128);

	zadanie2("zad2_1.txt", 1.0, 6.0, 10, 0.1);
	zadanie2("zad2_2.txt", 1.0, 15.0, 20, 0.1);
	zadanie2("zad2_3.txt", 1.0, 20.0, 40, 0.1);
	return 0;
}
