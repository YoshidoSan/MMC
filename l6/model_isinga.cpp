#include "model_isinga.h"

ModelIsinga::ModelIsinga(int rozmiar, int energia) 
	: L{rozmiar}
	, energia_docelowa_ukladu{energia}
	, generator{rozmiar} 
	, rodzaj{false}
{
	siatka = new int* [L];
	for (int i = 0; i < L; i++)
		siatka[i] = new int[L];
}

ModelIsinga::ModelIsinga(int rozmiar, float temperatura) 
	: L{rozmiar}
	, temperatura_docelowa_ukladu{temperatura}
	, generator{rozmiar} 
	, rodzaj{true}
{
	siatka = new int* [L];
	for (int i = 0; i < L; i++)
		siatka[i] = new int[L];
}	

ModelIsinga::~ModelIsinga() 
{
	if (siatka)
	{
        for (int i = 0; i < L; i++)
            delete[] siatka[i];
        delete[] siatka;
	}
}

void ModelIsinga::ustaw_same_jedynki() 
{
	for(int i = 0; i < L; i++)
		for(int j = 0; j < L; j++)
			siatka[i][j] = 1;
}

int ModelIsinga::deltaE(int x, int y) 
{
	// Periodyczne warunki brzegowe.
	int gorny = (x != 0)   ? x-1 : L-1;
	int dolny = (x != L-1) ? x+1 : 0;
	int prawy = (y != L-1) ? y+1 : 0;
	int lewy  = (y != 0)   ? y-1 : L-1;

	int E_pocz = siatka[x][y] * ( siatka[x][lewy] + siatka[x][prawy] + siatka[gorny][y] + siatka[dolny][y] );
	int E_konc = -E_pocz;

	return E_pocz - E_konc;
}

void ModelIsinga::sprobuj_odwrocic_spin_losowego_atomu() 
{
	int i = generator.losuj_wspolrzedna();
	int j = generator.losuj_wspolrzedna();
	int dE = deltaE(i, j);

	// mikrokanoniczny
	if(rodzaj==false){
		if (dE <= energia_duszka) 
		{				
			siatka[i][j] = -siatka[i][j];
			energia_duszka -= dE;
			energia_poczatkowa_ukladu  += dE;			
			magnetyzacja += 2*siatka[i][j];
		}
	}
	// kanoniczny
	if(rodzaj==true){
		// warunek z prawdopodobieńswa
		float p = exp(-dE / (temperatura_docelowa_ukladu)); 
		float l = generator.losuj_z_zakresu_0_1();
		// albo jest ujemne albo spełnione prawd
		if (dE <= 0 || l <= p) 
		{				
			siatka[i][j] = -siatka[i][j];
			energia_poczatkowa_ukladu  += dE;			
			magnetyzacja += 2*siatka[i][j];
		}
	}
}

void ModelIsinga::doprowadzenie_do_stanu_rownowagi(int liczba_krokow) 
{
	magnetyzacja = L * L;
	energia_poczatkowa_ukladu  = -2 * L * L;
	energia_duszka = energia_docelowa_ukladu - energia_poczatkowa_ukladu;
	ustaw_same_jedynki();

	for (int k = 0; k < liczba_krokow; ++k) 
	{
		sprobuj_odwrocic_spin_losowego_atomu();
	}
}

// Wyznacza średnią energię duszka, energię wewnętrzną układu, magnetyzację oraz temperaturę
void ModelIsinga::zliczanie_srednich(int liczba_krokow) 
{
	int energia_duszka_do_sredniej = 0;
	int energia_ukladu_do_sredniej = 0;	
	int magnetyzacja_tot = 0;
	
	// Pętla liczby_kroków
	for (int k = 0; k < liczba_krokow; ++k) 
	{
		// Pętla statystycznie po każdym spinie
		for (int l = 0; l < L*L; ++l) 
		{
			sprobuj_odwrocic_spin_losowego_atomu();
		}
		energia_duszka_do_sredniej += energia_duszka;
		energia_ukladu_do_sredniej += energia_poczatkowa_ukladu;
		magnetyzacja_tot += abs(magnetyzacja);
	}

	// Obliczanie średnich
	srednia_energia_duszka = (float)energia_duszka_do_sredniej / liczba_krokow;
	srednia_energia_ukladu = (float)energia_ukladu_do_sredniej / liczba_krokow;
	srednia_magnetyzacja = magnetyzacja_tot / (float)liczba_krokow / ((float)L*(float)L);
	temperatura = 4.0 / (log(1 + 4.0/srednia_energia_duszka));
}

float ModelIsinga::podaj_srednia_energie_duszka() 
{
	return srednia_energia_duszka;
}

float ModelIsinga::podaj_srednia_energie_ukladu() 
{
	return srednia_energia_ukladu;
}

float ModelIsinga::podaj_srednia_magnetyzacje() 
{
	return srednia_magnetyzacja;
}

float ModelIsinga::podaj_temperature() 
{
	return temperatura;
}

