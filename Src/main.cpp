#include <iostream>
#include <string>

#include "../Header/benchmark_session.hpp"

namespace {

std::string prompt_line(const std::string& label, const std::string& default_value) {
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

        std::cout << "Podaj dodatnia liczbe calkowita.\n";
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

        std::cout << "Podaj poprawna liczbe bez znaku.\n";
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

}  // namespace

int main() {
    TestSettings settings;
    settings.repetitions_per_case = 100;
    settings.base_seed = 20260418u;
    settings.csv_path = "Results/results.csv";
    settings.ascending = true;

    std::cout << "Pelny benchmark sortowania\n";
    std::cout << "Nacisnij Enter, aby zaakceptowac wartosc domyslna.\n\n";

    settings.repetitions_per_case = prompt_int("Liczba powtorzen", settings.repetitions_per_case);
    settings.base_seed = prompt_unsigned("Seed", settings.base_seed);
    settings.csv_path = prompt_line("Bazowa sciezka CSV", settings.csv_path);
    settings.ascending = !prompt_yes_no("Sortowanie malejace", false);

    std::cout << "\nUruchamiam benchmark...\n";
    run_default_benchmark(settings, true);
    std::cout << "\nGotowe. Wyniki zapisane w katalogu Results.\n";

    return 0;
}
