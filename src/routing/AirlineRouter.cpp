//
// Created by Ltpri on 4/15/2022.
//

#include <unordered_map>
#include <queue>
#include <cfloat>
#include <algorithm>
#include "AirlineRouter.h"
#include "FrontierNode.h"
#include "../Utils.h"

#define MAX_OPT_PATHS 3

AirlineRouter::AirlineRouter(std::unique_ptr<AirlineNetwork>& airlineNetwork) {
    this->airlineNetwork = std::move(airlineNetwork);
}

const std::unique_ptr<AirlineNetwork>& AirlineRouter::getAirlineNetwork() const {
    return airlineNetwork;
}

const std::vector<RequestedFlight>& AirlineRouter::getRequestedFlights() const {
    return requestedFlights;
}

void AirlineRouter::addRequestedFlight(RequestedFlight& requestedFlight) {
    requestedFlights.push_back(requestedFlight);
}

const std::vector<std::vector<std::vector<Flight>>>& AirlineRouter::getOptimalFlightPaths() const {
    return optimalFlightPaths;
}

// implementation of iterative backtracking to check if a location is already contained in a path
bool isVisited(FrontierNode* node, const std::string& location) {
    // climb up tree of paths until we find src node or a match
    while (node != nullptr) {
        if (node->getFlight().getLocation() == location) {
            return true;
        }
        node = node->getParent();
    }
    return false;
}

std::vector<std::vector<Flight>> AirlineRouter::findFlightPath(RequestedFlight& requestedFlight) {
    // create a lambda function to return the right path cost depending on whether we optimize for flight time or flight cost
    auto getPathCost = requestedFlight.getPathOptimization() == FLIGHT_TIME ?
                       [](Flight& flight) { return (float) flight.getTime(); } :
                       [](Flight& flight) { return flight.getCost(); };

    // create a vector to store the shortest paths from src to dest
    std::vector<std::vector<Flight>> shortestPaths{};

    // src flight representing the src location
    Flight srcFlight(requestedFlight.getFromLocation(), 0, 0.0f);
    // frontier decides which nodes we expand first, the ones with the lowest cost
    auto comp = [](auto left, auto right ) {return left->getPathCost() > right->getPathCost(); };
    std::priority_queue<std::shared_ptr<FrontierNode>, std::vector<std::shared_ptr<FrontierNode>>, decltype(comp)> frontier(comp);

    // src node representing the src location
    auto srcNode = std::make_shared<FrontierNode>(&srcFlight, 0.0f);
    frontier.push(srcNode);

    // counts the number of times we've discovered the dest node
    int timesDiscovered = 0;

    std::cout << "\n";
    // iterate until every vertex has been reached
    while(!frontier.empty()) {
        // expand the vertex with the lowest cost so far
        auto current = frontier.top();
        frontier.pop();

        std::cout << current->getFlight().getLocation() << "\n";

        // check if the current vertex is the dest vertex
        if (current->getFlight().getLocation() == requestedFlight.getToLocation() && timesDiscovered < MAX_OPT_PATHS) {
            auto path = backtrackPath(current.get());
            shortestPaths.push_back(path);
            timesDiscovered++;
            continue;
        }

        // iterate through the neighbors of the current vertex and add them to the frontier
        auto neighbors = airlineNetwork->neighborFlights(current->getFlight().getLocation());

        for (auto &neighbor : *neighbors) {
            std::cout << " " << neighbor.getLocation() << " " << neighbor.getCost() << "\n";
            // cost to reach this neighbor from src vertex
            auto cost = current->getPathCost() + getPathCost(neighbor);
            // add vertex to frontier, if it hasn't been visited in the current path
            if (!isVisited(current.get(), neighbor.getLocation())) {
                std::cout << " Added to Frontier" << "\n";
                // add node to frontier
                auto node = std::make_shared<FrontierNode>(current, &neighbor, cost);
                frontier.push(node);
            }
        }
    }

    return shortestPaths;
}

// implementation of iterative backtracking to reconstruct a path
std::vector<Flight> AirlineRouter::backtrackPath(FrontierNode* node) {
    std::vector<Flight> path{};
    // climb up tree of paths until we find src node
    while (node != nullptr) {
        std::cout << " " << node->getFlight().getLocation() << " ->";
        path.push_back(node->getFlight());
        node = node->getParent();
    }
    // reverse the path into right order since it was found by backtracking
    std::reverse(path.begin(), path.end());
    std::cout << "\n";
    return path;
}

void AirlineRouter::addFlightPaths() {
    for (auto &requestedFlight : requestedFlights) {
        optimalFlightPaths.push_back(findFlightPath(requestedFlight));
    }
}

std::ostream& operator<<(std::ostream &os, AirlineRouter &airlineRouter) {
    // for each requested flight, output the requested flight info, and the best flight paths
    for (int i = 0; i < airlineRouter.requestedFlights.size(); i++) {
        auto requestedFlight = airlineRouter.requestedFlights[i];
        auto flightPaths = airlineRouter.optimalFlightPaths[i];

        // output requested flight info
        os << "Flight " << (i + 1) << ": ";
        os << requestedFlight.getFromLocation() << ", ";
        os << requestedFlight.getToLocation() << " ";
        os << "(" << (requestedFlight.getPathOptimization() == PathOptimization::FLIGHT_COST ? "Cost" : "Time") << ")" << "\n";

        // iterate through optimized paths (already in order) and output
        for (int j = 0; j < flightPaths.size(); j++) {
            // iterate through each flight in the path and output
            float totalCost = 0;
            int totalTime = 0;
            os << "Path " << (j + 1) << ": ";
            auto flightPath = flightPaths[j];
            for (int k = 0; k < flightPath.size(); k++) {
                os << flightPath[k].getLocation() << (k < flightPath.size() - 1 ? " -> " : ". ");
                totalCost += flightPath[k].getCost();
                totalTime += flightPath[k].getTime();
            }

            // output the total times and costs of the best path
            os << "Time: " << totalTime << " ";
            os << "Cost: " << totalCost << "\n";
        }

        os << "\n";
    }
    return os;
}

std::istream& operator>>(std::istream &is, AirlineRouter &airlineRouter) {
    std::string line;

    // expand the vector to the number of requested flights to reduce the amount of internal resizes needed
    if (std::getline(is, line)) {
        int size = std::stoi(line);
        airlineRouter.requestedFlights.reserve(size);
    } else {
        return is;
    }

    // get each line from input stream and parse into vector of requested flights
    while (std::getline(is, line)) {
        std::vector<std::string> splitStr = splitString(line, '|');

        if (splitStr.size() < 3) {
            throw std::out_of_range("requested flight must contain 3 tokens");
        }

        auto pathOptimization = splitStr[2][0] == 'T' ? PathOptimization::FLIGHT_TIME : PathOptimization::FLIGHT_COST;
        RequestedFlight requestedFlight(splitStr[0], splitStr[1], pathOptimization);
        airlineRouter.requestedFlights.push_back(requestedFlight);
    }

    return is;
}
