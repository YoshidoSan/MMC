/***************************************************************************************
*
*   Program do modelowania rozprzestrzeniania siÄ™ epidemii na kwadratowej siatce.
*  
*	Autor: Dominik Kasprowicz
*   Poprawki: Sebastian CieÅ›lak
*	Ostatnia aktualizacja: 6 maja 2021
*
***************************************************************************************/


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator> // std::distance()
#include <random>

// ZbiÃ³r moÅ¼liwych stanÃ³w osobnika
enum Stan: char { zaszczepiony, ozdrowialy, podatny, chory }; 
/********************************************************************************/

// PoÅ‚oÅ¼enie osobnika na mapie
struct Koordynaty { 
    int x, y;

    Koordynaty() : x(0), y(0) { }

    Koordynaty(int ax, int ay) : x(ax), y(ay) { }

    Koordynaty sasiad_lewy() const { return Koordynaty(x-1, y); }

    Koordynaty sasiad_prawy() const { return Koordynaty(x+1, y); } 

    Koordynaty sasiad_gorny() const { return Koordynaty(x, y-1); } 

    Koordynaty sasiad_dolny() const { return Koordynaty(x, y+1); } 
   
    friend std::ostream& operator<<(std::ostream& ostr, const Koordynaty& wsp) {
        ostr << "(" << wsp.x << ", " << wsp.y << ")";
        return ostr;
    } 
};
/********************************************************************************/

struct Wirus {
    float beta, gamma;

    Wirus() : beta(0.5), gamma(0.5) { }
    Wirus(float b, float g) : beta(b), gamma(g) { }

    friend std::ostream& operator<<(std::ostream& ostr, const Wirus& wirus) {
        ostr << "Zaraza o parametrach: beta=" << wirus.beta << ", gamma=" << wirus.gamma << std::endl;
        return ostr;
    } 
};
/********************************************************************************/

// Generator liczb pseudolosowych
class RNG {
    std::mt19937_64 generator;
    std::uniform_int_distribution<int>    losowa_koordynata;
    std::uniform_real_distribution<float> losowa_0_1;
public:
    RNG(int bok_mapy) : generator(132123), losowa_koordynata(0, bok_mapy-1), losowa_0_1(0, 1) { }

    Koordynaty losuj_koordynaty() { return Koordynaty(losowa_koordynata(generator), losowa_koordynata(generator)); }
    float      losuj_od_0_do_1()  { return losowa_0_1(generator); }
};
/********************************************************************************/

// Populacja jako kwadratowa siatka osobnikÃ³w (a wÅ‚aÅ›ciwie ich stanÃ³w) z przydatnymi metodami.
class Populacja {
    int bok_mapy; // DÅ‚ugoÅ›Ä‡ boku siatki
    // Generator liczb pseudolosowych uÅ¼ywany do:
    // 1. losowania wspÃ³Å‚rzÄ™dnych osobnikÃ³w zaszczepionych i zaraÅ¼onych pierwszego dnia,
    // 2. decydowania, czy sÄ…siad zostanie zaraÅ¼ony (co zachodzi z prawdopodobieÅ„stwem beta),
    // 3. decydowania, czy osobnik wyzdrowiaÅ‚ (co zachodzi z prawdopodobieÅ„stwem gamma).
    RNG rng;       

    // Populacja jako dwuwymiarowa siatka osobnikÃ³w
    std::vector<Stan> osobniki;

    // Pomocnicze listy wspÃ³Å‚rzÄ™dnych: osobniki chore i odporne 
    std::list<Koordynaty>  chorzy_x_y;    
    std::list<Koordynaty>  odporni_x_y;

    // Parametry epidemii
    Wirus wirus;

    bool jest_na_mapie(const Koordynaty& wsp) const {
        return wsp.x >= 0 and wsp.y >= 0 and wsp.x < bok_mapy and wsp.y < bok_mapy;
    }

public:
    Populacja(int n) : bok_mapy(n), rng(bok_mapy) { reset(); }   

    void reset() {
        osobniki = std::vector<Stan>(bok_mapy*bok_mapy, Stan::podatny);
        chorzy_x_y.clear();
        odporni_x_y.clear();
    }    

    int  dlugosc_boku() const { return bok_mapy; }

    long liczebnosc() const { return osobniki.size(); }

    long ilu_chorych() const { return chorzy_x_y.size(); }

