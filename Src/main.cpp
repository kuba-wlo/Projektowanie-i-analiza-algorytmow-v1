#include <iostream>
#include <vector>

#include "../Header/introsort.hpp"
#include "../Header/merge_sort.hpp"
#include "../Header/quicksort.hpp"
#include "../Header/utils.hpp"

int main() {
    std::vector<int> sample = {7, 2, 9, 1, 5, 3};

    sorting::merge_sort(sample, sorting::SortOrder::Ascending);

    std::cout << "Merge sort result: ";
    for (const int value : sample) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    return 0;
}
