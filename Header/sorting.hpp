#ifndef SORTING_HPP
#define SORTING_HPP

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
Comparator<T> make_comparator(SortOrder order) {
    if (order == SortOrder::Descending) {
        return std::greater<T>();
    }

    return std::less<T>();
}

template <typename T>
void merge_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending) {
    (void)data;
    (void)order;
}

template <typename T>
void quick_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending) {
    (void)data;
    (void)order;
}

template <typename T>
void intro_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending) {
    (void)data;
    (void)order;
}

}  // namespace sorting

#endif
