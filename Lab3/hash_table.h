#pragma once
#include <list>
#include "definitions.h"

class hash_table {
protected:
    static constexpr int INITIAL_TABLE_SIZE = 10;

    struct Node {
        Place data;
        Node* next;
        Node(const Place& p) : data(p), next(nullptr) {}
    };

    Node** table;
    int table_size;
    int hashFunction(const Place& p, int size) const;
    void resize();

public:
    hash_table();
    ~hash_table();
    int num_elements;
    void insert(const Place& p);
    Place find_by_city_state(const std::string& city, const std::string& state_abbreviation) const;
    std::list<Place> find_by_city(const std::string& city) const;
    void print() const;
    std::string remove_white_space(const std::string& str) const;
};
