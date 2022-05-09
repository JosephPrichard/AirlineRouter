//
// Created by Ltpri on 5/8/2022.
//

#include "FlightPath.h"

float totalFlightCost(std::vector<Flight>& path) {
    float cost = 0;
    for (auto &flight : path) {
        cost += flight.getCost();
    }
    return cost;
}


int totalFlightTime(std::vector<Flight>& path) {
    int time = 0;
    for (auto &flight : path) {
        time += flight.getTime();
    }
    return time;
}

FlightPath::FlightPath(std::vector<Flight*>::iterator begin, std::vector<Flight*>::iterator end) {
    for (auto it = begin ; it != end; it++) {
        flightPath.push_back(**it);
    }
    totalTime = totalFlightTime(flightPath);
    totalCost = totalFlightCost(flightPath);
}

const std::vector<Flight>& FlightPath::getFlightPath() const {
    return flightPath;
}

int FlightPath::getTotalTime() const {
    return totalTime;
}

float FlightPath::getTotalCost() const {
    return totalCost;
}

unsigned int FlightPath::size() const {
    return flightPath.size();
}

const Flight& FlightPath::operator[](int index) {
    return flightPath[index];
}
