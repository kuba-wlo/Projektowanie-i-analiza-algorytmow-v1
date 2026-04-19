# Projekt 1 – Analiza i porównanie algorytmów sortowania

## Opis projektu

Projekt zawiera implementacje i benchmark trzech algorytmów sortowania:

- `merge_sort`
- `quick_sort`
- `intro_sort`

Algorytmy zostały zaimplementowane jako funkcje szablonowe operujące na `std::vector<T>`.
Warstwa benchmarku uruchamia je na danych typu `int`, mierzy czas wykonania, sprawdza
poprawność wyniku i zapisuje rezultaty do osobnych plików CSV.

Program jest uruchamiany z terminala przez `Src/main.cpp` i udostępnia proste menu tekstowe,
które pozwala:

- uruchomić pełny benchmark,
- zmienić ustawienia testów,
- podejrzeć aktualną konfigurację,
- wyświetlić skrót wymagań projektu.

## Cel projektu

Celem projektu jest:

- implementacja trzech algorytmów wymaganych w projekcie,
- porównanie ich efektywności czasowej,
- sprawdzenie zachowania algorytmów dla różnych typów danych wejściowych,
- przygotowanie wyników do dalszej analizy w arkuszu kalkulacyjnym lub sprawozdaniu.

## Zaimplementowane algorytmy

### Merge Sort

Sortowanie przez scalanie z dodatkowym buforem pomocniczym. Algorytm rekurencyjnie dzieli
tablicę na dwie połowy, a następnie scala posortowane fragmenty.

### Quick Sort

Sortowanie szybkie z wyborem pivota metodą `median of three`. Dla małych zakresów algorytm
przełącza się na insertion sort, a rekurencja jest prowadzona najpierw po mniejszej partycji,
co zmniejsza narzut i ogranicza wzrost stosu.

### Intro Sort

Algorytm hybrydowy. Działa jak quicksort, ale pilnuje limitu głębokości `2 * floor(log2(n))`.
Po wyczerpaniu limitu przełącza się na heap sort dla bieżącego zakresu. Dla małych zakresów
korzysta również z insertion sorta.

## Struktura projektu

Projekt jest podzielony na trzy główne katalogi:

- `Header/` – pliki nagłówkowe
- `Src/` – pliki źródłowe
- `Results/` – pliki wynikowe CSV

### Najważniejsze pliki w `Header/`

- `benchmark_session.hpp` – deklaracja uruchomienia pełnej sesji benchmarkowej
- `isorter.hpp` – wspólny interfejs dla sorterów używanych przez benchmark
- `tests.hpp` – ustawienia testów, przypadki testowe i deklaracje funkcji pomiarowych
- `merge_sort.hpp` – pełna implementacja merge sorta
- `quicksort.hpp` – pełna implementacja quicksorta
- `introsort.hpp` – pełna implementacja introsorta
- `utils.hpp` – wspólne typy i funkcje pomocnicze, np. `SortOrder`

### Najważniejsze pliki w `Src/`

- `main.cpp` – menu tekstowe i obsługa uruchamiania programu
- `benchmark_session.cpp` – składanie listy algorytmów i przygotowanie plików wynikowych
- `tests.cpp` – główny silnik benchmarku
- `utils.cpp` – dodatkowe funkcje pomocnicze do generowania danych

Uwaga: implementacje algorytmów szablonowych znajdują się bezpośrednio w plikach `.hpp`.
Projekt nie używa już osobnych plików `merge_sort.cpp`, `quicksort.cpp` ani `introsort.cpp`.

## Jak działa program

### 1. Warstwa wejścia

Po uruchomieniu programu użytkownik dostaje proste menu tekstowe. Z tego poziomu można:

- wystartować benchmark,
- edytować ustawienia,
- wypisać aktualną konfigurację,
- sprawdzić skrót wymagań projektu.

### 2. Warstwa benchmarku

Funkcja `run_default_benchmark(...)` tworzy trzy adaptery zgodne z interfejsem `ISorter`
i przekazuje je do wspólnego silnika testowego.

