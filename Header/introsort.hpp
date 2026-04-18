#ifndef INTROSORT_HPP
#define INTROSORT_HPP

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void intro_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

}  // namespace sorting

// Definicje szablonow sa dolaczone tutaj, aby kompilator mogl je instancjonowac.
#include "../Src/introsort.cpp"

#endif
