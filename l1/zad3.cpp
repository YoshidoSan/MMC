#include <iostream>
#include <random>
#include <chrono>
#include <fstream>  

// Alias dla typu zwracanego przez 'steady_clock', czyli zegar dla którego
// kolejne wywołania metody 'now()' na pewno zwracają ciąg niemalejący.
typedef std::chrono::steady_clock::time_point moment;
// Zwraca bieżący czas w mikrosekundach.
moment teraz() { 
    return std::chrono::steady_clock::now(); 
}
// Zwraca liczbę mikrosekund, który upłynęły między dwoma 'momentami'.
long ile_trwa(moment od_kiedy, moment do_kiedy) {
  return std::chrono::duration_cast<std::chrono::microseconds>(do_kiedy-od_kiedy).count();
}


int main(){
    int count = 1000000;
    //rozkład jednostajnego na przedziale (0,10)
    std::uniform_real_distribution<double> rozklad(0, 10);
    //plik
    std::ofstream outfile("zad3_times.txt");

    // minstd_rand - Park, Miller, and Stockmeye
    std::random_device seed1;
    std::minstd_rand generator1(seed1());
    moment czas_przed_1 = teraz();
    for (int i=0; i<count; i++){
        int number = rozklad(generator1);
    }
    moment czas_po_1 = teraz();
    long czas_1 = ile_trwa(czas_przed_1, czas_po_1);
    outfile << czas_1 << std::endl;

    //mt19937 - 32-bit Mersenne Twister by Matsumoto and Nishimura
    std::random_device seed2;
    std::mt19937 generator2(seed2());
    moment czas_przed_2 = teraz();
    for (int i=0; i<count; i++){
        int number = rozklad(generator2);
    }
    moment czas_po_2 = teraz();
    long czas_2 = ile_trwa(czas_przed_2, czas_po_2);
    outfile << czas_2 << std::endl;

    //mt19937_64 - 64-bit Mersenne Twister by Matsumoto and Nishimura
    std::random_device seed3;
    std::mt19937_64 generator3(seed3());
    moment czas_przed_3 = teraz();
    for (int i=0; i<count; i++){
        int number = rozklad(generator3);
    }
    moment czas_po_3 = teraz();
    long czas_3 = ile_trwa(czas_przed_3, czas_po_3);
    outfile << czas_3 << std::endl;

    //ranlux48_base
    std::random_device seed4;
    std::ranlux48_base generator4(seed4());
    moment czas_przed_4 = teraz();
    for (int i=0; i<count; i++){
        int number = rozklad(generator4);
    }
    moment czas_po_4 = teraz();
    long czas_4 = ile_trwa(czas_przed_4, czas_po_4);
    outfile << czas_4 << std::endl;

    //ranlux48 - 48-bit RANLUX generator by Martin Lüscher and Fred James
    std::random_device seed5;
    std::ranlux48 generator5(seed5());
    moment czas_przed_5 = teraz();
    for (int i=0; i<count; i++){
        int number = rozklad(generator5);
    }
    moment czas_po_5 = teraz();
    long czas_5 = ile_trwa(czas_przed_5, czas_po_5);
    outfile << czas_5 << std::endl;

    //knuth_b
    std::random_device seed6;
    std::knuth_b generator(seed6());
    moment czas_przed_6 = teraz();
    for (int i=0; i<count; i++){
        int number = rozklad(generator);
    }
    moment czas_po_6 = teraz();
    long czas_6 = ile_trwa(czas_przed_6, czas_po_6);
    outfile << czas_6 << std::endl;


    outfile.close();
}