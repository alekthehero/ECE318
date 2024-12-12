#pragma once
#include <string>
#include <vector>

class Intersection;

struct State {
    std::string name, code;
    State(const std::string& n, const std::string& c) : name(n), code(c) {}
};

struct Connection {
    std::string name;
    double length;
    Intersection* from;
    Intersection* to;

    Connection(const std::string& n, double l, Intersection* f, Intersection* t)
        : name(n), length(l), from(f), to(t) {}
};

class Intersection {
public:
    int id;
    double latitude, longitude, distance;
    std::vector<Connection*> connections;
    Intersection* prev;

    Intersection() : id(0), latitude(0.0), longitude(0.0), distance(-1), prev(nullptr) {}

    Intersection(int id, double lat, double lon)
        : id(id), latitude(lat), longitude(lon), distance(-1), prev(nullptr) {}


    Intersection(double lat, double lon, double dist, const std::string& s, const std::string& ci, int id)
        : id(id), latitude(lat), longitude(lon), distance(dist), prev(nullptr) {}

    void print() const {
        std::cout << "Intersection ID: " << id;
        std::cout << "  Latitude: " << latitude;
        std::cout << "  Longitude: " << longitude << std::endl;
        std::cout << "  Roads: ";
        for (Connection* connection : connections) {
            std::cout << connection->name << ", ";
        }
        std::cout << std::endl;
    }
};

class Place {
public:
    int place_id, intersection_code, pop;
    std::string state, city;
    double area, latitude, longitude, distence;

    Place() : place_id(0), intersection_code(0), pop(0), area(0), latitude(0), longitude(0), distence(0) {}

    Place(int code, std::string state, std::string city, int pop, double area, double lat, double lon, int interCode,
          double distToInter)
        : place_id(code),
          intersection_code(interCode),
          pop(pop),
          state(state),
          city(city),
          area(area),
          latitude(lat), longitude(lon),
          distence(distToInter) {}


    void print() const {
        std::cout << "Place: " << place_id << std::endl;
        std::cout << "State: " << state << std::endl;
        std::cout << "Name: " << city << std::endl;
        std::cout << "Population: " << pop << std::endl;
        std::cout << "Area: " << area << std::endl;
        std::cout << "Latitude: " << latitude << std::endl;
        std::cout << "Longitude: " << longitude << std::endl;
        std::cout << "Intersection Code: " << intersection_code << std::endl;
        std::cout << "Distance to Intersection: " << distence << std::endl;
    }
};
