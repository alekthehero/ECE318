#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "BinaryTree.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1; // Exit with an error code
    }

    const std::string filename = argv[1];

    std::fstream file(filename, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return 1;
    }

    BinaryTree tree;

    int count = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        std::istringstream iss(line);
        std::string firstName, lastName, zip, state, secret;
        double balance;
        int social, birthDay, birthMonth, birthYear;
        iss >> firstName >> lastName >> state >> zip >> birthDay >> birthMonth >> birthYear >> secret >> balance >>
            social;

        int birthDate = birthYear * 10000 + birthMonth * 100 + birthDay;
        auto person = new Person(social, birthDate, firstName, lastName, zip, balance, secret, state);

        tree.insert(person);
        count++;
    }

    file.close();
    std::cout << "Loaded " << count << " records." << std::endl;

    while (true) {
        std::cout <<
            "Enter command: FIND <first> <last>, FAMILY <last>, FIRST <first> OLDEST, RELOCATE <first> <last> <new zip>, DELETE <fist> <last>,\n"
            "PRINT, SAVE <file name>, EXIT\n";
        std::string line, command, param1, param2, param3;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        iss >> command >> param1 >> param2 >> param3;
        std::transform(command.begin(), command.end(), command.begin(), toupper);

        if (command == "FIND") {
            if (param1 == "" || param2 == "") {
                std::cout << "Usage: FIND <first> <last>" << std::endl;
                continue;
            }
            Person* person = tree.search(param1, param2);
            if (person != nullptr) {
                std::cout << "Found " << person->first << " " << person->last << " " << person->birthDate << " " <<
                    person->social << " " << person->zip << person->balance << " " << person->secret << " " << person->
                    state << std::endl;
            }
            else {
                std::cout << "Person not found" << std::endl;
            }
        }
        else if (command == "DELETE") {
            if (param1 == "" || param2 == "") {
                std::cout << "Usage: REMOVE <first> <last>" << std::endl;
                continue;
            }
            Person* person = tree.search(param1, param2);
            if (person != nullptr) {
                tree.remove(person);
                std::cout << "Removed " << person->first << " " << person->last << std::endl;
            }
            else {
                std::cout << "Person not found" << std::endl;
            }
        }
        else if (command == "FAMILY") {
            if (param1 == "") {
                std::cout << "Usage: FAMILY <last>" << std::endl;
                continue;
            }
            std::vector<Person*> family = tree.search_family(param1);
            if (family.size() > 0) {
                for (auto person : family) {
                    std::cout << person->first << " " << person->last << " " << person->birthDate << " " << person->
                        social << " " << person->zip << person->balance << " " << person->secret << " " << person->state
                        << std::endl;
                }
            }
            else {
                std::cout << "No people with last name " << param1 << std::endl;
            }
        }
        else if (command == "FIRST") {
            if (param1 == "") {
                std::cout << "Usage: FIRST <first>" << std::endl;
                continue;
            }
            std::vector<Person*> first_family = tree.first(param1);
            if (first_family.size() > 0) {
                for (auto person : first_family) {
                    std::cout << person->first << " " << person->last << " " << person->birthDate << " " << person->
                        social << " " << person->zip << person->balance << " " << person->secret << " " << person->state
                        << std::endl;
                }
            }
            else {
                std::cout << "No people with first name " << param1 << std::endl;
            }
        }
        else if (command == "PRINT") {
            tree.print();
        }
        else if (command == "OLDEST") {
            Person* oldest = tree.search_oldest();
            if (oldest != nullptr) {
                std::cout << "Oldest person is " << oldest->first << " " << oldest->last << " " << oldest->birthDate <<
                    " " << oldest->social << " " << oldest->zip << oldest->balance << " " << oldest->secret << " " <<
                    oldest->state << std::endl;
            }
            else {
                std::cout << "No people in tree" << std::endl;
            }
        }
        else if (command == "SAVE") {
            if (param1 == "") {
                std::cout << "Usage: SAVE <file name>" << std::endl;
                continue;
            }
            tree.save(param1);
        }
        else if (command == "RELOCATE") {
            if (param1 == "" || param2 == "" || param3 == "") {
                std::cout << "Usage: RELOCATE <first> <last> <new zip>" << std::endl;
                continue;
            }

            tree.relocate_person(param1, param2, param3);
        }
        else if (command == "CHECK") {
            tree.check_integrity();
        }
        else if (command == "EXIT") {
            break;
        }
        else {
            std::cout << "Invalid command. Try again." << std::endl;
        }
    }


    return 0;
}
