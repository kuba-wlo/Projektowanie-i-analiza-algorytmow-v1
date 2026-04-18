#include <iostream>
#include <vector>

#include "../Header/sorting.hpp"

int main() {
    std::vector<int> sample = {5, 2, 4, 1, 3};

    sorting::merge_sort(sample);

    std::cout << "Sorting project skeleton is ready.\n";

    return 0;
}
