#pragma once
#include <vector>

#include "Person.h"

struct Node {
    Person* data;
    Node* left;
    Node* right;

    Node() : data(nullptr), left(nullptr), right(nullptr) {}
    Node(Person* data) : data(data), left(nullptr), right(nullptr) {}
    Node(Person* data, Node* left, Node* right) : data(data), left(left), right(right) {}
};

class BinaryTree {
protected:
    Node* root_;
    Person* search_helper(Node* root, std::string first, std::string last);
    void first_helper(Node* root, std::string first, std::vector<Person*>& first_family);
    void in_order_traversal(Node* root);
    void in_order_traversal(Node* root, int& count);
    Person* search_oldest_helper(const Node* root, Person* oldestPerson);
    void search_family_helper(const Node* root, std::string last, std::vector<Person*>& family);
    void save_helper(const Node* root, std::ofstream& file);
    Node* remove_helper(Node* root, Person* data);

public:
    BinaryTree();
    ~BinaryTree();

    void insert(Person* data);
    void remove(Person* data);
    void check_integrity();
    bool is_bst(Node* node, Person* min, Person* max);
    Person* search(std::string first, std::string last);
    std::vector<Person*> first(std::string first);
    Person* search_oldest();
    std::vector<Person*> search_family(std::string last);
    void relocate_person(std::string first, std::string last, std::string newZip);

    void save(std::string filename);

    void print();
};
