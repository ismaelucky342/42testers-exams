#pragma once

#include "searchable_bag.hpp"

class set {
    private:
        searchable_bag& _bag;

        // C++98 compliant way to forbid copying and default initialization
        set();
        set(const set& src);
        set& operator=(const set& other);

    public:
        set(searchable_bag& other_s_bag);
        ~set();

        bool has(int value) const;
        void insert(int value);
        void insert(int* data, int size);
        void print() const;
        void clear();

        const searchable_bag& get_bag() const; // Added const to the function signature
};