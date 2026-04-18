#include <cstddef>
#include <vector>

#include "../Header/utils.hpp"

namespace sorting {

namespace detail {

template <typename T>
bool comes_before(const T& left, const T& right, SortOrder order) {
    if (order == SortOrder::Descending) {
        return left > right;
    }

    return left < right;
}

template <typename T>
void merge_ranges(std::vector<T>& data,
                  std::vector<T>& buffer,
                  std::size_t left,
                  std::size_t middle,
                  std::size_t right,
                  SortOrder order) {
    std::size_t left_index = left;
    std::size_t right_index = middle + 1;
    std::size_t buffer_index = left;

    // Merge two sorted halves into a temporary buffer.
    while (left_index <= middle && right_index <= right) {
        if (comes_before(data[right_index], data[left_index], order)) {
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

    const std::size_t middle = left + (right - left) / 2;

    merge_sort_recursive(data, buffer, left, middle, order);
    merge_sort_recursive(data, buffer, middle + 1, right, order);
    merge_ranges(data, buffer, left, middle, right, order);
}

}  // namespace detail

template <typename T>
void merge_sort(std::vector<T>& data, SortOrder order) {
    if (data.size() < 2) {
        return;
    }

    std::vector<T> buffer(data.size());
    detail::merge_sort_recursive(data, buffer, 0, data.size() - 1, order);
}

}  // namespace sorting
