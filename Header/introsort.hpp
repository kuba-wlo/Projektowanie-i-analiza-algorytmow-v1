#pragma once

#include <vector>

#include "utils.hpp"

namespace sorting {

template <typename T>
void intro_sort(std::vector<T>& data, SortOrder order = SortOrder::Ascending);

} 

// Definicje szablonów są dołączone tutaj, aby kompilator mógł je instancjonować.
#include "../Src/introsort.cpp"
