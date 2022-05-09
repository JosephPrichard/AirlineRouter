//
// Created by Ltpri on 4/14/2022.
//

#include "Flight.h"

Flight::Flight() = default;

Flight::Flight(const std::string& location, float cost, int time) {
    this->location = location;
    this->cost = cost;
    this->time = time;
}

const std::string& Flight::getLocation() const {
    return location;
}

float Flight::getCost() const {
    return cost;
}

int Flight::getTime() const {
    return time;
}
