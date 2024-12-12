#include <iostream>
#include <string>
#include "hash_table.h"

hash_table::hash_table() : table_size(INITIAL_TABLE_SIZE), num_elements(0) {
    table = new Node*[table_size]();
}

hash_table::~hash_table() {
    for (int i = 0; i < table_size; ++i) {
        Node* current = table[i];
        while (current) {
            Node* to_delete = current;
            current = current->next;
            delete to_delete;
        }
    }
    delete[] table;
}

std::string hash_table::remove_white_space(const std::string& str) const {
    std::string result;
    for (char ch : str) {
        if (!isspace(ch)) {
            result += ch;
        }
    }
    return result;
}

int hash_table::hashFunction(const Place& p, int size) const {
    int h = 0;
    for (char c : p.city) {
        h = h + c;
    }
    return h % size;
}

void hash_table::resize() {
    int new_table_size = table_size * 2;
    Node** new_table = new Node*[new_table_size]();

    for (int i = 0; i < table_size; ++i) {
        Node* current = table[i];
        while (current != NULL) {
            Node* next = current->next;
            int new_index = hashFunction(current->data, new_table_size);
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }

    delete[] table;
    table = new_table;
    table_size = new_table_size;
}

void hash_table::insert(const Place& p) {
    if (num_elements >= table_size) {
        resize();
    }

    int index = hashFunction(p, table_size);
    Node* new_node = new Node(p);
    new_node->next = table[index];
    table[index] = new_node;
    ++num_elements;
}

Place hash_table::find_by_city_state(const std::string& city, const std::string& state_abbreviation) const {
    const std::string city_no_whitespace = remove_white_space(city);
    for (int i = 0; i < table_size; ++i) {
        Node* current = table[i];
        while (current) {
            if (current->data.city == city_no_whitespace && current->data.state == state_abbreviation) {
                return current->data;
            }
            current = current->next;
        }
    }
    return Place();
}

std::list<Place> hash_table::find_by_city(const std::string& city) const {
    const std::string city_no_whitespace = remove_white_space(city);
    std::list<Place> result;
    for (int i = 0; i < table_size; ++i) {
        Node* current = table[i];
        while (current) {
            if (current->data.city == city_no_whitespace) {
                result.push_back(current->data);
            }
            current = current->next;
        }
    }
    return result;
}

void hash_table::print() const {
    for (int i = 0; i < table_size; ++i) {
        Node* current = table[i];
        while (current) {
            const Place& p = current->data;
            std::cout << "code: " << p.place_id << " state: " << p.state << " city: " << p.city << " population: " << p.
                pop
                << " area: " << p.area << " latitude: " << p.latitude << " longitude: " << p.longitude
                << " intersection: " << p.intersection_code << " distance: " << p.distence << std::endl;
            current = current->next;
        }
    }
}
