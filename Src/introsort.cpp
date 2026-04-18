#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "../Header/utils.hpp"

namespace sorting {

namespace introsort_detail {

template <typename T>
std::size_t median_of_three(std::vector<T>& data,
                            std::size_t left,
                            std::size_t right,
                            SortOrder order) {
    const std::size_t middle = left + (right - left) / 2;

    // Ten sam wybor pivota co w quicksort, ale introsort ma dodatkowy bezpiecznik glebi.
    if (compare_values(data[middle], data[left], order)) {
        std::swap(data[left], data[middle]);
    }

    if (compare_values(data[right], data[left], order)) {
        std::swap(data[left], data[right]);
    }

    if (compare_values(data[right], data[middle], order)) {
        std::swap(data[middle], data[right]);
    }

    return middle;
}

template <typename T>
std::size_t partition(std::vector<T>& data,
                      std::size_t left,
                      std::size_t right,
                      SortOrder order) {
    const std::size_t pivot_index = median_of_three(data, left, right, order);
    const T pivot_value = data[pivot_index];
    std::swap(data[pivot_index], data[right]);

    std::size_t smaller_index = left;

    for (std::size_t current = left; current < right; ++current) {
        if (!compare_values(pivot_value, data[current], order)) {
            std::swap(data[smaller_index], data[current]);
            ++smaller_index;
        }
    }

    std::swap(data[smaller_index], data[right]);
    return smaller_index;
}

template <typename T>
void heap_sort_range(std::vector<T>& data,
                     std::size_t left,
                     std::size_t right,
                     SortOrder order) {
    // Sortujemy tylko wycinek [left, right], nie cala tablice.
    auto begin = data.begin() + static_cast<std::ptrdiff_t>(left);
    auto end = data.begin() + static_cast<std::ptrdiff_t>(right + 1);

    auto comparator = [order](const T& left_value, const T& right_value) {
        return compare_values(left_value, right_value, order);
    };

    std::make_heap(begin, end, comparator);
    std::sort_heap(begin, end, comparator);
}

template <typename T>
void intro_sort_recursive(std::vector<T>& data,
                          std::size_t left,
                          std::size_t right,
                          int depth_limit,
                          SortOrder order) {
    if (left >= right) {
        return;
    }

    if (depth_limit == 0) {
        // Przelaczenie na heap sort chroni przed pesymistycznym przypadkiem quicksorta.
        heap_sort_range(data, left, right, order);
        return;
    }

    // Dopoki limit glebi nie zostal wyczerpany, introsort zachowuje sie jak quicksort.
    const std::size_t pivot_index = partition(data, left, right, order);

    if (pivot_index > 0) {
        intro_sort_recursive(data, left, pivot_index - 1, depth_limit - 1, order);
    }

    intro_sort_recursive(data, pivot_index + 1, right, depth_limit - 1, order);
}

template <typename T>
int calculate_depth_limit(const std::vector<T>& data) {
    std::size_t range_size = data.size();
    int depth_limit = 0;

    while (range_size > 1) {
        range_size /= 2;
        ++depth_limit;
    }

    // Klasyczny limit introsort: 2 * floor(log2(n)).
    return depth_limit * 2;
}

}  // namespace introsort_detail

template <typename T>
void intro_sort(std::vector<T>& data, SortOrder order) {
    if (data.size() < 2) {
        return;
    }

    const int depth_limit = introsort_detail::calculate_depth_limit(data);
    introsort_detail::intro_sort_recursive(data, 0, data.size() - 1, depth_limit, order);
}

}  // namespace sorting
