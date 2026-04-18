#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void quick_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

}  // namespace sorting

// Template definitions are included here so the compiler can instantiate them.
#include "../Src/quicksort.cpp"

#endif