    long ilu_odpornych() const { return odporni_x_y.size(); }
    
    long ilu_podatnych() const { return liczebnosc() - ilu_chorych() - ilu_odpornych(); }

    // Funkcja dodana przez Sebastiana CieÅ›laka.
    long ilu_ozdrowialych() {
        long ilu = 0;
        for (Koordynaty& wsp: odporni_x_y) {
            if(czy_ozdrowialy(wsp)) ++ilu;
        }
        return ilu;
    }

    // Funkcja dodana przez Sebastiana CieÅ›laka.
    long ilu_zaszczepionych() {
        long ilu = 0;
        for (Koordynaty& wsp: odporni_x_y) {
            if(czy_zaszczepiony(wsp)) ++ilu;
        }
        return ilu;
    }

    Stan odczytaj_stan(const Koordynaty& wsp) const { return osobniki[bok_mapy*wsp.x + wsp.y]; }

    void ustaw_stan(const Koordynaty& wsp, const Stan& stan) { osobniki[bok_mapy*wsp.x + wsp.y] = stan; }                

    bool czy_chory(const Koordynaty& wsp) const { return odczytaj_stan(wsp) == Stan::chory; }

    bool czy_zaszczepiony(const Koordynaty& wsp) const { return odczytaj_stan(wsp) == Stan::zaszczepiony; }

    bool czy_ozdrowialy(const Koordynaty& wsp) const { return odczytaj_stan(wsp) == Stan::ozdrowialy; }

    bool czy_podatny(const Koordynaty& wsp) const { return odczytaj_stan(wsp) == Stan::podatny; }

    bool czy_niepodatny(const Koordynaty& wsp) const { return czy_zaszczepiony(wsp) or czy_ozdrowialy(wsp); }

    void zamien_osobniki(const Koordynaty& wsp1, const Koordynaty& wsp2) { 
        Stan stan1 = odczytaj_stan(wsp1);
        ustaw_stan(wsp1, odczytaj_stan(wsp2));        
        ustaw_stan(wsp2, stan1);
    }

    // Zwraca listÄ™ Koordynat zawierajÄ…cych wyÅ‚Ä…cznie podatnych sÄ…siadÃ³w     
    std::list<Koordynaty> znajdz_podatnych_sasiadow(const Koordynaty& wsp) {
        std::list<Koordynaty> sasiedzi;
        for (Koordynaty sasiad: { 
            wsp.sasiad_lewy(), 
            wsp.sasiad_prawy(), 
            wsp.sasiad_dolny(), 
            wsp.sasiad_gorny() 
        }) if (jest_na_mapie(sasiad) and czy_podatny(sasiad))
            sasiedzi.push_back(sasiad);        
        return sasiedzi;
    }

    // Zaszczep losowo wybrane osobniki i zainfekuj inne, rÃ³wnieÅ¼ wybrane losowo.
    // Funkcja poprawiona przez Sebastiana CieÅ›laka.
    void zaraza_przybywa(const Wirus& wir, long ilu_chorych, long ilu_odpornych) {
        wirus = wir;

        // Losowe wspÃ³Å‚rzÄ™dne osobnikÃ³w odpornych (wyÅ‚Ä…cznie wÅ›rÃ³d podatnych).
        for (int i = 0; i < ilu_odpornych; ++i) {
            Koordynaty wsp;
            do { wsp = rng.losuj_koordynaty(); } while (not czy_podatny(wsp));
            odporni_x_y.push_back(wsp);
            ustaw_stan(odporni_x_y.back(), Stan::zaszczepiony);
        }

        // Losowe wspÃ³Å‚rzÄ™dne osobnikÃ³w zaraÅ¼onych (wyÅ‚Ä…cznie wÅ›rÃ³d podatnych). 
        for (int i = 0; i < ilu_chorych; ++i) {
            Koordynaty wsp;
            do { wsp = rng.losuj_koordynaty(); } while (not czy_podatny(wsp));
            chorzy_x_y.push_back(wsp);
            ustaw_stan(chorzy_x_y.back(), Stan::chory);
        }
    }


