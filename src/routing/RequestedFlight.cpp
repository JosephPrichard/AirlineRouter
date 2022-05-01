//
// Created by Ltpri on 4/15/2022.
//

#include "RequestedFlight.h"
#include "../Utils.h"

RequestedFlight::RequestedFlight() = default;

RequestedFlight::RequestedFlight(
    std::string& fromLocation,
    std::string& toLocation,
    PathOptimization pathOptimization
) {
    this->fromLocation = fromLocation;
    this->toLocation = toLocation;
    this->pathOptimization = pathOptimization;
}

const std::string& RequestedFlight::getFromLocation() const {
    return fromLocation;
}

const std::string& RequestedFlight::getToLocation() const {
    return toLocation;
}

PathOptimization RequestedFlight::getPathOptimization() const {
    return pathOptimization;
}