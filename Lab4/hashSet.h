#pragma once

#include <vector>
#include <string>

using namespace std;

# define tableSize 1000

class hashSet {
private:
    vector<string> table[tableSize];

    int hash(const string& input) const {
        int h = 0;
        for (int i = 0; i < input.length(); ++i) {
            char c = input[i];
            h = h + c;
        }
        return h % tableSize;
    }

public:
    bool has(const string& name) const {
        unsigned int index = hash(name);
        for (size_t i = 0; i < table[index].size(); ++i) {
            const string& str = table[index][i];
            if (str == name)
                return true;
        }
        return false;
    }

    void add(const string& name) {
        int index = hash(name);
        for (int i = 0; i < table[index].size(); ++i) {
            const string& str = table[index][i];
            if (str == name)
                return;
        }
        table[index].push_back(name);
    }
};
