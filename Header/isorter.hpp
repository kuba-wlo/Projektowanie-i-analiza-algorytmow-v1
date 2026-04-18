#pragma once

#include <vector>

class ISorter {
public:
    virtual ~ISorter() = default;

    virtual const char* name() const = 0;
    virtual void sort(std::vector<int>& data, bool ascending) = 0;
};
