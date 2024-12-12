#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "hashSet.h"
#include <string>
#include <cctype>

using namespace std;

struct Constant {
    int ln;
    string prefix;
    vector<string> names;
    bool isG;
};

struct MemVar {
    int ln;
    string access;
    vector<string> names;
    string type;
};

struct ClassDefinition {
    int ln;
    string name;
    vector<MemVar> members;
    vector<Constant> constants;
};

struct typeGroup {
    vector<string> names;
    string type;
};

enum RESERVED {
    CLASS,
    END,
    PUB,
    PRO,
    NAMES
};

RESERVED getReservedEnum(const string& tok) {
    if (tok == "class") return CLASS;
    if (tok == "end") return END;
    if (tok == "pub") return PUB;
    if (tok == "pro") return PRO;
    if (tok == "names") return NAMES;
    return static_cast<RESERVED>(-1);
}

const vector<string> RESERVED_WORDS = {"class", "end", "pub", "pro", "names"};

bool isIdentifier(const string& identifier) {
    if (!(isalpha(identifier[0]) || identifier[0] == '_' || identifier.empty())) {
        return false;
    }
    for (int i = 0; i < identifier.length(); ++i) {
        char c = identifier[i];
        if (!(isalnum(c) || c == '_')) {
            return false;
        }
    }
    return true;
}