    // Kolejna "tura" symulacji: chorzy majÄ… szansÄ™ wyzdrowieÄ‡, podatni mogÄ… siÄ™ zaraziÄ‡. 
    void kolejny_dzien() {
        std::list<Koordynaty> chorzy_nowi;
        //==============================================================================================================
        //==============================================================================================================
        //==============================================================================================================
        // dwa losowe osobniki mają szansę że zamienią się miejscami
        
        double travel_param = 0.9;
        long liczba = liczebnosc();
        for(int i=0; i<liczba/2; i++){
            // szansa na podróż
            if (rng.losuj_od_0_do_1() < travel_param){
                // losujemy dwa różne osobniki
                Koordynaty wsp_travel_1, wsp_travel_2;
                do {wsp_travel_1 = rng.losuj_koordynaty(); wsp_travel_2 = rng.losuj_koordynaty();} while ((wsp_travel_1.x==wsp_travel_2.x)&&(wsp_travel_1.y==wsp_travel_2.y));
                zamien_osobniki(wsp_travel_1, wsp_travel_2);}
        }
        //==============================================================================================================
        //==============================================================================================================
        //==============================================================================================================

        // Dla kaÅ¼dego chorego...
        for (Koordynaty& chory_x_y: chorzy_x_y) {
            // ...znajdujemy jego podatnych sÄ…siadÃ³w...
            for (Koordynaty& podatny_x_y: znajdz_podatnych_sasiadow(chory_x_y)) {                
                // ... i prÃ³bujemy zaraziÄ‡ kaÅ¼dego z nich, 
                // co udaje siÄ™ z prawdopodobieÅ„stwem 'beta'.
                if (rng.losuj_od_0_do_1() < wirus.beta) {
                    // Zainfekuj
                    ustaw_stan(podatny_x_y, Stan::chory);
                    chorzy_nowi.push_back(podatny_x_y);
                }
            }
        }

        // Osobniki, ktÃ³re byÅ‚y chore juÅ¼ w poprzedniej iteracji, powoli zdrowiejÄ….
        for (Koordynaty& osobnik_x_y: chorzy_x_y) {        
            if (rng.losuj_od_0_do_1() < wirus.gamma) {
                ustaw_stan(osobnik_x_y, Stan::ozdrowialy);
                // Dodaj do listy odpornych
                odporni_x_y.push_back(osobnik_x_y);                        
            }
        }

        // UsuÅ„ ozdrowiaÅ‚ych z listy chorych.
        chorzy_x_y.remove_if([&](const Koordynaty& wsp) { return czy_ozdrowialy(wsp); });        

        // Osobniki zaraÅ¼one w bieÅ¼Ä…cej iteracji doÅ‚Ä…czamy do ogÃ³lnej puli zaraÅ¼onych.
        chorzy_x_y.splice(chorzy_x_y.end(), chorzy_nowi);

    }        

    // Zapisuje stan siatki do pliku o podanej nazwie, 
    bool zapisz_do_pliku(const std::string& nazwa_pliku) const {
        std::ofstream plik(nazwa_pliku);

	    if (not plik.is_open()) {
		    std::cout << " Nie mogÄ™ utworzyÄ‡ pliku '" << nazwa_pliku << "`" << std::endl;
		    return false;
        }

        char stan_jako_znak;

        for (int x = 0; x < bok_mapy; ++x) {
            for (int y = 0; y < bok_mapy; ++y) {
                Stan stan = osobniki[bok_mapy*x + y];
                switch (stan) {
                    case Stan::chory:        stan_jako_znak = '3'; break;
                    case Stan::podatny:      stan_jako_znak = '2'; break;
                    case Stan::ozdrowialy:   stan_jako_znak = '1'; break;
                    case Stan::zaszczepiony: stan_jako_znak = '0'; break;
                    default: break;
                }
                plik << stan_jako_znak << '\t';
            }
            plik << std::endl;
        }
        plik.close();
        return true;
	}
};
/********************************************************************************/

// Kontener do przechowywania dziennych licznoÅ›ci grupy w wybranym stanie, np. zaraÅ¼onych.
// UdostÄ™pnia rÃ³wnieÅ¼ podstawowe metody do obrÃ³bki statystycznej
// (oczywiÅ›cie zachÄ™camy do dodawania wÅ‚asnych). 
class Statystyka {
    // KtÃ³rej grupy dotyczy statystyka: podatnych, zaraÅ¼onych itp.
    Stan stan;
    std::list<long> grupa;
public:
    Statystyka(const Stan& s) : stan(s) { }

    Stan dotyczy_stanu() const { return stan; }

