#pragma once

struct Intersection {
    double latitude, longitude, distance;
    std::string state, city;

    Intersection() : latitude(0.0), longitude(0.0), distance(0.0) {}

    Intersection(double lat, double lon, double dist, const std::string& s, const std::string& ci)
        : latitude(lat), longitude(lon), distance(dist), state(s), city(ci) {}
};