bool isResWord(const string& word) {
    for (int i = 0; i < RESERVED_WORDS.size(); ++i) {
        const string& resWord = RESERVED_WORDS[i];
        if (word == resWord) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    int maxLine = 120;
    if (argc == 2) {
        maxLine = atoi(argv[1]);
    }

    int lineNum = 0;
    string line, tok;
    string currAccess = "protected";
    ClassDefinition* class_ = nullptr;
    hashSet globNames, classNames;
    vector<ClassDefinition> classes;
    vector<Constant> constants;
    istringstream iss;

    while (getline(cin, line)) {
        lineNum++;
        iss.clear();
        iss.str(line);
        if (!(iss >> tok)) {
            continue;
        }

        switch (getReservedEnum(tok)) {
        case CLASS:
            string cName;
            ClassDefinition definition;
            if (class_ != nullptr) {
                cerr << "Err:" << lineNum << ": No Nested Classes" << endl;
                exit(1);
            }
            if (!(iss >> cName)) {
                cerr << "Err:" << lineNum << ": No class name" << endl;
                exit(1);
            }
            if (!isIdentifier(cName) || isResWord(cName)) {
                cerr << "Err:" << lineNum << ": Wrong class name, " << cName << endl;
                exit(1);
            }
            if (globNames.has(cName)) {
                cerr << "Err:" << lineNum << ": Duplicated name " << cName << endl;
                exit(1);
            }
            currAccess = "protected";
            classNames = hashSet();
            globNames.add(cName);
            definition.ln = lineNum;
            definition.name = cName;
            classes.push_back(definition);
            class_ = &classes.back();
            break;
        case END:
            if (class_ == nullptr) {
                cerr << "Err:" << lineNum << ": Unmatched end" << endl;
                return 1;
            }
            class_ = nullptr;
            break;
        case PUB:
            if (class_ == nullptr) {
                cerr << "Err:" << lineNum << ": outside of class pub" << endl;
                return 1;
            }
            currAccess = "public";
            break;
        case PRO:
            if (class_ == nullptr) {
                cerr << "Err:" << lineNum << "outside of class pro" << endl;
                return 1;
            }
            currAccess = "protected";
            break;
        case NAMES:
            string pre, nm, rest, name;
            vector<string> names;
            bool cont = false;
            if (!(iss >> pre)) {
                cerr << "Err:" << lineNum << " prefix names missing" << endl;
                exit(1);
            }
            if (!isIdentifier(pre) || isResWord(pre)) {
                cerr << "Err:" << lineNum << ": Invalid prefix " << pre << endl;
                exit(1);
            }
            if (!(iss >> nm)) {
                cerr << "Err:" << lineNum << ": name needed" << endl;
                exit(1);
            }
            if (nm != "-") {
                names.push_back(nm);
            }
            getline(iss, rest);
            istringstream stream(rest);

            while (stream >> name) {
                names.push_back(name);
            }

            if (names.back() == "-" && !names.empty()) {
                names.pop_back();
                cont = true;
            }
            while (cont) {
                if (!getline(cin, line)) {
                    cerr << "Err:" << lineNum << ": Should not end input after -" << endl;
                    exit(1);
                }
                lineNum++;
                stream.clear();
                stream.str(line);
                while (stream >> name) {
                    if (name == "-") {
                        cont = true;
                        break;
                    } else {
                        names.push_back(name);
                        cont = false;
                    }
                }
            }
            for (int i = 0; i < names.size(); ++i) {
                const string& identifier = names[i];
                if (!isIdentifier(identifier) || isResWord(identifier)) {
                    cerr << "Err:" << lineNum << ": Invalid name " << identifier << endl;
                    exit(1);
                }
            }
            hashSet* hashes = class_ ? &classNames : &globNames;
            for (int i = 0; i < names.size(); ++i) {
                const string& s = names[i];
                string full = pre + "_" + s;
                if (hashes->has(full)) {
                    cerr << "Err:" << lineNum << ": Duplicated name " << full << endl;
                    exit(1);
                }
                hashes->add(full);
            }
            Constant constant;
            constant.isG = class_ == nullptr;
            constant.names = names;
            constant.prefix = pre;
            constant.ln = lineNum;
            if (class_) {
                if (currAccess != "public") {
                    currAccess = "public";
                }
                class_->constants.push_back(constant);
            } else {
                constants.push_back(constant);
            }
            break;
        default:
            string type = tok;
            string next, name_;
            vector<string> tokens, names_;
            bool complete = false;
            bool contin = false;
            if (class_ == nullptr) {
                cerr << "Err:" << lineNum << ": member outside class" << endl;
                exit(1);
            }

            tokens.push_back(type);
            while (!complete && iss >> next) {
                if (next == "-") {
                    complete = true;
                } else {
                    tokens.push_back(next);
                }
            }
            if (!complete) {
                cerr << "Err:" << lineNum << ": - is missing" << endl;
                exit(1);
            }
            type = "";
            for (int i = 0; i < tokens.size(); ++i) {
                if (i > 0) {
                    type += " ";
                }
                type += tokens[i];
            }
            while (iss >> name) {
                names.push_back(name);
            }
            if (!names.empty() && names.back() == "-") {
                contin = true;
                names.pop_back();
            }
            while (contin) {
                if (!getline(cin, line)) {
                    cerr << "Err:" << lineNum << " No input after - allowed" << endl;
                    exit(1);
                }
                lineNum++;
                istringstream lineStream(line);
                while (lineStream >> name_) {
                    if (name_ == "-") {
                        contin = true;
                        break;
                    } else {
                        names_.push_back(name_);
                        contin = false;
                    }
                }
            }
            for (int i = 0; i < names_.size(); ++i) {
                const string& n = names_[i];
                if (!isIdentifier(n) || isResWord(n)) {
                    cerr << "Err:" << lineNum << ": bad name " << n << endl;
                    exit(1);
                }
                if (classNames.has(n)) {
                    cerr << "Err:" << lineNum << ": Duplicated member name " << n << endl;
                    exit(1);
                }
                classNames.add(n);
            }
            MemVar memVar;
            memVar.names = names;
            memVar.ln = lineNum;
            memVar.access = currAccess;
            memVar.type = type;
            class_->members.push_back(memVar);
        }
    }

    for (int i = 0; i < classes.size(); ++i) {
        const ClassDefinition& classDefinition = classes[i];
        cout << "\nclass " << classDefinition.name << "\n{";
        string lastAcc = "";
        vector<typeGroup> variables;

        for (int mvI = 0; mvI < classDefinition.members.size(); ++mvI) {
            bool found = false;
            const MemVar& mv = classDefinition.members[mvI];
            for (int tgI = 0; tgI < variables.size(); ++tgI) {
                if (variables[tgI].type == mv.type) {
                    for (int nI = 0; nI < mv.names.size(); ++nI) {
                        variables[tgI].names.push_back(mv.names[nI]);
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                typeGroup group;
                group.type = mv.type;
                for (int nI = 0; nI < mv.names.size(); ++nI) {
                    group.names.push_back(mv.names[nI]);
                }
                variables.push_back(group);
            }
        }

        for (int tgI = 0; tgI < variables.size(); ++tgI) {
            const typeGroup& group = variables[tgI];
            if (group.type != "") {
                if (currAccess != lastAcc) {
                    cout << "\n  " << currAccess << ":";
                    lastAcc = currAccess;
                }
                cout << "\n    " << group.type << " ";
                for (int i = 0; i < group.names.size(); ++i) {
                    cout << group.names[i];
                    if (i < group.names.size() - 1)
                        cout << ", ";
                    else
                        cout << ";";
                }
            }
        }

        if (!classDefinition.constants.empty()) {
            if ("public" != lastAcc) {
                cout << "\n  public:";
                lastAcc = "public";
            }
            for (int i = 0; i < classDefinition.constants.size(); ++i) {
                const Constant& nc = classDefinition.constants[i];
                cout << "\n    const int ";
                int value = 1;
                int lineLength = 4;
                for (int nameI = 0; nameI < nc.names.size(); ++nameI) {
                    string constantName = nc.prefix + "_" + nc.names[nameI];
                    string constantDefinition = constantName + " = " + to_string(value++);
                    if (nameI < nc.names.size() - 1)
                        constantDefinition += ",";
                    else
                        constantDefinition += ";";
                    if (lineLength + static_cast<int>(constantDefinition.length()) > maxLine) {
                        cout << "\n      " << constantDefinition;
                        lineLength = 6 + constantDefinition.length();
                    } else {
                        if (nameI > 0)
                            cout << " ";
                        cout << constantDefinition;
                        lineLength += constantDefinition.length() + 1;
                    }
                }
            }
        }

        for (int i = 0; i < classDefinition.members.size(); ++i) {
            const MemVar& mv = classDefinition.members[i];
            for (int nameI = 0; nameI < mv.names.size(); ++nameI) {
                const string& name = mv.names[nameI];
                string setType = mv.type;
                string getType = mv.type;
                string paramType = mv.type;
                bool pointer = false;
                if (getType.find("*") != string::npos)
                    pointer = true;
                if (pointer) {
                    paramType = getType + " &";
                } else {
                    paramType = "const " + getType + " &";
                }
                cout << "\n\n    " << getType << " get_" << name << "() const\n    { return " << name << "; }";
                cout << "\n\n    void set_" << name << "(" << paramType << " v)\n    { " << name << " = v; }";
            }
        }
        cout << " };";
    }

    for (int i = 0; i < constants.size(); ++i) {
        const Constant& constant = constants[i];
        int val = 1;
        int length = 0;
        cout << "\n\nconst int ";
        for (int nI = 0; nI < constant.names.size(); ++nI) {
            string name = constant.prefix + "_" + constant.names[nI];
            string def = name + " = " + to_string(val++);
            if (nI < constant.names.size() - 1)
                def += ",";
            else
                def += ";";
            if (length + static_cast<int>(def.length()) > maxLine) {
                cout << "\n  " << def;
                length = 2 + def.length();
            } else {
                if (nI > 0)
                    cout << " ";
                cout << def;
                length += def.length() + 1;
            }
        }
    }
    cout << "\n";
    return 0;
}
