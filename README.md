# Projekt 1 - Analiza i porownanie algorytmow sortowania

## Opis projektu

Projekt zawiera implementacje i benchmark trzech algorytmow sortowania:

- `merge_sort`
- `quick_sort`
- `intro_sort`

Algorytmy zostaly zaimplementowane jako funkcje szablonowe operujace na `std::vector<T>`.
Warstwa benchmarku uruchamia je na danych typu `int`, mierzy czas wykonania, sprawdza
poprawnosc wyniku i zapisuje rezultaty do osobnych plikow CSV.

Program jest uruchamiany z terminala przez `Src/main.cpp` i udostepnia proste menu tekstowe,
ktore pozwala:

- uruchomic pelny benchmark,
- zmienic ustawienia testow,
- podejrzec aktualna konfiguracje,
- wyswietlic skrot wymagan projektu.

## Cel projektu

Celem projektu jest:

- implementacja trzech algorytmow wymaganych w projekcie,
- porownanie ich efektywnosci czasowej,
- sprawdzenie zachowania algorytmow dla roznych typow danych wejsciowych,
- przygotowanie wynikow do dalszej analizy w arkuszu kalkulacyjnym lub sprawozdaniu.

## Zaimplementowane algorytmy

### Merge Sort

Sortowanie przez scalanie z dodatkowym buforem pomocniczym. Algorytm rekurencyjnie dzieli
tablice na dwie polowy, a nastepnie scala posortowane fragmenty.

### Quick Sort

Sortowanie szybkie z wyborem pivota metoda `median of three`. Dla malych zakresow algorytm
przelacza sie na insertion sort, a rekurencja jest prowadzona najpierw po mniejszej partycji,
co zmniejsza narzut i ogranicza wzrost stosu.

### Intro Sort

Algorytm hybrydowy. Dziala jak quicksort, ale pilnuje limitu glebokosci `2 * floor(log2(n))`.
Po wyczerpaniu limitu przelacza sie na heap sort dla biezacego zakresu. Dla malych zakresow
korzysta rowniez z insertion sorta.

## Struktura projektu

Projekt jest podzielony na trzy glowne katalogi:

- `Header/` - pliki naglowkowe
- `Src/` - pliki zrodlowe
- `Results/` - pliki wynikowe CSV

### Najwazniejsze pliki w `Header/`

- `benchmark_session.hpp` - deklaracja uruchomienia pelnej sesji benchmarkowej
- `isorter.hpp` - wspolny interfejs dla sorterow uzywanych przez benchmark
- `tests.hpp` - ustawienia testow, przypadki testowe i deklaracje funkcji pomiarowych
- `merge_sort.hpp` - pelna implementacja merge sorta
- `quicksort.hpp` - pelna implementacja quicksorta
- `introsort.hpp` - pelna implementacja introsorta
- `utils.hpp` - wspolne typy i funkcje pomocnicze, np. `SortOrder`

### Najwazniejsze pliki w `Src/`

- `main.cpp` - menu tekstowe i obsluga uruchamiania programu
- `benchmark_session.cpp` - skladanie listy algorytmow i przygotowanie plikow wynikowych
- `tests.cpp` - glowny silnik benchmarku
- `utils.cpp` - dodatkowe funkcje pomocnicze do generowania danych

Uwaga: implementacje algorytmow szablonowych znajduja sie bezposrednio w plikach `.hpp`.
Projekt nie uzywa juz osobnych plikow `merge_sort.cpp`, `quicksort.cpp` ani `introsort.cpp`.

## Jak dziala program

### 1. Warstwa wejscia

Po uruchomieniu programu uzytkownik dostaje proste menu tekstowe. Z tego poziomu mozna:

- wystartowac benchmark,
- edytowac ustawienia,
- wypisac aktualna konfiguracje,
- sprawdzic skrot wymagan projektu.

### 2. Warstwa benchmarku

Funkcja `run_default_benchmark(...)` tworzy trzy adaptery zgodne z interfejsem `ISorter`
i przekazuje je do wspolnego silnika testowego.

Benchmark:

- generuje dane testowe,
- uruchamia kazdy algorytm na identycznym wejsciu,
- mierzy czas wykonania,
- sprawdza poprawnosc sortowania,
- zapisuje kazdy pomiar do odpowiedniego pliku CSV,
- dopisuje wiersz sredniej dla kazdego przypadku.

### 3. Warstwa wynikow

Kazdy algorytm zapisuje swoje wyniki do osobnego pliku, np.:

