#include "set.hpp"

// Must initialize reference in the initializer list
set::set(searchable_bag& other_s_bag) : _bag(other_s_bag) {}

set::~set() {}

bool set::has(int value) const{
    return _bag.has(value);
}

void set::insert(int value){
    // Set logic: only insert if it doesn't already exist
    if (!this->has(value)){
        _bag.insert(value);
    }
}

void set::insert(int* data, int size){
    // Re-use our single insert logic to handle the array easily
    for (int i = 0; i < size; i++){
        this->insert(data[i]);
    }
}

void set::print() const{
    _bag.print();
}

void set::clear(){
    _bag.clear();
}

const searchable_bag& set::get_bag() const{
    return _bag;
}