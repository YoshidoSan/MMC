#include <iostream>
#include <chrono>
#include <random>
#include <fstream>  
#include <gsl/rng/gsl_rng.h>

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

int main() {
    std::ofstream outfile("zad4_times.txt");

    const gsl_rng_type **t, **t0;
    // Tworzy listę generatorów i zwraca wskaźnik na jej początek.
    t0 = gsl_rng_types_setup ();
    for (t = t0; *t != 0; t++){
        //generator, seed, inicjalizacja
        gsl_rng *gen = gsl_rng_alloc(*t);
        //std::random_device seed;
        //unsigned long seed_as_long = static_cast<unsigned long>(seed);
        //int seed_as_int = static_cast<int>(seed);
        gsl_rng_set(gen, 42068);

        moment czas_przed = teraz();
        // Generacja liczb pseudolosowych
        int count = 1000000;
        for (int i = 0; i < count; i++){
            gsl_rng_uniform(gen);
        }
        gsl_rng_free(gen);
        moment czas_po = teraz();
        long czas = ile_trwa(czas_przed, czas_po);
        outfile << (*t)->name << " " << czas << std::endl;
    }  
    outfile.close();
    return 0;
}