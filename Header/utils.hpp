#ifndef UTILS_HPP
#define UTILS_HPP

#include <functional>
#include <vector>

namespace sorting {

enum class SortOrder {
    Ascending,
    Descending
};

template <typename T>
using Comparator = std::function<bool(const T&, const T&)>;

}  // namespace sorting

#endif
