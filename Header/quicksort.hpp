#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void quick_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

} 

// Definicje szablonow sa dolaczone tutaj, aby kompilator mogl je instancjonowac.
#include "../Src/quicksort.cpp"

#endif
