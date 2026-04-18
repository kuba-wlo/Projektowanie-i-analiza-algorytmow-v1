#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "../Header/introsort.hpp"
#include "../Header/merge_sort.hpp"
#include "../Header/quicksort.hpp"
#include "../Header/utils.hpp"

struct BenchmarkCase {
    std::string name;
    std::function<std::vector<int>(std::size_t)> generator;
};

template <typename SortFunction>
void run_benchmark(const std::string& algorithm_name,
                   SortFunction sort_function,
                   const std::vector<std::size_t>& sizes,
                   const std::vector<BenchmarkCase>& benchmark_cases) {
    for (const BenchmarkCase& benchmark_case : benchmark_cases) {
        std::cout << '\n' << algorithm_name << " - " << benchmark_case.name << '\n';

        for (const std::size_t size : sizes) {
            std::vector<int> data = benchmark_case.generator(size);
            const double elapsed_ms = sorting::measure_sort_time(
                data,
                [&sort_function](std::vector<int>& values) {
                    sort_function(values);
                });

            const bool sorted_correctly = sorting::is_sorted(data, sorting::SortOrder::Ascending);

            std::cout << "  size = " << std::setw(7) << size
                      << ", time = " << std::setw(10) << std::fixed << std::setprecision(3)
                      << elapsed_ms << " ms"
                      << ", sorted = " << (sorted_correctly ? "yes" : "no") << '\n';
        }
    }
}

int main() {
    const std::vector<std::size_t> sizes = {10000, 50000, 100000, 500000, 1000000};

    const std::vector<BenchmarkCase> benchmark_cases = {
        {"Random data",
         [](std::size_t size) {
             return sorting::generate_random_data(size);
         }},
        {"Partially sorted data",
         [](std::size_t size) {
             return sorting::generate_partially_sorted_data(size, 0.5);
         }},
        {"Reverse sorted data",
         [](std::size_t size) {
             return sorting::generate_reverse_sorted_data(size);
         }}
    };

    run_benchmark(
        "Merge sort",
        [](std::vector<int>& data) {
            sorting::merge_sort(data, sorting::SortOrder::Ascending);
        },
        sizes,
        benchmark_cases);

    run_benchmark(
        "Quicksort",
        [](std::vector<int>& data) {
            sorting::quick_sort(data, sorting::SortOrder::Ascending);
        },
        sizes,
        benchmark_cases);

    run_benchmark(
        "Introsort",
        [](std::vector<int>& data) {
            sorting::intro_sort(data, sorting::SortOrder::Ascending);
        },
        sizes,
        benchmark_cases);

    return 0;
}
