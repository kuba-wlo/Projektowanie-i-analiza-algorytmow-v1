#include <filesystem>
#include <functional>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "../Header/introsort.hpp"
#include "../Header/merge_sort.hpp"
#include "../Header/quicksort.hpp"
#include "../Header/utils.hpp"

struct BenchmarkCase {
    std::string name;
    std::string file_stub;
    std::function<std::vector<int>(std::size_t)> generator;
};

struct BenchmarkResult {
    std::string algorithm_name;
    std::string case_name;
    std::size_t size;
    int run_number;
    double elapsed_ms;
    bool sorted_correctly;
};

template <typename SortFunction>
std::vector<BenchmarkResult> run_benchmark(const std::string& algorithm_name,
                                           SortFunction sort_function,
                                           const std::vector<std::size_t>& sizes,
                                           const std::vector<BenchmarkCase>& benchmark_cases,
                                           int repetitions) {
    std::vector<BenchmarkResult> results;

    for (const BenchmarkCase& benchmark_case : benchmark_cases) {
        std::cout << '\n' << algorithm_name << " - " << benchmark_case.name << '\n';

        for (const std::size_t size : sizes) {
            for (int run_number = 1; run_number <= repetitions; ++run_number) {
                std::vector<int> data = benchmark_case.generator(size);
                const double elapsed_ms = sorting::measure_sort_time(
                    data,
                    [&sort_function](std::vector<int>& values) {
                        sort_function(values);
                    });

                const bool sorted_correctly =
                    sorting::is_sorted(data, sorting::SortOrder::Ascending);

                std::cout << "  size = " << std::setw(7) << size
                          << ", run = " << std::setw(2) << run_number
                          << ", time = " << std::setw(10) << std::fixed << std::setprecision(3)
                          << elapsed_ms << " ms"
                          << ", sorted = " << (sorted_correctly ? "yes" : "no") << '\n';

                results.push_back(
                    {algorithm_name, benchmark_case.name, size, run_number, elapsed_ms, sorted_correctly});
            }
        }
    }

    return results;
}

void save_results_to_csv(const std::vector<BenchmarkResult>& results,
                         const std::string& output_path) {
    std::filesystem::create_directories(std::filesystem::path(output_path).parent_path());

    std::ofstream output_file(output_path);
    if (!output_file.is_open()) {
        throw std::runtime_error("Could not open CSV file for writing.");
    }

    output_file << "algorithm,data_type,size,run_number,time_ms,sorted_correctly\n";
    output_file << std::fixed << std::setprecision(3);

    for (const BenchmarkResult& result : results) {
        output_file << result.algorithm_name << ','
                    << result.case_name << ','
                    << result.size << ','
                    << result.run_number << ','
                    << result.elapsed_ms << ','
                    << (result.sorted_correctly ? "yes" : "no") << '\n';
    }
}

int main() {
    const std::vector<std::size_t> sizes = {10000, 50000, 100000, 500000, 1000000};
    const int repetitions = 10;
    const std::string output_directory = "Results";
    const std::string output_path = output_directory + "/sorting_results.csv";

    const std::vector<BenchmarkCase> benchmark_cases = {
        {"Random data",
         "random_data",
         [](std::size_t size) {
             return sorting::generate_random_data(size);
         }},
        {"Partially sorted data",
         "partially_sorted_data",
         [](std::size_t size) {
             return sorting::generate_partially_sorted_data(size, 0.5);
         }},
        {"Reverse sorted data",
         "reverse_sorted_data",
         [](std::size_t size) {
             return sorting::generate_reverse_sorted_data(size);
         }}
    };

    std::vector<BenchmarkResult> all_results;

    const std::vector<BenchmarkResult> merge_sort_results = run_benchmark(
        "Merge sort",
        [](std::vector<int>& data) {
            sorting::merge_sort(data, sorting::SortOrder::Ascending);
        },
        sizes,
        benchmark_cases,
        repetitions);
    all_results.insert(all_results.end(), merge_sort_results.begin(), merge_sort_results.end());

    const std::vector<BenchmarkResult> quicksort_results = run_benchmark(
        "Quicksort",
        [](std::vector<int>& data) {
            sorting::quick_sort(data, sorting::SortOrder::Ascending);
        },
        sizes,
        benchmark_cases,
        repetitions);
    all_results.insert(all_results.end(), quicksort_results.begin(), quicksort_results.end());

    const std::vector<BenchmarkResult> introsort_results = run_benchmark(
        "Introsort",
        [](std::vector<int>& data) {
            sorting::intro_sort(data, sorting::SortOrder::Ascending);
        },
        sizes,
        benchmark_cases,
        repetitions);
    all_results.insert(all_results.end(), introsort_results.begin(), introsort_results.end());

    save_results_to_csv(all_results, output_path);
    for (const BenchmarkCase& benchmark_case : benchmark_cases) {
        std::vector<BenchmarkResult> case_results;

        for (const BenchmarkResult& result : all_results) {
            if (result.case_name == benchmark_case.name) {
                case_results.push_back(result);
            }
        }

        const std::string case_output_path =
            output_directory + "/" + benchmark_case.file_stub + "_results.csv";
        save_results_to_csv(case_results, case_output_path);
    }

    std::cout << "\nResults saved to:\n";
    std::cout << "  " << output_path << '\n';
    for (const BenchmarkCase& benchmark_case : benchmark_cases) {
        std::cout << "  "
                  << output_directory + "/" + benchmark_case.file_stub + "_results.csv"
                  << '\n';
    }

    return 0;
}
