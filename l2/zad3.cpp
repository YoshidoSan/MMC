#include <iostream>
#include <random>
#include <fstream>  


int main(){
    int count = 1000000;
    // rozkład jednostajny na przedziale (-1,1)
    std::uniform_real_distribution<double> rozklad(-1.0, 1.0);
    std::random_device seed;
    std::ranlux48 generator(seed());
    // liczność próby, estymata wyniku i odchylenie standardowe
    std::ofstream outfile("zad3.txt");
    double M, N, x, y, pi, sigma;
    long log_step=1;
    for (int i=1; i<count; i++){
        // próbka
        x = rozklad(generator);
        y = rozklad(generator);
        // warunek sukcesu
        if(x*x + y*y <= 1.0){
            M++;
        }
        N = i;
        pi = 4.0* M/N;
        sigma = 4.0*sqrt(M/(N*N) * (1.0- M/N));
        if (i % log_step ==0){
            outfile << i <<" "<< pi <<" "<< sigma << "\n";
            log_step *=2;
        }
    }
    outfile.close();
    return 0;
}