    void dodaj_dzisiejsze_dane(long ilu) { grupa.push_back(ilu); }

    // Zmiana licznoÅ›ci danej grupy ostatniego dnia.
    long ile_dzisiaj_nowych() const {
        if (grupa.size() == 0) return 0;
        if (grupa.size() == 1) return grupa.back();
        auto ostatni = --grupa.end();
        return *ostatni - *(--ostatni);
    }

    // Maksymalna liczba osobnikÃ³w w danej grupie w czasie trwania eksperymentu.        
    long maksimum() const {
        auto szczytowy_dzien = grupa.begin();
        for (auto dzis = grupa.begin(); dzis != grupa.end(); ++dzis) {
            if (*dzis > *szczytowy_dzien)
                szczytowy_dzien = dzis;
        }
        return *szczytowy_dzien;
    }

    // Na ktÃ³ry dzieÅ„ przypadÅ‚ szczyt licznoÅ›ci grupy
    int kiedy_maksimum() const {
       auto szczytowy_dzien = grupa.begin();
        for (auto dzis = grupa.begin(); dzis != grupa.end(); ++dzis) {
            if (*dzis > *szczytowy_dzien)
                szczytowy_dzien = dzis;
        }
        return std::distance(grupa.begin(), szczytowy_dzien);
    }

    // Zlicz dni, w ktÃ³rych licznoÅ›Ä‡ grupy przekracza podanÄ… wartoÅ›Ä‡
    int ile_dni_powyzej(long ilu) const {
        long ile_dni = 0;
        for (auto dzis = grupa.begin(); dzis != grupa.end(); ++dzis)
            if (*dzis > ilu) ++ile_dni;
        return ile_dni;
    }

    // Od ktÃ³rego dnia licznoÅ›Ä‡ grupy siÄ™ (aÅ¼ do koÅ„ca symulacji)
    // poniÅ¼ej podanej wartoÅ›ci 
    int od_kiedy_ponizej(long ilu) const {
        // BÄ™dziemy siÄ™ cofaÄ‡, poczynajÄ…c od ostatniego dnia
        auto dzis = grupa.rbegin();
        for (; dzis != grupa.rend(); ++dzis)
            if (*dzis >= ilu) break;
        return std::distance(grupa.rbegin(), dzis);
    }

    void wypisz() const {
        for (auto x: grupa)
            std::cout << x << '\t';
        std::cout << std::endl;
    }

    // Zapisuje dane z caÅ‚ego eksperymentu do pliku o podanej nazwie, 
    // oddzielajÄ…c poszczegÃ³lne rekordy znakiem tabulacji i koÅ„czÄ…c 
    // znakiem nowego wiersza.
    // JeÅ›li drugi argument to 'true', nadpisuje plik.
    bool zapisz_do_pliku(const std::string& nazwa_pliku, bool nadpisz=false) const {
        std::ofstream plik;

        if (nadpisz) plik.open(nazwa_pliku);
        else         plik.open(nazwa_pliku, std::ostream::app);

	    if (not plik.is_open()) {
		    std::cout << " Nie mogÄ™ utworzyÄ‡ pliku '" << nazwa_pliku << "`" << std::endl;
		    return false;
        }
        for (auto x: grupa)
            plik << x << '\t';
        plik << std::endl;

        plik.close();
        return true;
	}
};
/********************************************************************************/

