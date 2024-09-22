#pragma once
#include "place.h"
#include <list>

class hash_table {
protected:
    static constexpr int INITIAL_TABLE_SIZE = 10;
    std::list<place>* table;
    int table_size;
    int num_elements;
    int hashFunction(const place& p) const;
    void resize();

public:
    hash_table();
    ~hash_table();
    void insert(const place& p);
    place find_by_city_state(const std::string& city, const std::string& state_abbreviation) const;
    std::list<place> find_by_city(const std::string& city) const;
    void print() const;
    std::string remove_white_space(const std::string& str) const;
};
