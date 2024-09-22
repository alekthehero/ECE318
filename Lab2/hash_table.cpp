#include <iostream>
#include <string>
#include "hash_table.h"

hash_table::hash_table() : table_size(INITIAL_TABLE_SIZE), num_elements(0) {
    table = new std::list<place>[table_size];
}

hash_table::~hash_table() {
    delete[] table;
}

std::string hash_table::remove_white_space(const std::string& str) const {
    std::string resut;
    for (char ch : str) {
        if (!isspace(ch)) {
            resut += ch;
        }
    }
    return resut;
}

int hash_table::hashFunction(const place& p) const {
    place_hash hashFunc;
    return hashFunc(p) % table_size;
}

void hash_table::resize() {
    int new_table_size = table_size * 2;
    std::list<place>* new_table = new std::list<place>[new_table_size];

    for (int i = 0; i < table_size; ++i) {
        for (const place& p : table[i]) {
            int new_index = place_hash()(p) % new_table_size;
            new_table[new_index].push_back(p);
        }
    }

    delete[] table;
    table = new_table;
    table_size = new_table_size;
}

void hash_table::insert(const place& p) {
    if (num_elements >= table_size) {
        resize();
    }

    int index = hashFunction(p);
    table[index].push_back(p);
    ++num_elements;
}

place hash_table::find_by_city_state(const std::string& city, const std::string& state_abbreviation) const {
    const std::string city_no_whitespace = remove_white_space(city);
    for (int i = 0; i < table_size; ++i) {
        for (const place& place : table[i]) {
            if (place.city == city_no_whitespace && place.state == state_abbreviation) {
                return place;
            }
        }
    }
    return place();
}

std::list<place> hash_table::find_by_city(const std::string& city) const {
    const std::string city_no_whitespace = remove_white_space(city);
    std::list<place> result;
    for (int i = 0; i < table_size; ++i) {
        for (const place& p : table[i]) {
            if (p.city == city_no_whitespace) {
                result.push_back(p);
            }
        }
    }
    return result;
}

void hash_table::print() const {
    for (int i = 0; i < table_size; ++i) {
        for (const place& p : table[i]) {
            std::cout << "code: " << p.code << " state: " << p.state << " city: " << p.city << " population: " << p.
                population
                << " area: " << p.area << " latitude: " << p.latitude << " longitude: " << p.longitude
                << " intersection: " << p.intersection << " distance: " << p.distance << std::endl;
        }
    }
}
