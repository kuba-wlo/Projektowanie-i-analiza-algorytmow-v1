#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <cstddef>
#include <functional>
#include <vector>

namespace sorting {

enum class SortOrder {
    Ascending,
    Descending
};

template <typename T>
using Comparator = std::function<bool(const T&, const T&)>;

template <typename T>
bool compare_values(const T& left, const T& right, SortOrder order) {
    // Jeden helper obsluguje oba kierunki sortowania.
    if (order == SortOrder::Descending) {
        return left > right;
    }

    return left < right;
}

template <typename T>
bool is_sorted(const std::vector<T>& data, SortOrder order = SortOrder::Ascending) {
    for (std::size_t index = 1; index < data.size(); ++index) {
        if (compare_values(data[index], data[index - 1], order)) {
            return false;
        }
    }

    return true;
}

std::vector<int> generate_random_data(std::size_t size,
                                      int min_value = 0,
                                                      int max_value = 1000000);
std::vector<int> generate_partially_sorted_data(std::size_t size,
                                                double sorted_fraction = 0.5);
std::vector<int> generate_reverse_sorted_data(std::size_t size);

template <typename T, typename SortFunction>
double measure_sort_time(std::vector<T>& data, SortFunction sort_function) {
    // Uzyteczne do prostych lokalnych pomiarow poza glownym benchmarkiem projektu.
    const auto start_time = std::chrono::high_resolution_clock::now();
    sort_function(data);
    const auto end_time = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double, std::milli>(end_time - start_time).count();
}

}  // namespace sorting

#endif
