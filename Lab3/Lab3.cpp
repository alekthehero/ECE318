#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "hash_table.h"
#include "definitions.h"

std::vector<Intersection*> intersections;
std::vector<Connection*> connections;

Intersection* findIntersectionByID(int id) {
    for (Intersection* intersection : intersections) {
        if (intersection->id == id) {
            return intersection;
        }
    }
    return nullptr;
}

void exploreIntersection(Intersection* thisIntersection) {
    bool exploring = true;
    while (exploring) {
        thisIntersection->print();
        int numOptions = thisIntersection->connections.size();
        for (int i = 0; i < numOptions; ++i) {
            Connection* connection = thisIntersection->connections[i];
            Intersection* nextIntersection = connection->from == thisIntersection ? connection->to : connection->from;

            std::cout << i << ": " << connection->name << " to Intersection "
                << nextIntersection->id << ", length: " << connection->length << std::endl;
        }
        std::cout << "Enter Number, or 'Q' to quit: ";
        std::string line;
        getline(std::cin, line);
        if (line == "Q" || line == "\n") {
            exploring = false;
        } else {
            int selectedRoad = stoi(line);
            if (selectedRoad > 0 && selectedRoad < numOptions) {
                Connection* selectedConnection = thisIntersection->connections[selectedRoad];
                thisIntersection = selectedConnection->from == thisIntersection ? selectedConnection->to : selectedConnection->from;
            } else std::cout << "Invalid Option Selected" << std::endl;
        }
    }
}

void shortestPath(Intersection* start, Intersection* end) {
    for (Intersection* intersection : intersections) {
        intersection->distance = -1;
        intersection->prev = nullptr;
    }
    start->distance = 0;
    start->prev = nullptr;
    int front = 0;
    // Using a vector as a queue, tried my own and I couldn't get it to work kept overflowing the heap...
    std::vector<Intersection*> queue;
    queue.push_back(start);

    while (front < queue.size()) {
        Intersection* i1 = queue[front];
        front++;
        for (Connection* r : i1->connections) {
            Intersection* i2 = r->from->id == i1->id ? r->to : r->from;
            double nextTotalDist = i1->distance + r->length;
            if (i2->distance == -1 || nextTotalDist < i2->distance) {
                i2->distance = nextTotalDist;
                i2->prev = i1;
                queue.push_back(i2);
            }
        }
    }
    if (end->prev != nullptr) {
        std::vector<Intersection*> path;
        Intersection* current = end;
        while (current != nullptr) {
            path.push_back(current);
            current = current->prev;
        }
    } else {
        std::cout << "no path" << std::endl;
    }
    std::cout << "From " << start->id << " to " << end->id << ": " << end->distance << std::endl;
}

