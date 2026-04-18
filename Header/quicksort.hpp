#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void quick_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

} 

// Definicje szablonów są dołączone tutaj, aby kompilator mógł je instancjonować.
#include "../Src/quicksort.cpp"

#endif
