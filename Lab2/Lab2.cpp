#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "hash_table.h"
#include "intersection.h"
#include "place.h"

int main(int argc, char* argv[]) {
    std::fstream named_file("named-places.txt", std::ios::in);
    if (!named_file.is_open()) {
        std::cerr << "Error: Unable to open file: named-places.txt " << std::endl;
        return 1;
    }

    hash_table table;

    std::string line;
    while (std::getline(named_file, line)) {
        if (line.empty()) {
            continue;
        }
        std::string code, state, city, population, area, latitude, longitude, intersection, distance;
        code = table.remove_white_space(line.substr(0, 8));
        state = table.remove_white_space(line.substr(8, 2));
        city = table.remove_white_space(line.substr(10, 30));
        population = table.remove_white_space(line.substr(57, 10));
        area = table.remove_white_space(line.substr(68, 13));
        latitude = table.remove_white_space(line.substr(81, 9));
        longitude = table.remove_white_space(line.substr(91, 10));
        intersection = table.remove_white_space(line.substr(101, 5));
        distance = table.remove_white_space(line.substr(108, 7));

        place p(std::stoi(code), state, city, std::stoi(population), std::stod(area), std::stod(latitude),
                std::stod(longitude), std::stoi(intersection), std::stod(distance));
        table.insert(p);
    }
    named_file.close();
    std::fstream intersection_file("intersections.txt", std::ios::in);
    if (!intersection_file.is_open()) {
        std::cerr << "Error: Unable to open file: intersections.txt " << std::endl;
        return 1;
    }
    std::vector<intersection*> intersections;
    while (std::getline(intersection_file, line)) {
        if (line.empty()) {
            continue;
        }
        std::string latatude, longitude, distance, state, city;
        latatude = table.remove_white_space(line.substr(0, 9));
        longitude = table.remove_white_space(line.substr(10, 10));
        distance = table.remove_white_space(line.substr(20, 7));
        state = table.remove_white_space(line.substr(28, 2));
        city = table.remove_white_space(line.substr(30, 30));
        intersection* inter = new intersection(std::stod(latatude), std::stod(longitude), std::stod(distance), state,
                                               city);
        intersections.push_back(inter);
    }
    intersection_file.close();
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
        std::string state, name;
        state = table.remove_white_space(line.substr(0, 2));
        name = table.remove_white_space(line.substr(3, 30));
        states_pairs.push_back(std::make_pair(state, name));
    }
    states_file.close();

    while (true) {
        std::cout <<
            "Enter command: N <place name>, S <place name> <state abbreviation>, Q\n";
        std::string input, command, place_name, state_abbreviation;
        std::getline(std::cin, input);
        std::istringstream iss(input);
        iss >> command;

        if (command == "N") {
            std::getline(iss, place_name);
            place_name = place_name.substr(1);
            if (place_name.empty()) {
                std::cout << "Usage: N <place name>" << std::endl;
                continue;
            }
            std::list<place> result = table.find_by_city(place_name);
            if (result.empty()) {
                std::cout << "No places found for " << place_name << std::endl;
                continue;
            }

            for (const std::pair<std::string, std::string>& state : states_pairs) {
                for (const place& place : result) {
                    if (place.state == state.first) {
                        std::cout << state.first << " " << state.second << std::endl;
                    }
                }
            }
        }
        else if (command == "S") {
            std::getline(iss, place_name);
            size_t pos = place_name.find_last_of(' ');
            state_abbreviation = place_name.substr(pos + 1);
            place_name = place_name.substr(1, pos - 1);
            std::cout << state_abbreviation << ' ' << place_name << std::endl;
            if (place_name.empty() || state_abbreviation.empty()) {
                std::cout << "Usage: S <place name> <state abbreviation>" << std::endl;
                continue;
            }
            place result = table.find_by_city_state(place_name, state_abbreviation);
            if (result.city.empty()) {
                std::cout << "No place found for " << place_name << " in " << state_abbreviation << std::endl;
                continue;
            }
            intersection* closest = intersections[result.intersection];

            std::cout << "Place: " << result.city << ", " << result.state << ", Pop: " << result.population
                << ", Area: " << result.area << ", Lat: " << result.latitude << ", Long: " << result.longitude
                << ", Inter: " << result.intersection << ", Dist: " << result.distance << std::endl;
            std::cout << "Closest intersection: " << closest->latitude << ", " << closest->longitude << ", "
                << closest->distance << ", " << closest->state << ", " << closest->city << std::endl;
        }
        else if (command == "Q") {
            break;
        }
        else {
            std::cout << "Invalid command" << std::endl;
        }
    }

    return 0;
}
