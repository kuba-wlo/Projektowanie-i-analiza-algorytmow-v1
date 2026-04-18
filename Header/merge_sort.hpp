#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void merge_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

}  // namespace sorting

#endif
