#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "ISorter.hpp"

struct TestSettings {
    std::vector<std::size_t> sizes {10'000, 50'000, 100'000, 500'000, 1'000'000};
    int repetitions_per_case = 100;
    unsigned int base_seed = 123456u;
    bool ascending = true;
    std::string csv_path = "Results/results.csv";
};

enum class CaseKind {
    RandomAll,
    Prefix25,
    Prefix50,
    Prefix75,
    Prefix95,
    Prefix99,
    Prefix99_7,
    Reversed
};

struct CaseSpec {
    CaseKind kind;
    const char* name;
};

const std::vector<CaseSpec>& all_cases();

void make_case(int* first, int* last, CaseKind kind, unsigned int seed);

bool is_sorted(const int* first, const int* last, bool ascending);

struct RunArtifacts {
    std::map<std::string, std::unique_ptr<std::ostream>> owned_csv_streams;
    std::map<std::string, std::ostream*> csv_streams;
};

double measure_sort_ms(ISorter& sorter, std::vector<int>& data, bool ascending);
RunArtifacts prepare_outputs(const TestSettings& cfg, bool print_csv);

std::vector<std::string> csv_output_paths(const TestSettings& cfg,
                                          const std::vector<ISorter*>& sorters);

void run_all(const TestSettings& cfg,
             const std::vector<ISorter*>& sorters,
             const std::map<std::string, std::ostream*>& csv_out = {},
             bool print_human = true);