Benchmark:

- generuje dane testowe,
- uruchamia każdy algorytm na identycznym wejściu,
- mierzy czas wykonania,
- sprawdza poprawność sortowania,
- zapisuje każdy pomiar do odpowiedniego pliku CSV,
- dopisuje wiersz średniej dla każdego przypadku.

### 3. Warstwa wyników

Każdy algorytm zapisuje swoje wyniki do osobnego pliku, np.:

- `Results/results-merge_sort.csv`
- `Results/results-quicksort.csv`
- `Results/results-introsort.csv`

Ścieżka bazowa jest pobierana z `TestSettings::csv_path`, a nazwy plików są budowane
automatycznie przez dopisanie nazwy sortera do nazwy bazowej.

## Przypadki testowe

Program testuje algorytmy dla następujących rodzajów danych:

- `random` – wszystkie elementy losowe,
- `prefix_25%`
- `prefix_50%`
- `prefix_75%`
- `prefix_95%`
- `prefix_99%`
- `prefix_99.7%`
- `reversed` – dane w odwrotnej kolejności.

Przypadki `prefix_x%` oznaczają, że początkowy fragment tablicy pozostaje uporządkowany,
a tasowana jest tylko końcówka danych.

## Domyślna konfiguracja

Domyślne ustawienia odpowiadają wymaganiom projektu:

- rozmiary tablic: `10 000`, `50 000`, `100 000`, `500 000`, `1 000 000`
- liczba powtórzeń: `100`
- domyślny kierunek sortowania: rosnąco
- domyślna baza plików wynikowych: `Results/results.csv`

W `main.cpp` domyślny seed dla sesji benchmarkowej jest ustawiony na `20260418`.

## Format wyników CSV

Program zapisuje dane w formacie:

```text
row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly
trial;merge_sort;10000;random;0;1.234567;1
average;merge_sort;10000;random;-1;1.456789;1
```

Znaczenie kolumn:

- `row_type` – `trial` dla pojedynczego pomiaru albo `average` dla średniej
- `algorithm` – nazwa algorytmu
- `size` – rozmiar tablicy
- `case_name` – rodzaj danych testowych
- `run_number` – numer powtórzenia
- `time_ms` – czas wykonania w milisekundach
- `sorted_correctly` – poprawność wyniku (`1` = poprawny, `0` = błąd)

## Wykorzystane technologie

- `C++17` lub nowszy
- `std::vector`
- `std::chrono`
- `std::mt19937`
- `std::shuffle`
- `std::make_heap` i `std::sort_heap`
- programowanie obiektowe w warstwie benchmarku przez `ISorter`
- szablony C++ w implementacjach algorytmów

## Kompilacja

### PowerShell

W katalogu głównym projektu:

```powershell
g++ -std=c++17 -O2 Src\main.cpp Src\benchmark_session.cpp Src\tests.cpp Src\utils.cpp -o projekt.exe
```

Jeśli `g++` nie jest dodany do `PATH`, można użyć pełnej ścieżki, np.:

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

## Co można ustawić z poziomu programu

Z poziomu menu można zmienić:

- liczbę powtórzeń,
- seed generatora,
- bazową ścieżkę plików CSV,
- kierunek sortowania.


## Najważniejsze cechy projektu

- trzy algorytmy wymagane w projekcie,
- wspólna warstwa benchmarku dla wszystkich sorterów,
- osobne pliki wynikowe dla każdego algorytmu,
- walidacja poprawności po każdym sortowaniu,
- możliwość sortowania rosnąco i malejąco,
- wyniki gotowe do dalszej analizy w sprawozdaniu.

## Podsumowanie

Projekt stanowi kompletny benchmark algorytmów `MergeSort`, `QuickSort` i `IntroSort`
dla danych typu `int`. Łączy implementacje algorytmów, testowanie poprawności i pomiar
wydajności w jednej prostej strukturze projektu, zgodnej z założeniami zadania.