- `Results/results-merge_sort.csv`
- `Results/results-quicksort.csv`
- `Results/results-introsort.csv`

Sciezka bazowa jest pobierana z `TestSettings::csv_path`, a nazwy plikow sa budowane
automatycznie przez dopisanie nazwy sortera do nazwy bazowej.

## Przypadki testowe

Program testuje algorytmy dla nastepujacych rodzajow danych:

- `random` - wszystkie elementy losowe,
- `prefix_25%`
- `prefix_50%`
- `prefix_75%`
- `prefix_95%`
- `prefix_99%`
- `prefix_99.7%`
- `reversed` - dane w odwrotnej kolejnosci.

Przypadki `prefix_x%` oznaczaja, ze poczatkowy fragment tablicy pozostaje uporzadkowany,
a tasowana jest tylko koncowka danych.

## Domyslna konfiguracja

Domyslne ustawienia odpowiadaja wymaganiom projektu:

- rozmiary tablic: `10 000`, `50 000`, `100 000`, `500 000`, `1 000 000`
- liczba powtorzen: `100`
- domyslny kierunek sortowania: rosnaco
- domyslna baza plikow wynikowych: `Results/results.csv`

W `main.cpp` domyslny seed dla sesji benchmarkowej jest ustawiony na `20260418`.

## Format wynikow CSV

Program zapisuje dane w formacie:

```text
row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly
trial;merge_sort;10000;random;0;1.234567;1
average;merge_sort;10000;random;-1;1.456789;1
```

Znaczenie kolumn:

- `row_type` - `trial` dla pojedynczego pomiaru albo `average` dla sredniej
- `algorithm` - nazwa algorytmu
- `size` - rozmiar tablicy
- `case_name` - rodzaj danych testowych
- `run_number` - numer powtorzenia
- `time_ms` - czas wykonania w milisekundach
- `sorted_correctly` - poprawnosc wyniku (`1` = poprawny, `0` = blad)

## Wykorzystane technologie

- `C++17` lub nowszy
- `std::vector`
- `std::chrono`
- `std::mt19937`
- `std::shuffle`
- `std::make_heap` i `std::sort_heap`
- programowanie obiektowe w warstwie benchmarku przez `ISorter`
- szablony C++ w implementacjach algorytmow

## Kompilacja

### PowerShell

W katalogu glownym projektu:

```powershell
g++ -std=c++17 -O2 Src\main.cpp Src\benchmark_session.cpp Src\tests.cpp Src\utils.cpp -o projekt.exe
```

Jesli `g++` nie jest dodany do `PATH`, mozna uzyc pelnej sciezki, np.:

```powershell
& "C:\msys64\ucrt64\bin\g++.exe" -std=c++17 -O2 Src\main.cpp Src\benchmark_session.cpp Src\tests.cpp Src\utils.cpp -o projekt.exe
```

### MSYS2 / UCRT64

W terminalu `MSYS2 UCRT64`:

```bash
cd /c/Users/kubaw/Projects/Projektowanie-i-analiza-algorytmow-v1
g++ -std=c++17 -O2 Src/main.cpp Src/benchmark_session.cpp Src/tests.cpp Src/utils.cpp -o projekt.exe
```

## Uruchamianie

Po kompilacji:

```bash
./projekt.exe
```

Program uruchamia menu tekstowe w terminalu.

## Co mozna ustawic z poziomu programu

Z poziomu menu mozna zmienic:

- liczbe powtorzen,
- seed generatora,
- bazowa sciezke plikow CSV,
- kierunek sortowania.

Projekt nie uzywa parsera argumentow wiersza polecen, GUI, `ncurses`, CMake ani zewnetrznych
skryptow do wykresow. To swiadomie prosty wariant: kompilacja, uruchomienie, benchmark, pliki CSV.

## Najwazniejsze cechy projektu

- trzy algorytmy wymagane w projekcie,
- wspolna warstwa benchmarku dla wszystkich sorterow,
- osobne pliki wynikowe dla kazdego algorytmu,
- walidacja poprawnosci po kazdym sortowaniu,
- mozliwosc sortowania rosnaco i malejaco,
- wyniki gotowe do dalszej analizy w sprawozdaniu.

## Podsumowanie

Projekt stanowi kompletny benchmark algorytmow `MergeSort`, `QuickSort` i `IntroSort`
dla danych typu `int`. Laczy implementacje algorytmow, testowanie poprawnosci i pomiar
wydajnosci w jednej prostej strukturze projektu, zgodnej z zalozeniami zadania.
