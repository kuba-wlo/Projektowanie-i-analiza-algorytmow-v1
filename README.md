# Projekt 1 - Analiza i porównanie algorytmów sortowania

## Opis projektu

Projekt zawiera implementację i porównanie trzech algorytmów sortowania:

- `merge_sort`
- `quick_sort`
- `intro_sort`

Algorytmy zostały zaimplementowane samodzielnie w C++ jako funkcje szablonowe operujące na `std::vector<T>`. Warstwa benchmarku uruchamia je na danych typu `int`, mierzy czas wykonania, sprawdza poprawność wyniku i zapisuje rezultaty do osobnych plików CSV.

Program działa bezpośrednio w terminalu z poziomu `Src/main.cpp`. Po uruchomieniu pojawia się proste menu tekstowe, które pozwala:

- uruchomić pełny benchmark,
- zmienić ustawienia testów,
- podejrzeć aktualną konfigurację,
- sprawdzić skrót wymagań projektu.

## Cel projektu

Celem projektu jest:

- implementacja trzech algorytmów wymaganych dla oceny `5,0`,
- porównanie ich efektywności czasowej,
- sprawdzenie zachowania algorytmów dla różnych typów danych wejściowych,
- przygotowanie wyników do dalszej analizy w arkuszu kalkulacyjnym lub sprawozdaniu.

## Zaimplementowane algorytmy

### Merge Sort

Sortowanie przez scalanie z dodatkowym buforem pomocniczym. Algorytm rekurencyjnie dzieli tablicę na dwie połowy, a następnie scala posortowane fragmenty w jedną całość.

### Quick Sort

Sortowanie szybkie z wyborem pivota metodą `median of three`. Dane są dzielone względem elementu osiowego, a następnie osobno sortowane są lewa i prawa część tablicy.

### Intro Sort

Algorytm hybrydowy. Działa jak quicksort, ale pilnuje limitu głębokości rekurencji. Po przekroczeniu tego limitu przełącza się na sortowanie przez kopcowanie dla bieżącego fragmentu tablicy.

## Struktura projektu

Projekt jest podzielony na trzy główne katalogi:

- **`Header/`** - pliki nagłówkowe
- **`Src/`** - pliki źródłowe
- **`Results/`** - pliki wynikowe CSV

### Najważniejsze pliki w `Header/`

- `benchmark_session.hpp` - deklaracja uruchomienia pełnej sesji benchmarkowej
- `isorter.hpp` - wspólny interfejs dla sorterów używanych przez benchmark
- `tests.hpp` - struktury ustawień, przypadki testowe i deklaracje funkcji pomiarowych
- `merge_sort.hpp` - deklaracja sortowania przez scalanie
- `quicksort.hpp` - deklaracja quicksorta
- `introsort.hpp` - deklaracja introsorta
- `utils.hpp` - funkcje pomocnicze i wspólne typy, np. `SortOrder`

### Najważniejsze pliki w `Src/`

- `main.cpp` - menu tekstowe i obsługa uruchamiania programu
- `benchmark_session.cpp` - składanie listy algorytmów i przygotowanie plików wynikowych
- `tests.cpp` - główny silnik benchmarku
- `merge_sort.cpp` - implementacja merge sorta
- `quicksort.cpp` - implementacja quicksorta
- `introsort.cpp` - implementacja introsorta
- `utils.cpp` - dodatkowe funkcje pomocnicze do generowania danych

## Jak działa program

### 1. Warstwa wejściowa

Po uruchomieniu programu użytkownik dostaje proste menu tekstowe. Z tego poziomu można:

- wystartować benchmark,
- edytować ustawienia,
- wypisać aktualną konfigurację,
- sprawdzić skrót wymagań projektu.

### 2. Warstwa benchmarku

Funkcja `run_default_benchmark(...)` tworzy trzy adaptery zgodne z interfejsem `ISorter` i przekazuje je do wspólnego silnika testowego.

Benchmark:

- generuje dane testowe,
- uruchamia każdy algorytm na identycznym wejściu,
- mierzy czas wykonania,
- sprawdza poprawność sortowania,
- zapisuje każdy pomiar do odpowiedniego pliku CSV,
- dopisuje także wiersz średniej dla każdego przypadku.

