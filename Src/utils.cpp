#include "../Header/utils.hpp"

// Generatory danych do eksperymentów / innych narzędzi (benchmark w tests.cpp ma własne make_case).

#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

namespace sorting {

namespace {

std::mt19937& random_engine() {
    // Jeden generator współdzielony przez helpery ogranicza koszt ciągłego tworzenia RNG.
    static std::mt19937 engine(std::random_device{}());
    return engine;
}

}  // namespace

std::vector<int> generate_random_data(std::size_t size, int min_value, int max_value) {
    // Rozkład jednostajny na [min_value, max_value]; silnik współdzielony między wywołaniami.
    if (min_value > max_value) {
        throw std::invalid_argument("min_value cannot be greater than max_value");
    }

    std::vector<int> data(size);
    std::uniform_int_distribution<int> distribution(min_value, max_value);

    for (int& value : data) {
        value = distribution(random_engine());
    }

    return data;
}

std::vector<int> generate_partially_sorted_data(std::size_t size, double sorted_fraction) {
    // sorted_fraction × n pierwszych elementów zostaje 0..k-1; reszta tasowana (np. „prawie posortowane”).
    if (sorted_fraction < 0.0 || sorted_fraction > 1.0) {
        throw std::invalid_argument("sorted_fraction must be between 0.0 and 1.0");
    }

    std::vector<int> data(size);
    std::iota(data.begin(), data.end(), 0);

    const std::size_t sorted_count = static_cast<std::size_t>(size * sorted_fraction);
    auto shuffled_begin = data.begin() + static_cast<std::ptrdiff_t>(sorted_count);

    // Zachowujemy posortowany prefiks, a mieszamy tylko pozostałą część tablicy.
    std::shuffle(shuffled_begin, data.end(), random_engine());
    return data;
}

std::vector<int> generate_reverse_sorted_data(std::size_t size) {
    // Ścisłe malejąco: n, n-1, … — częsty worst case dla naiwnego quicksorta (tu nie używane w make_case).
    std::vector<int> data(size);

    for (std::size_t index = 0; index < size; ++index) {
        data[index] = static_cast<int>(size - index);
    }

    return data;
}

}  // namespace sorting
