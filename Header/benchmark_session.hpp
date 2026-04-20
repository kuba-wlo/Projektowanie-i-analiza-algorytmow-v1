#pragma once

// Jedna funkcja wejścia: domyślna sesja benchmarku trzech sortowań + ścieżki wygenerowanych CSV.

#include <string>
#include <vector>

#include "tests.hpp"

struct BenchmarkOutputs {
    // Każdy sorter zapisuje wyniki do osobnego pliku CSV.
    std::vector<std::string> csv_paths;
    // Wspólny plik z samymi wierszami average dla wszystkich sorterów.
    std::string averages_csv_path;
};

// Uruchamia standardowy zestaw 3 algorytmów wymaganych w projekcie.
BenchmarkOutputs run_default_benchmark(const TestSettings& cfg,
                                       bool print_human,
                                       StatusCallback status_callback = {});
