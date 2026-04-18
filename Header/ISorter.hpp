#pragma once

class ISorter {
public:
    virtual ~ISorter() = default;

    virtual const char* name() const = 0;
    virtual void sort(int* first, int* last, bool ascending) = 0;
};
