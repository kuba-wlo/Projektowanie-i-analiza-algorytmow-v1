# Projekt 1 — analiza i porównanie algorytmów sortowania

## Opis

Program implementuje trzy algorytmy sortowania (**merge sort**, **quicksort**, **introsort**) w postaci szablonów C++ (`template`), tak aby można było sortować `std::vector` różnych typów — w benchmarku używane są tablice `int`.

Dla tych samych danych wejściowych każdy algorytm jest uruchamiany osobno: mierzony jest czas, sprawdzana jest poprawność porządku, a wyniki trafiają do **osobnych plików CSV** (jeden plik na algorytm). Dodatkowo w konsoli widać krótki postęp (rozmiar, przypadek).

Sterowanie odbywa się **interaktywnie z terminala** (`Src/main.cpp`): nie ma osobnego parsera argumentów wiersza poleceń — po starcie program pyta o parametry (Enter = wartość domyślna).

## Struktura katalogów

| Ścieżka | Zawartość |
|--------|-----------|
| `Header/` | Deklaracje i — dla sortowań — dołączone definicje szablonów (`*.hpp` + `#include` odpowiadającego `Src/*.cpp`). |
| `Src/` | Implementacja: `main`, silnik testów, adaptery benchmarku, pliki sortowań używane przez nagłówki. |
| `Results/` | Pliki wynikowe CSV (tworzone przy uruchomieniu). |

### Pliki źródłowe (`Src/`)

- **`main.cpp`** — punkt wejścia, proste pytania w terminalu (powtórzenia, seed, ścieżka CSV, kierunek sortowania), wywołanie benchmarku.
- **`benchmark_session.cpp`** — składa listę algorytmów przez interfejs `ISorter`, przygotowuje strumienie CSV, odpala `run_all`.
- **`tests.cpp`** — generowanie przypadków testowych, pętle po rozmiarach i scenariuszach, pomiar czasu (`std::chrono`), walidacja `is_sorted`, zapis wierszy do CSV.
- **`merge_sort.cpp`**, **`quicksort.cpp`**, **`introsort.cpp`** — logika sortowań (dołączane z nagłówków o tych samych nazwach).
- **`utils.cpp`** — funkcje pomocnicze do danych losowych / częściowo posortowanych (np. pod ewentualne inne testy); sam benchmark w `tests.cpp` buduje tablice inaczej, ale moduł jest w projekcie.

### Nagłówki (`Header/`)

- **`isorter.hpp`** — wspólny interfejs dla adapterów używanych w benchmarku (`sort` na `std::vector<int>`, kierunek przez `bool`).
- **`tests.hpp`** — `TestSettings`, rodzaje przypadków (`CaseKind`), deklaracje silnika testów.
- **`benchmark_session.hpp`** — `run_default_benchmark(...)`.
- **`merge_sort.hpp`**, **`quicksort.hpp`**, **`introsort.hpp`**, **`utils.hpp`** — API sortowań i porównań (`SortOrder`).

## Jak to działa (warstwy)

1. **`main`** ustawia `TestSettings` i wywołuje `run_default_benchmark`.
2. **`benchmark_session`** tworzy adaptery (`merge_sort`, `quicksort`, `introsort`) implementujące `ISorter` i przekazuje je do `run_all` wraz z mapą strumieni CSV.
3. **`tests` / `run_all`** dla każdego rozmiaru tablicy i każdego scenariusza generuje dane (`make_case`), dla każdego powtórzenia kopiuje tablicę do pracy i wywołuje `sort` każdego algorytmu, mierzy czas i sprawdza spójność porządku.

## Zaimplementowane algorytmy (skrót)

- **Merge sort** — dziel i rządź, scalanie dwóch połówek z buforem pomocniczym, stabilny sensownie na poziomie implementacji scalania.
- **Quicksort** — pivot: mediana z trzech, partycja Lomuto (pivot na końcu zakresu), rekurencja po lewej i prawej stronie pivota.
- **Introsort** — quicksort z limitem głębokości \(2 \log_2 n\) (w kodzie: odpowiednik przez dzielenie rozmiaru); po wyczerpaniu limitu **fallback** do sortowania przez kopiec na fragmencie tablicy przy użyciu `std::make_heap` / `std::sort_heap` (kopiec na wybranym zakresie `[left, right]`, nie „cały `std::sort` tablicy”).

## Przypadki testowe i domyślna konfiguracja

Zgodnie z typowym zestawem projektowym:

- rozmiary: `10 000`, `50 000`, `100 000`, `500 000`, `1 000 000`;
- **100 powtórzeń** na każdą kombinację (rozmiar × scenariusz × algorytm w pętli);
- scenariusze: `random`, `prefix_25%` … `prefix_99.7%` (posortowany prefiks, reszta przetasowana), `reversed` (malejąca permutacja przy sortowaniu rosnącym).

Domyślny kierunek w `TestSettings`: **rosnąco**; w `main` można wybrać malejąco.

## Kompilacja

W katalogu głównym projektu (ścieżki jak w repozytorium):

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic ^
  Src/main.cpp Src/benchmark_session.cpp Src/tests.cpp Src/utils.cpp ^
  -o projekt.exe
```

W **bash** (MSYS2, Git Bash) zamień `^` na `\` albo napisz komendę w jednej linii.

Wymagany kompilator z obsługą **C++17** (np. `g++` 9+ / clang z odpowiednim standardem).

## Uruchomienie

```bash
./projekt.exe
```

Program zapyta o:

- liczbę powtórzeń na przypadek,
- seed generatora,
- bazową ścieżkę pliku CSV,
- czy sortowanie ma być malejące.

Pliki wynikowe powstają obok podanej bazy, z dopiskiem `-merge_sort`, `-quicksort`, `-introsort` w nazwie (np. `Results/results-merge_sort.csv`), o ile bazą jest np. `Results/results.csv`.

## Format CSV

Nagłówek (separator `;`):

```text
row_type;algorithm;size;case_name;run_number;time_ms;sorted_correctly
```

- **`row_type`**: `trial` — pojedynczy pomiar, `average` — średnia z powtórzeń dla danego rozmiaru i scenariusza.
- **`sorted_correctly`**: `1` / `0` — wynik walidacji porządku po sortowaniu.

## Technologie

- C++17, `std::vector`, `std::chrono`, `std::mt19937`, `std::shuffle`
- szablony dla algorytmów sortowania; benchmark przez wspólny interfejs `ISorter`

## Uwaga na środowisko

To **zwykła aplikacja konsolowa** (tekst + CSV), bez menu ncurses/TUI i bez przełączników `--reps` itd. — jeśli szukasz takich funkcji, to jest to inny wariant projektu; tutaj konfiguracja jest wyłącznie interaktywna przy starcie.
