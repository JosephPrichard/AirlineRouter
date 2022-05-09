//
// Created by Ltpri on 5/8/2022.
//

#ifndef AIRLINEROUTER_FLIGHTPATH_H
#define AIRLINEROUTER_FLIGHTPATH_H


#include <vector>
#include "../network/Flight.h"

class FlightPath {
private:
    std::vector<Flight> flightPath;
    int totalTime;
    float totalCost;

public:
    FlightPath(std::vector<Flight*>::iterator begin, std::vector<Flight*>::iterator end);

    const std::vector<Flight>& getFlightPath() const;

    int getTotalTime() const;

    float getTotalCost() const;

    unsigned int size() const;

    const Flight& operator[](int index);
};


#endif //AIRLINEROUTER_FLIGHTPATH_H
