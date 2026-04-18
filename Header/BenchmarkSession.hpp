#pragma once

#include <string>
#include <vector>

#include "Tests.hpp"

struct BenchmarkOutputs {
    std::vector<std::string> csv_paths;
};

BenchmarkOutputs run_default_benchmark(const TestSettings& cfg,
                                       bool print_human,
                                       StatusCallback status_callback = {});
