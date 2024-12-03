#include <iostream>
#include <random>
#include <fstream>  
#include <vector>
#include <algorithm>
#include <numeric>  
#include <cmath>

double chi2(int k, std::vector<double> numbers){
    int n = numbers.size();
    // podział na k zbiorów
    double first = *std::min_element(numbers.begin(), numbers.end());
    double last = *std::max_element(numbers.begin(), numbers.end());
    double divider = (last-first) / (double)k;
    std::vector<int> divided_numbers_count(k,0);
    double curr_divider=first;
    double prev_divider;
    for(int i=0; i<k; i++){
        prev_divider = curr_divider;
        curr_divider += divider;
        for (int j=0; j< n; j++){
            if (((numbers[j] < curr_divider)&&(numbers[j] >= prev_divider))||
                ((i==k-1)&&((numbers[j] <= curr_divider)&&(numbers[j] >= prev_divider)))){
                divided_numbers_count[i]++;
            }
        } 
    }
    // obliczenie wzoru
    double value=0;
    for (int i = 0; i < k; i++){
        value += pow(divided_numbers_count[i], 2);
    }
    value = (((double)k/(double)n) *(value)) -(double)n;
    return value;
}

void evaluate(int k, std::vector<double> numbers){
    double value = chi2(k, numbers);
    std::cout << "test value = "<<value<<std::endl;     
    std::cout << "-----------------"<<std::endl;
}

void generate_values(int count){
    //rozkład jednostajnego na przedziale (0,10)
    std::uniform_real_distribution<double> rozklad(0, 10);
    std::ofstream outfile1("zad2_good_numbers.txt");
    std::ofstream outfile2("zad2_bad_numbers.txt");
    // minstd_rand knuth_b ranlux48 ranlux48_base mt19937_64 mt19937    
    // random_device
    std::random_device seed;
    //DOBRY
    std::ranlux48 generator1(seed());
    for (int i=0; i<count; i++){
        double number = rozklad(generator1);
        outfile1 << number << std::endl;
    }
    outfile1.close();
    //ZLY
    std::minstd_rand generator2(seed());
    for (int i=0; i<count; i++){
        double number = rozklad(generator2);
        outfile2 << number << std::endl;
    }
    outfile2.close();
}

int main(){
    // wygenerowanie danych i plików
    //generate_values(100000);
    // wczytanie danych z plików
    std::fstream file1("zad2_good_numbers.txt");
    std::vector<double> dobry_gen;
    double c1;
    while (file1 >> c1) {
        dobry_gen.push_back(c1);
    }
    file1.close();
    std::fstream file2("zad2_bad_numbers.txt");
    std::vector<double> zly_gen;
    double c2;
    while (file2 >> c2) {
        zly_gen.push_back(c2);
    }
    file2.close();
    // obliczenia
    int p=11;
    std::cout << "Przedziały= "<<p<<std::endl;
    std::cout << "ranlux48"<<std::endl;
    evaluate(p, dobry_gen);
    std::cout << "minstd_rand"<<std::endl;
    evaluate(p, zly_gen);
    p=51;
    std::cout << "Przedziały= "<<p<<std::endl;
    std::cout << "ranlux48"<<std::endl;
    evaluate(p, dobry_gen);
    std::cout << "minstd_rand"<<std::endl;
    evaluate(p, zly_gen);
    p=101;
    std::cout << "Przedziały= "<<p<<std::endl;
    std::cout << "ranlux48"<<std::endl;
    evaluate(p, dobry_gen);
    std::cout << "minstd_rand"<<std::endl;
    evaluate(p, zly_gen);
    return 0;
}