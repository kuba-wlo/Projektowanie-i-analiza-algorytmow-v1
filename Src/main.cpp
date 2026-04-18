#include <iostream>
#include <string>
#include <vector>

#include "../Header/benchmark_session.hpp"

namespace {

std::string prompt_line(const std::string& label, const std::string& default_value) {
    // Puste wejście oznacza „zostaw wartość domyślną”.
    std::cout << label << " [" << default_value << "]: ";

    std::string input;
    std::getline(std::cin, input);
    if (input.empty()) {
        return default_value;
    }

    return input;
}

int prompt_int(const std::string& label, int default_value) {
    while (true) {
        const std::string input = prompt_line(label, std::to_string(default_value));
        try {
            std::size_t processed = 0;
            const int value = std::stoi(input, &processed);
            if (processed == input.size() && value > 0) {
                return value;
            }
        } catch (...) {
        }

        std::cout << "Podaj dodatnią liczbę całkowitą.\n";
    }
}

unsigned int prompt_unsigned(const std::string& label, unsigned int default_value) {
    while (true) {
        const std::string input = prompt_line(label, std::to_string(default_value));
        try {
            std::size_t processed = 0;
            const unsigned long value = std::stoul(input, &processed);
            if (processed == input.size()) {
                return static_cast<unsigned int>(value);
            }
        } catch (...) {
        }

        std::cout << "Podaj poprawną liczbę bez znaku.\n";
    }
}

bool prompt_yes_no(const std::string& label, bool default_value) {
    const std::string default_text = default_value ? "t" : "n";

    while (true) {
        const std::string input = prompt_line(label + " (t/n)", default_text);

        if (input == "t" || input == "T") {
            return true;
        }

        if (input == "n" || input == "N") {
            return false;
        }

        std::cout << "Wpisz t albo n.\n";
    }
}

void print_separator() {
    std::cout << "\n========================================\n";
}

void print_settings(const TestSettings& settings) {
    print_separator();
    std::cout << "Aktualne ustawienia:\n";
    std::cout << "  - Liczba powtórzeń: " << settings.repetitions_per_case << '\n';
    std::cout << "  - Seed: " << settings.base_seed << '\n';
    std::cout << "  - Bazowa ścieżka CSV: " << settings.csv_path << '\n';
    std::cout << "  - Kierunek sortowania: " << (settings.ascending ? "rosnąco" : "malejąco")
              << '\n';
    std::cout << "  - Rozmiary tablic: ";
    for (std::size_t index = 0; index < settings.sizes.size(); ++index) {
        std::cout << settings.sizes[index];
        if (index + 1 < settings.sizes.size()) {
            std::cout << ", ";
        }
    }
    std::cout << '\n';
    print_separator();
}

void print_requirements() {
    print_separator();
    std::cout << "Wymagania projektu (skrót):\n";
    std::cout << "  - 3 algorytmy: merge sort, quicksort, introsort.\n";
    std::cout << "  - Rozmiary: 10k, 50k, 100k, 500k, 1M.\n";
    std::cout << "  - 100 powtórzeń na przypadek.\n";
    std::cout << "  - Przypadki: random, prefix 25/50/75/95/99/99.7, reversed.\n";
    std::cout << "  - Walidacja poprawności sortowania po każdym uruchomieniu.\n";
    print_separator();
}

void edit_settings(TestSettings& settings) {
    // Menu edycji modyfikuje ten sam obiekt settings, z którego potem startuje benchmark.
    print_separator();
    std::cout << "Edycja ustawień (Enter zostawia bieżącą wartość)\n";
    settings.repetitions_per_case = prompt_int("Liczba powtórzeń", settings.repetitions_per_case);
    settings.base_seed = prompt_unsigned("Seed", settings.base_seed);
    settings.csv_path = prompt_line("Bazowa ścieżka CSV", settings.csv_path);
    settings.ascending = !prompt_yes_no("Sortowanie malejące", !settings.ascending);
    print_separator();
}

int prompt_menu_choice() {
    while (true) {
        const std::string input = prompt_line("Wybierz opcje", "1");
        try {
            std::size_t processed = 0;
            const int value = std::stoi(input, &processed);
            if (processed == input.size() && value >= 1 && value <= 5) {
                return value;
            }
        } catch (...) {
        }
        std::cout << "Wpisz liczbę od 1 do 5.\n";
    }
}

void show_main_menu() {
    print_separator();
    std::cout << "Benchmark sortowania - proste TUI\n";
    std::cout << "  1) Start benchmarku\n";
    std::cout << "  2) Ustawienia benchmarku\n";
    std::cout << "  3) Pokaż aktualne ustawienia\n";
    std::cout << "  4) Pokaż wymagania projektu\n";
    std::cout << "  5) Wyjdź\n";
    print_separator();
}

}  // namespace

int main() {
    TestSettings settings;
    settings.repetitions_per_case = 100;
    settings.base_seed = 20260418u;
    settings.csv_path = "Results/results.csv";
    settings.ascending = true;

    bool running = true;
    while (running) {
        // Proste TUI pozwala uruchomić testy bez zmiany kodu i rekompilacji.
        show_main_menu();
        const int choice = prompt_menu_choice();

        switch (choice) {
            case 1:
                print_settings(settings);
                std::cout << "Uruchamiam benchmark...\n";
                run_default_benchmark(settings, true);
                std::cout << "\nGotowe. Wyniki zapisane w katalogu Results.\n";
                break;
            case 2:
                edit_settings(settings);
                break;
            case 3:
                print_settings(settings);
                break;
            case 4:
                print_requirements();
                break;
            case 5:
                running = false;
                break;
            default:
                break;
        }
    }

    return 0;
}
