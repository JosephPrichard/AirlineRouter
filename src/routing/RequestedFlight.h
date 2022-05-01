//
// Created by Ltpri on 4/15/2022.
//

#ifndef AIRLINESHORTESTPATHS_REQUESTEDFLIGHT_H
#define AIRLINESHORTESTPATHS_REQUESTEDFLIGHT_H

#include <istream>
#include <string>
#include <vector>

enum PathOptimization { FLIGHT_COST, FLIGHT_TIME };

class RequestedFlight {
private:
    std::string fromLocation;
    std::string toLocation;
    PathOptimization pathOptimization = FLIGHT_COST;

public:
    RequestedFlight();

    RequestedFlight(std::string& fromLocation, std::string& toLocation, PathOptimization pathOptimization);

    const std::string& getFromLocation() const;

    const std::string& getToLocation() const;

    PathOptimization getPathOptimization() const;
};

#endif //AIRLINESHORTESTPATHS_REQUESTEDFLIGHT_H
