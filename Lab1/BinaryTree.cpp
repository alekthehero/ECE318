#include "BinaryTree.h"
#include <fstream>
#include <iomanip>
#include <iostream>

BinaryTree::BinaryTree() {
    root_ = nullptr;
}

BinaryTree::~BinaryTree() {
    delete root_;
}

void BinaryTree::insert(Person* data) {
    if (root_ == nullptr) {
        root_ = new Node(data);
        return;
    }

    Node* current = root_;

    while (true) {
        if (*data < *(current->data)) {
            if (current->left == nullptr) {
                current->left = new Node(data);
                return;
            }
            current = current->left;
        }
        else if (*(current->data) < *data) {
            if (current->right == nullptr) {
                current->right = new Node(data);
                return;
            }
            current = current->right;
        }
        else {
            return;
        }
    }
}

void BinaryTree::remove(Person* data) {
    //To remove a big node, go right one then left as far as you can, remove main then replace with found
    root_ = remove_helper(root_, data);
}

Node* BinaryTree::remove_helper(Node* root, Person* data) {
    if (root == nullptr) {
        return root;
    }

    if (*data < *(root->data)) {
        root->left = remove_helper(root->left, data);
    }
    else if (*(root->data) < *data) {
        root->right = remove_helper(root->right, data);
    }
    else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = root->right;
        while (temp->left != nullptr) {
            temp = temp->left;
        }

        root->data = temp->data;
        root->right = remove_helper(root->right, temp->data);
    }

    return root;
}

void BinaryTree::check_integrity() {
    if (is_bst(root_, nullptr, nullptr)) {
        std::cout << "Tree is in good condition." << std::endl;
    }
    else {
        std::cout << "Tree integrity check failed." << std::endl;
    }
}

bool BinaryTree::is_bst(Node* node, Person* min, Person* max) {
    if (node == nullptr) {
        return true;
    }

    if ((min != nullptr && *(node->data) <= *min) || (max != nullptr && *(node->data) >= *max)) {
        return false;
    }

    return is_bst(node->left, min, node->data) && is_bst(node->right, node->data, max);
}

Person* BinaryTree::search(std::string first, std::string last) {
    return search_helper(root_, first, last);
}

Person* BinaryTree::search_helper(Node* root, std::string first, std::string last) {
    if (root == nullptr || (root->data->first == first && root->data->last == last)) {
        return (root != nullptr) ? root->data : nullptr;
    }

    if (last < root->data->last || (last == root->data->last && first < root->data->first)) {
        return search_helper(root->left, first, last);
    }
    return search_helper(root->right, first, last);
}

std::vector<Person*> BinaryTree::first(std::string first) {
    std::vector<Person*> first_family;
    first_helper(root_, first, first_family);
    return first_family;
}

void BinaryTree::first_helper(Node* root, std::string first, std::vector<Person*>& first_family) {
    if (root != nullptr) {
        first_helper(root->left, first, first_family);
        if (root->data->first == first) {
            first_family.push_back(root->data);
        }
        first_helper(root->right, first, first_family);
    }
}

void BinaryTree::in_order_traversal(Node* root) {
    if (root != nullptr) {
        in_order_traversal(root->left);
        int birthYear = root->data->birthDate / 10000;
        int birthMonth = (root->data->birthDate % 10000) / 100;
        int birthDay = root->data->birthDate % 100;
        std::cout
            << root->data->first << " "
            << root->data->last << " "
            << root->data->state << " "
            << root->data->zip << " "
            << birthDay << " "
            << birthMonth << " "
            << birthYear << " "
            << root->data->secret << " "
            << root->data->balance << " "
            << root->data->social
            << std::endl;
        in_order_traversal(root->right);
    }
}

void BinaryTree::in_order_traversal(Node* root, int& count) {
    if (root != nullptr) {
        in_order_traversal(root->left, count);
        int birthYear = root->data->birthDate / 10000;
        int birthMonth = (root->data->birthDate % 10000) / 100;
        int birthDay = root->data->birthDate % 100;
        std::cout
            << root->data->first << " "
            << root->data->last << " "
            << root->data->state << " "
            << root->data->zip << " "
            << birthDay << " "
            << birthMonth << " "
            << birthYear << " "
            << root->data->secret << " "
            << root->data->balance << " "
            << root->data->social
            << std::endl;
        count = count + 1;
        in_order_traversal(root->right, count);
    }
}

void BinaryTree::save(std::string filename) {
    std::ofstream outFile(filename + ".txt");

    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        return;
    }
    save_helper(root_, outFile);
    outFile.close();

    std::cout << "Saved to " << filename << std::endl;
}

void BinaryTree::print() {
    int count = 0;
    in_order_traversal(root_, count);
    std::cout << "Printed " << count << " records." << std::endl;
}

Person* BinaryTree::search_oldest_helper(const Node* root, Person* oldestPerson) {
    if (root != nullptr) {
        oldestPerson = search_oldest_helper(root->left, oldestPerson);

        if (oldestPerson == nullptr || root->data->birthDate < oldestPerson->birthDate) {
            oldestPerson = root->data;
        }

        oldestPerson = search_oldest_helper(root->right, oldestPerson);
    }

    return oldestPerson;
}

void BinaryTree::search_family_helper(const Node* root, std::string last, std::vector<Person*>& family) {
    //Find all people with the same last name
    if (root == nullptr || root->data->last == last) {
        if (root != nullptr) {
            family.push_back(root->data);
        }
        else {
            return;
        }
    }

    if (last < root->data->last || last == root->data->last) {
        search_family_helper(root->left, last, family);
    }
    search_family_helper(root->right, last, family);
}

void BinaryTree::save_helper(const Node* root, std::ofstream& file) {
    if (root != nullptr) {
        save_helper(root->left, file);
        int birthYear = root->data->birthDate / 10000;
        int birthMonth = (root->data->birthDate % 10000) / 100;
        int birthDay = root->data->birthDate % 100;
        file << std::setfill('0') << root->data->first << " " << root->data->last << " " << root->data->state << " "
            << root->data->zip << " " << std::setw(2) << std::right << birthDay << " " << std::setw(2) << std::right
            << birthMonth << " " << std::setw(4) << std::right << birthYear << " " << root->data->secret << " "
            << root->data->balance << " " << std::setw(9) << std::right << root->data->social
            << std::endl;
        save_helper(root->right, file);
    }
}

Person* BinaryTree::search_oldest() {
    return search_oldest_helper(root_, nullptr);
}

std::vector<Person*> BinaryTree::search_family(std::string last) {
    std::vector<Person*> family;
    search_family_helper(root_, last, family);
    return family;
}

void BinaryTree::relocate_person(std::string first, std::string last, std::string newZip) {
    Person* person = search(first, last);
    if (person != nullptr) {
        person->zip = newZip;
        std::cout << "Relocated " << first << " " << last << " to " << newZip << std::endl;
    }
    else {
        std::cout << "No person found" << std::endl;
    }
}