int main(int argc, char* argv[]) {
    // Read named-places file
    std::fstream named_file("named-places.txt", std::ios::in);
    if (!named_file.is_open()) {
        std::cerr << "Error: Unable to open file: named-places.txt " << std::endl;
        return 1;
    }

    hash_table table;

    std::string line, code, state, city, population, area, latitude, longitude, intersection, distance;
    while (std::getline(named_file, line)) {
        if (line.empty()) {
            continue;
        }

        code = table.remove_white_space(line.substr(0, 8));
        state = table.remove_white_space(line.substr(8, 2));
        city = table.remove_white_space(line.substr(10, 30));
        population = table.remove_white_space(line.substr(57, 10));
        area = table.remove_white_space(line.substr(68, 13));
        latitude = table.remove_white_space(line.substr(81, 9));
        longitude = table.remove_white_space(line.substr(91, 10));
        intersection = table.remove_white_space(line.substr(101, 5));
        distance = table.remove_white_space(line.substr(108, 7));

        Place p(std::stoi(code), state, city, std::stoi(population), std::stod(area), std::stod(latitude),
                std::stod(longitude), std::stoi(intersection), std::stod(distance));
        table.insert(p);
        // TODO - Remove for submission
        if (table.num_elements % 200 == 0) {
            std::cout << "Inserted: " << p.city << ", " << p.state << std::endl;
        }
    }
    named_file.close();
    // Read intersections file
    std::fstream intersection_file("intersections.txt", std::ios::in);
    if (!intersection_file.is_open()) {
        std::cerr << "Error: Unable to open file: intersections.txt " << std::endl;
        return 1;
    }
    int intersectionCount = 1;
    while (std::getline(intersection_file, line)) {
        if (line.empty()) {
            continue;
        }
        latitude = table.remove_white_space(line.substr(0, 9));
        longitude = table.remove_white_space(line.substr(10, 10));
        distance = table.remove_white_space(line.substr(20, 7));
        state = table.remove_white_space(line.substr(28, 2));
        city = table.remove_white_space(line.substr(30, 30));
        Intersection* inter = new Intersection(std::stod(latitude), std::stod(longitude), std::stod(distance), state,
                                               city, intersectionCount++);
        intersections.push_back(inter);
    }
    intersection_file.close();
    // Read states file
    std::fstream states_file("states.txt", std::ios::in);
    if (!states_file.is_open()) {
        std::cerr << "Error: Unable to open file: states.txt " << std::endl;
        return 1;
    }
    std::vector<std::pair<std::string, std::string>> states_pairs;
    while (std::getline(states_file, line)) {
        if (line.empty()) {
            continue;
        }
        state = table.remove_white_space(line.substr(0, 2));
        city = table.remove_white_space(line.substr(3, 30));
        states_pairs.push_back(std::make_pair(state, city));
    }
    states_file.close();
    //Read connections file
    std::fstream connectionsFile("connections.txt", std::ios::in);
    if (!connectionsFile.is_open()) {
        std::cerr << "Error: Unable to open file: connections.txt " << std::endl;
        return 1;
    }
    while (std::getline(connectionsFile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);

        std::string road, type;
        int id1, id2;
        double length;
        iss >> road >> type >> id1 >> id2 >> length;

        Intersection* iner1 = findIntersectionByID(id1);
        Intersection* inter2 = findIntersectionByID(id2);

        if (iner1 && inter2) {
            Connection* connection = new Connection(road, length, iner1, inter2);
            iner1->connections.push_back(connection);
            inter2->connections.push_back(connection);
            connections.push_back(connection);
        } else
            std::cout << "Intersections not found for: " << road << std::endl;
    }

    while (true) {
        std::cout <<
            "Enter command: N <City>, S <City> <State>, Explore <City> <State>, Shortest, Q\n";
        std::string input, command, placeName, stateAbbreviation;
        std::getline(std::cin, input);
        std::istringstream iss(input);
        iss >> command;

        if (command == "N") {
            std::getline(iss, placeName);
            placeName = placeName.substr(1);
            if (placeName.empty()) {
                std::cout << "Usage: N <place name>" << std::endl;
                continue;
            }
            std::list<Place> result = table.find_by_city(placeName);
            if (result.empty()) {
                std::cout << "No places found for " << placeName << std::endl;
                continue;
            }

            for (const std::pair<std::string, std::string>& pair : states_pairs) {
                for (const Place& place : result) {
                    if (place.state == pair.first) {
                        std::cout << pair.first << " " << pair.second << std::endl;
                    }
                }
            }
        } else if (command == "S") {
            std::getline(iss, placeName);
            int pos = placeName.find_last_of(' ');
            stateAbbreviation = placeName.substr(pos + 1);
            placeName = placeName.substr(1, pos - 1);
            std::cout << stateAbbreviation << ' ' << placeName << std::endl;
            if (placeName.empty() || stateAbbreviation.empty()) {
                std::cout << "Usage: S <place name> <state abbreviation>" << std::endl;
                continue;
            }
            Place result = table.find_by_city_state(placeName, stateAbbreviation);
            if (result.city.empty()) {
                std::cout << "No place found for " << placeName << " in " << stateAbbreviation << std::endl;
                continue;
            }
            Intersection* closest = findIntersectionByID(result.intersection_code);
            if (closest)
                closest->print();
            else
                std::cout << "No intersection found" << std::endl;
        } else if (command == "Explore") {
            std::getline(iss, line);
            if (line.empty()) {
                std::cout << "Usage: Explore <place name> <state abbreviation>" << std::endl;
                continue;
            }
            int pos = line.find_last_of(' ');
            stateAbbreviation = line.substr(pos + 1);
            placeName = line.substr(1, pos - 1);

            Place result = table.find_by_city_state(placeName, stateAbbreviation);

            if (!result.city.empty()) {
                Intersection* currentIntersection = findIntersectionByID(result.intersection_code);
                if (currentIntersection == nullptr) {
                    std::cout << "Intersection not found for the place." << std::endl;
                    continue;
                }
                exploreIntersection(currentIntersection);
            } else {
                std::cout << "Place not found." << std::endl;
            }
        } else if (command == "Shortest") {
            std::string startCity, startState, endCity, endState;

            std::cout << "Enter Start Location, <City> <State>: ";
            getline(std::cin, line);
            std::istringstream startText(line);
            startText >> startCity >> startState;
            std::cout << "Enter End Location, <City> <State>: ";
            getline(std::cin, line);
            std::istringstream endText(line);
            endText >> endCity >> endState;

            Place startPlace = table.find_by_city_state(startCity, startState);
            Place destPlace = table.find_by_city_state(endCity, endState);

            if (startPlace.city.empty() || destPlace.city.empty()) {
                std::cout << "location not found." << std::endl;
                continue;
            }

            Intersection* start = findIntersectionByID(startPlace.intersection_code);
            Intersection* end = findIntersectionByID(destPlace.intersection_code);

            if (start == nullptr || end == nullptr) {
                std::cout << "intersection not found." << std::endl;
                continue;
            }
            shortestPath(start, end);
        } else if (command == "Q") {
            break;
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }

    return 0;
}
