//
// Created by Ltpri on 4/15/2022.
//

#ifndef AIRLINESHORTESTPATHS_AIRLINEROUTER_H
#define AIRLINESHORTESTPATHS_AIRLINEROUTER_H

#include <vector>
#include "RequestedFlight.h"
#include "../network/Flight.h"
#include "../network/AirlineNetwork.h"
#include "FrontierNode.h"

// data structure responsible for maintaining airline network graph, requested flights, and optimal flight paths
// provides the ability to calculate optimal flight paths for a requested flight using Dijkstra's algorithm
class AirlineRouter {
private:
    // unique ownership of airline network, when router is deallocated so should the network
    std::unique_ptr<AirlineNetwork> airlineNetwork;
    // the vector of requested flights to calculate paths for
    std::vector<RequestedFlight> requestedFlights;
    // 3d vector: a vector of top paths (which is a vector of paths (each path is a vector of flights)) for each requested flight
    // top paths vector contains the path already in sorted order from least to most costly (in order of discovery)
    std::vector<std::vector<std::vector<Flight>>> optimalFlightPaths;

public:
    explicit AirlineRouter(std::unique_ptr<AirlineNetwork>& airlineNetwork);

    const std::unique_ptr<AirlineNetwork>& getAirlineNetwork() const;

    const std::vector<RequestedFlight>& getRequestedFlights() const;

    void addRequestedFlight(RequestedFlight& requestedFlight);

    const std::vector<std::vector<std::vector<Flight>>>& getOptimalFlightPaths() const;

    // calculate optimal flight paths using Dijkstra's algorithm for a specific requested flight
    std::vector<std::vector<Flight>> findFlightPath(RequestedFlight& requestedFlight);

    // uses iterative backtracking to reconstruct a path after Dijkstra's algorithm is finished
    static std::vector<Flight> backtrackPath(FrontierNode* node);

    // calculate optimal flight paths using Dijkstra's algorithm for each requested flight and push into optimal paths vector
    void addFlightPaths();

    // read an input stream containing requested flights into the airline router
    friend std::istream& operator>>(std::istream& is, AirlineRouter& airlineRouter);

    // write an optimal flight paths for each requested flight into airline router
    friend std::ostream& operator<<(std::ostream& os, AirlineRouter& airlineRouter);
};

#endif //AIRLINESHORTESTPATHS_AIRLINEROUTER_H
