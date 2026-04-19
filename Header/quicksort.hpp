#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include "utils.hpp"

namespace sorting {

namespace quicksort_detail {

inline constexpr std::size_t insertion_sort_threshold = 16;

template <typename T>
void insertion_sort_range(std::vector<T>& data,
                          std::size_t left,
                          std::size_t right,
                          SortOrder order) {
    for (std::size_t index = left + 1; index <= right; ++index) {
        T value = std::move(data[index]);
        std::size_t insert_at = index;

        while (insert_at > left && compare_values(value, data[insert_at - 1], order)) {
            data[insert_at] = std::move(data[insert_at - 1]);
            --insert_at;
        }

        data[insert_at] = std::move(value);
    }
}

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
    while (left < right) {
        if (right - left + 1 <= insertion_sort_threshold) {
            insertion_sort_range(data, left, right, order);
            return;
        }

        const std::size_t pivot_index = partition(data, left, right, order);
        const std::size_t left_size = pivot_index > left ? pivot_index - left : 0;
        const std::size_t right_size = right > pivot_index ? right - pivot_index : 0;

        // Recurse into the smaller side first to keep stack growth bounded.
        if (left_size < right_size) {
            if (pivot_index > left) {
                quick_sort_recursive(data, left, pivot_index - 1, order);
            }
            left = pivot_index + 1;
        } else {
            if (pivot_index < right) {
                quick_sort_recursive(data, pivot_index + 1, right, order);
            }

            if (pivot_index == 0) {
                return;
            }

            right = pivot_index - 1;
        }
    }
}

}  // namespace quicksort_detail

template <typename T>
void quick_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending) {
    if (data.size() < 2) {
        return;
    }

    quicksort_detail::quick_sort_recursive(data, 0, data.size() - 1, order);
}

}  // namespace sorting
