#pragma once
#include <string>

struct place {
    int code;
    std::string state;
    std::string city;
    int population;
    double area;
    double latitude;
    double longitude;
    int intersection;
    double distance;

    place() : code(0), population(0), area(0.0), latitude(0.0), longitude(0.0), intersection(0), distance(0.0) {}

    place(int c, const std::string& s, const std::string& ci, int p, double a, double lat, double lon, int inter,
          double dist)
        : code(c), state(s), city(ci), population(p), area(a), latitude(lat), longitude(lon), intersection(inter),
          distance(dist) {}

    bool operator<(const place& other) const {
        return city < other.city;
    }

    bool operator>(const place& other) const {
        return city > other.city;
    }

    bool operator==(const place& other) const {
        return city == other.city;
    }

    bool operator!=(const place& other) const {
        return city != other.city;
    }
};

struct place_hash {
    std::size_t operator()(const place& p) const {
        return std::hash<std::string>()(p.city);
    }
};

struct place_equal {
    bool operator()(const place& lhs, const place& rhs) const {
        return lhs.city == rhs.city;
    }
};
