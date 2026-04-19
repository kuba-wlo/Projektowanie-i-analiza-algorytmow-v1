#pragma once

// Merge sort: dziel i rządź, scalanie dwóch posortowanych połówek w buforze pomocniczym O(n).

#include <cstddef>
#include <vector>

#include "utils.hpp"

namespace sorting {

namespace merge_sort_detail {

template <typename T>
void merge_ranges(std::vector<T>& data,
                  std::vector<T>& buffer,
                  std::size_t left,
                  std::size_t middle,
                  std::size_t right,
                  SortOrder order) {
    // Dwuwskaźnikowe scalanie [left, middle] i [middle+1, right] do buffer, potem kopiuj z powrotem.
    std::size_t left_index = left;
    std::size_t right_index = middle + 1;
    std::size_t buffer_index = left;

    while (left_index <= middle && right_index <= right) {
        if (compare_values(data[right_index], data[left_index], order)) {
            buffer[buffer_index++] = data[right_index++];
        } else {
            buffer[buffer_index++] = data[left_index++];
        }
    }

    while (left_index <= middle) {
        buffer[buffer_index++] = data[left_index++];
    }

    while (right_index <= right) {
        buffer[buffer_index++] = data[right_index++];
    }

    for (std::size_t index = left; index <= right; ++index) {
        data[index] = buffer[index];
    }
}

template <typename T>
void merge_sort_recursive(std::vector<T>& data,
                          std::vector<T>& buffer,
                          std::size_t left,
                          std::size_t right,
                          SortOrder order) {
    if (left >= right) {
        return;
    }

    // Środek zakresu, potem scalenie dwóch już posortowanych fragmentów.
    const std::size_t middle = left + (right - left) / 2;
    merge_sort_recursive(data, buffer, left, middle, order);
    merge_sort_recursive(data, buffer, middle + 1, right, order);
    merge_ranges(data, buffer, left, middle, right, order);
}

}  // namespace merge_sort_detail

template <typename T>
void merge_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending) {
    // Jeden bufor rozmiaru n na całe sortowanie — O(n) pamięci dodatkowej.
    if (data.size() < 2) {
        return;
    }

    std::vector<T> buffer(data.size());
    merge_sort_detail::merge_sort_recursive(data, buffer, 0, data.size() - 1, order);
}

}  // namespace sorting
