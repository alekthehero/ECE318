#pragma once

struct intersection {
    double latitude, longitude, distance;
    std::string state, city;

    intersection() : latitude(0.0), longitude(0.0), distance(0.0) {}

    intersection(double lat, double lon, double dist, const std::string& s, const std::string& ci)
        : latitude(lat), longitude(lon), distance(dist), state(s), city(ci) {}
};
