#pragma once
#include <string>

struct Person {
    int social, birthDate;
    double balance;
    std::string first, last, zip, secret, state;

    Person(): social(0), birthDate(0), balance(0) {}

    Person(
        int social,
        int birth_date,
        std::string first,
        std::string last,
        std::string zip,
        double balance,
        std::string secret,
        std::string state)
        : social(social), birthDate(birth_date), balance(balance), first(first), last(last), zip(zip), secret(secret),
          state(state) {}

    Person(const Person& other) : social(other.social), birthDate(other.birthDate), balance(other.balance),
                                  first(other.first),
                                  last(other.last), zip(other.zip), secret(other.secret), state(other.state) {}

    Person& operator=(const Person& other) {
        if (this != &other) {
            social = other.social;
            birthDate = other.birthDate;
            first = other.first;
            last = other.last;
            zip = other.zip;
            balance = other.balance;
            secret = other.secret;
            state = other.state;
        }
        return *this;
    }

    bool operator==(const Person& other) const {
        return last == other.last && first == other.first;
    }

    bool operator!=(const Person& other) const {
        return !(*this == other);
    }

    bool operator<(const Person& other) const {
        if (last < other.last)
            return true;
        if (last == other.last)
            return first < other.first;
        return false;
    }

    bool operator>(const Person& other) const {
        if (last > other.last)
            return true;
        if (last == other.last)
            return first > other.first;
        return false;
    }

    bool operator<=(const Person& other) const {
        return !(*this > other);
    }

    bool operator>=(const Person& other) const {
        return !(*this < other);
    }
};