int main() {
    // Pierwiastek z liczby osobnikÃ³w (bok kwadratowej siatki).
    // Nie naleÅ¼y baÄ‡ siÄ™ liczb rzÄ™du 1000 (tysiÄ…ca), choÄ‡ 
    // ciekawe ciekawe wyniki moÅ¼na uzyskaÄ‡ i dla 100.
    const int bok_mapy = 1000;

    // Liczba osobnikÃ³w zaraÅ¼onych na poczÄ…tku epidemii.
    const long chorzy_dnia_zero = 500;

    // Liczba osobnikÃ³w zaszczepionych przed nastaniem epidemii.
    const long zaszczepieni_dnia_zero = 10000;

    // PrawdopodobieÅ„stwo zaraÅ¼enia kaÅ¼dego z sÄ…siadÃ³w 
    // danego osobnika w jednostce czasu.
    const float beta = 0.5;

    // PrawdopodobieÅ„stwo wyzdrowienia w jednostce czasu.
    const float gamma = 0.25;

    // Liczba niezaleÅ¼nych (!) eksperymentÃ³w Monte Carlo.
    const int ile_eksperymentow = 20;

    // Ile dni trwa pojedynczy eksperyment.
    const int ile_dni = 120;

    // Jedno miasto posÅ‚uÅ¼y nam do caÅ‚ej serii eksperymentÃ³w Monte Carlo.
    Populacja  miasto(bok_mapy);

    for (int eksp_nr = 0; eksp_nr < ile_eksperymentow; ++eksp_nr) {
        std::cout << "Eksperyment " << eksp_nr+1 << "/" << ile_eksperymentow << std::endl;
 
        Wirus      wirus(beta, gamma);
        Statystyka chorzy(Stan::chory);
        Statystyka podatni(Stan::podatny);
        Statystyka ozdrowiali(Stan::ozdrowialy);

        // Ten krok (reset) jest konieczny! Wszyscy mieszkaÅ„cy stajÄ… siÄ™ na nowo podatni.
        // Stan generatora liczb pseudolosowych NIE jest resetowany, wiÄ™c kolejny eksperyment
        // bÄ™dzie miaÅ‚ inny przebieg niÅ¼ ostatni (i o to chodzi).
        miasto.reset();
        miasto.zaraza_przybywa(wirus, chorzy_dnia_zero, zaszczepieni_dnia_zero);

        // WÅ‚aÅ›ciwy eksperyment odbywa siÄ™ tu.
        for (int dzien = 0; dzien < ile_dni; ++dzien) {
            chorzy.dodaj_dzisiejsze_dane(miasto.ilu_chorych());
            podatni.dodaj_dzisiejsze_dane(miasto.ilu_podatnych());
            ozdrowiali.dodaj_dzisiejsze_dane(miasto.ilu_ozdrowialych());
            miasto.kolejny_dzien();
            if(dzien % 20 == 0){
                miasto.zapisz_do_pliku("wyniki/mapa_" + std::to_string(dzien) + ".txt");
            }
        }

        // UWAGA! Pliki NIE SÄ„ CZYSZCZONE pomiÄ™dzy eksperymentami, co umoÅ¼liwa
        // zgromadzenie w nich wynikÃ³w caÅ‚ej serii eksperymentÃ³w Monte Carlo.
        // Przed rozpoczÄ™ciem nowej serii eksperymentÃ³w zaleca siÄ™ "rÄ™czne" usuniÄ™cie plikÃ³w.
        chorzy.zapisz_do_pliku("wyniki/chorzy_kazdego_dnia.txt");
        podatni.zapisz_do_pliku("wyniki/podatni_kazdego_dnia.txt");
        ozdrowiali.zapisz_do_pliku("wyniki/ozdrowiali_kazdego_dnia.txt");

        // Dla ostatniego z eksperymentÃ³w wypiszemy rÃ³Å¼ne charakterystyczne wielkoÅ›ci
        // (gÅ‚Ã³wnie po to, Å¼eby pokazaÄ‡, jak to siÄ™ robi).
        if (eksp_nr == ile_eksperymentow-1) {            

            // UWAGA! Plik z symbolicznie zapisanymi stanami wszystkich osobnikÃ³w moÅ¼e byÄ‡ duÅ¼y!
            miasto.zapisz_do_pliku("wyniki/mapa_final.txt");

            std::cout << "\n\nPodsumowanie ostatniego z " << ile_eksperymentow << " eksperymentÃ³w Monte Carlo" << std::endl;
            std::cout << "  Szczyt zachorowaÅ„ przypada na dzieÅ„ " << chorzy.kiedy_maksimum() << std::endl;
            std::cout << "  Liczba zaraÅ¼onych w szczycie to " << chorzy.maksimum();
            std::cout << " (" << 100*chorzy.maksimum()/miasto.liczebnosc() << "% populacji)" << std::endl;
            std::cout << "  Liczba zaraÅ¼onych przekraczaÅ‚a 5% populacji przez " 
                      << chorzy.ile_dni_powyzej(long(0.05*miasto.liczebnosc())) << " dni" << std::endl;
            std::cout << "  Liczba zaraÅ¼onych utrzymuje siÄ™ poniÅ¼ej 5% populacji od dnia " 
                      << chorzy.od_kiedy_ponizej(long(0.05*miasto.liczebnosc())) << std::endl;
        }
    }
    return 0;
}

