#pragma once

// Introsort (std::sort-style): hybryda quicksort + heap sort + insertion sort.
// Limit głębokości 2 * floor(log2 n) ogranicza najgorszy przypadek quicksorta; po jego
// wyczerpaniu sortujemy heap sortem. Krótkie fragmenty domykamy insertion sortem (taniej
// niż rekurencja). Pętla z eliminacją ogona rekurencji ogranicza wzrost stosu (ten sam schemat co w `quick_sort`).

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "utils.hpp"

namespace sorting {

namespace introsort_detail {

// Poniżej tej długości zakresu rekurencja nie opłaca się — O(n²) insertion sorta jest małe n.
inline constexpr std::size_t insertion_sort_threshold = 16;

template <typename T>
void insertion_sort_range(std::vector<T>& data,
                          std::size_t left,
                          std::size_t right,
                          SortOrder order) {
    // Klasyczne wstawianie w miejscu: stabilne dla małych n, bez dodatkowej pamięci.
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
    // Mediana z trzech indeksów (lewy, środek, prawy) — mniej partycji na „trudnych” danych.
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
    // Lomuto: pivot na końcu, elementy „mniejsze od pivotu” zbieramy od lewej.
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
    // std::make_heap / sort_heap na slicie [left, right] — gwarancja O(n log n) na zakresie.
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
    // Zamiast dwóch wywołań rekurencyjnych: jedna rekurencja na mniejszą połowę,
    // druga „ogon” obsługiwana w pętli — głębokość stosu O(log n).
    while (left < right) {
        if (right - left + 1 <= insertion_sort_threshold) {
            insertion_sort_range(data, left, right, order);
            return;
        }

        if (depth_limit == 0) {
            heap_sort_range(data, left, right, order);
            return;
        }

        const std::size_t pivot_index = partition(data, left, right, order);
        const std::size_t left_size = pivot_index > left ? pivot_index - left : 0;
        const std::size_t right_size = right > pivot_index ? right - pivot_index : 0;

        // Najpierw rekurencja w mniejszym podzakresie (ograniczenie stosu), potem kontynuacja.
        if (left_size < right_size) {
            if (pivot_index > left) {
                intro_sort_recursive(data, left, pivot_index - 1, depth_limit - 1, order);
            }
            left = pivot_index + 1;
        } else {
            if (pivot_index < right) {
                intro_sort_recursive(data, pivot_index + 1, right, depth_limit - 1, order);
            }

            if (pivot_index == 0) {
                return;
            }

            right = pivot_index - 1;
        }

        // Każda partycja zużywa jeden „poziom” quicksorta względem limitu introsorta.
        --depth_limit;
    }
}

template <typename T>
int calculate_depth_limit(const std::vector<T>& data) {
    // floor(log2 n) przez dzielenie przez 2; współczynnik 2 jak w typowym introsofcie (np. libc++).
    std::size_t range_size = data.size();
    int depth_limit = 0;

    while (range_size > 1) {
        range_size /= 2;
        ++depth_limit;
    }

    return depth_limit * 2;
}

}  // namespace introsort_detail

template <typename T>
void intro_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending) {
    // Publiczne API: sortuje cały wektor wg SortOrder (domyślnie rosnąco).
    if (data.size() < 2) {
        return;
    }

    const int depth_limit = introsort_detail::calculate_depth_limit(data);
    introsort_detail::intro_sort_recursive(data, 0, data.size() - 1, depth_limit, order);
}

}  // namespace sorting
