#include <cstddef>
#include <utility>
#include <vector>

#include "../Header/utils.hpp"

namespace sorting {

namespace quicksort_detail {

template <typename T>
std::size_t median_of_three(std::vector<T>& data,
                            std::size_t left,
                            std::size_t right,
                            SortOrder order) {
    const std::size_t middle = left + (right - left) / 2;

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
void quick_sort_recursive(std::vector<T>& data,
                          std::size_t left,
                          std::size_t right,
                          SortOrder order) {
    if (left >= right) {
        return;
    }

    const std::size_t pivot_index = partition(data, left, right, order);

    if (pivot_index > 0) {
        quick_sort_recursive(data, left, pivot_index - 1, order);
    }

    quick_sort_recursive(data, pivot_index + 1, right, order);
}

}  // namespace quicksort_detail

template <typename T>
void quick_sort(std::vector<T>& data, SortOrder order) {
    if (data.size() < 2) {
        return;
    }

    quicksort_detail::quick_sort_recursive(data, 0, data.size() - 1, order);
}

}  // namespace sorting
