#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void merge_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

} 

// Definicje szablonów są dołączone tutaj, aby kompilator mógł je instancjonować.
#include "../Src/merge_sort.cpp"

#endif
