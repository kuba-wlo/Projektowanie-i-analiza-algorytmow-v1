#include <iostream>
#include <vector>

#include "../Header/introsort.hpp"
#include "../Header/merge_sort.hpp"
#include "../Header/quicksort.hpp"
#include "../Header/utils.hpp"

int main() {
    std::vector<int> sample = sorting::generate_partially_sorted_data(10, 0.5);

    sorting::intro_sort(sample, sorting::SortOrder::Ascending);

    std::cout << "Introsort result: ";
    for (const int value : sample) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
    std::cout << "Sorted correctly: "
              << (sorting::is_sorted(sample, sorting::SortOrder::Ascending) ? "yes" : "no")
              << '\n';

    return 0;
}