### 3. Warstwa wyników

Każdy algorytm zapisuje swoje wyniki do osobnego pliku, np.:

- `Results/results-merge_sort.csv`
- `Results/results-quicksort.csv`
- `Results/results-introsort.csv`

## Przypadki testowe

Program testuje algorytmy dla następujących rodzajów danych:

- `random` - wszystkie elementy losowe,
- `prefix_25%`
- `prefix_50%`
- `prefix_75%`
- `prefix_95%`
- `prefix_99%`
- `prefix_99.7%`
- `reversed` - dane w odwrotnej kolejności.

Przypadki `prefix_x%` oznaczają, że początkowy fragment tablicy jest już posortowany, a pozostała część zostaje przetasowana.

## Domyślna konfiguracja

Domyślne ustawienia odpowiadają wymaganiom projektu:

- rozmiary tablic:
  - `10 000`
  - `50 000`
  - `100 000`
  - `500 000`
  - `1 000 000`
- liczba powtórzeń:
  - `100`
- domyślny kierunek sortowania:
  - rosnący
- domyślna baza plików wynikowych:
  - `Results/results.csv`

Na podstawie tej bazy program tworzy osobne pliki dla każdego algorytmu.

## Format wyników CSV

Program zapisuje dane w formacie:

```text
row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly
trial;merge_sort;10000;random;0;1.234567;1
average;merge_sort;10000;random;-1;1.456789;1
```

Znaczenie kolumn:

- `row_type` - `trial` dla pojedynczego pomiaru albo `average` dla średniej,
- `algorithm` - nazwa algorytmu,
- `size` - rozmiar tablicy,
- `case_name` - rodzaj danych testowych,
- `run_number` - numer powtórzenia,
- `time_ms` - czas wykonania w milisekundach,
- `sorted_correctly` - poprawność wyniku (`1` = poprawny, `0` = błąd).

## Wykorzystane technologie

- `C++17` lub nowszy
- `std::vector`
- `std::chrono`
- `std::mt19937`
- `std::shuffle`
- programowanie obiektowe w warstwie benchmarku przez `ISorter`
- szablony C++ w implementacjach algorytmów

## Kompilacja

### PowerShell

W katalogu głównym projektu:

```powershell
g++ -std=c++20 -O2 Src\main.cpp Src\benchmark_session.cpp Src\tests.cpp Src\utils.cpp -o projekt.exe
```

Jeśli `g++` nie jest dodany do `PATH`, można użyć pełnej ścieżki, np.:

```powershell
& "C:\msys64\ucrt64\bin\g++.exe" -std=c++20 -O2 Src\main.cpp Src\benchmark_session.cpp Src\tests.cpp Src\utils.cpp -o projekt.exe
```

### MSYS2 / UCRT64

W terminalu `MSYS2 UCRT64`:

```bash
cd /c/Users/kubaw/Projects/Projektowanie-i-analiza-algorytmow-v1
g++ -std=c++20 -O2 Src/main.cpp Src/benchmark_session.cpp Src/tests.cpp Src/utils.cpp -o projekt.exe
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

Nie ma tu parsera argumentów wiersza poleceń, trybu GUI, `ncurses`, CMake ani zewnętrznych skryptów do wykresów. Ten wariant projektu jest świadomie prosty: kompilacja, uruchomienie, benchmark, pliki CSV.

## Najważniejsze cechy projektu

- trzy algorytmy wymagane dla projektu na `5,0`,
- wspólna warstwa benchmarku dla wszystkich sorterów,
- osobne pliki wynikowe dla każdego algorytmu,
- walidacja poprawności po każdym sortowaniu,
- możliwość sortowania rosnąco i malejąco,
- dane gotowe do dalszej analizy w sprawozdaniu.

## Podsumowanie

Projekt stanowi kompletny benchmark algorytmów `MergeSort`, `QuickSort` i `IntroSort` dla danych typu `int`. Łączy implementację algorytmów, testowanie poprawności i pomiar wydajności w jednej prostej strukturze projektu, zgodnej z założeniami zadania z Projektowania i Analizy Algorytmów.
