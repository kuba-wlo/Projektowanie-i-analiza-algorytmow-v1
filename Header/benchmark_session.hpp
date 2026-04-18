#pragma once

#include <string>
#include <vector>

#include "tests.hpp"

struct BenchmarkOutputs {
    // Kazdy sorter zapisuje wyniki do osobnego pliku CSV.
    std::vector<std::string> csv_paths;
};

// Uruchamia standardowy zestaw 3 algorytmow wymaganych w projekcie.
BenchmarkOutputs run_default_benchmark(const TestSettings& cfg,
                                       bool print_human,
                                       StatusCallback status_callback = {});
