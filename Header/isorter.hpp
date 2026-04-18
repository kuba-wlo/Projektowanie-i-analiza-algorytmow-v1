#pragma once

#include <vector>

class ISorter {
public:
    virtual ~ISorter() = default;

    // Benchmark zna sortery tylko przez te 2 metody.
    virtual const char* name() const = 0;
    virtual void sort(std::vector<int>& data, bool ascending) = 0;
};
