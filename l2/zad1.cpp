#include <iostream>
#include <random>
#include <fstream>  

//robimy 2 i (3 lub 4)

int main(){
    int count = 1000000;
    //rozkład jednostajnego na przedziale (0,10)
    std::uniform_real_distribution<double> rozklad(0, 10);
    //plik
    std::ofstream outfile("zad1.txt");

    // minstd_rand knuth_b ranlux48 ranlux48_base mt19937_64 mt19937    
    // random_device

    std::random_device seed1;
    std::ranlux48 generator1(seed1());
    for (int i=0; i<count; i++){
        int number = rozklad(generator1);
        outfile << number << std::endl;
    }
    outfile.close();
}