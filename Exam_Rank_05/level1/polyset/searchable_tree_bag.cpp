#include "searchable_tree_bag.hpp"
#include <cstddef>

searchable_tree_bag::searchable_tree_bag() {}

searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag& src) : tree_bag(src) {}

searchable_tree_bag& searchable_tree_bag::operator=(const searchable_tree_bag& other){

    if (this != &other){
        tree_bag::operator=(other);
    }
    return *this;
}

searchable_tree_bag::~searchable_tree_bag(){}

bool searchable_tree_bag::has(int value) const{
    // Simple iterative binary search (much shorter than recursion)
    node* current = tree;
    while (current != NULL){
        if (current->value == value)
            return true;
        else if (value < current->value)
            current = current->l;
        else
            current = current->r;
    }
    return false;
}