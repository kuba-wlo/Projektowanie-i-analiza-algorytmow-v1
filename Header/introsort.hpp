#ifndef INTROSORT_HPP
#define INTROSORT_HPP

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void intro_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

}  // namespace sorting

// Template definitions are included here so the compiler can instantiate them.
#include "../Src/introsort.cpp"

#endif
