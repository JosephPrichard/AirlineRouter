//
// Created by Ltpri on 4/15/2022.
//

#include <algorithm>
#include <unordered_map>
#include "AirlineRouter.h"
#include "../stack/Stack.h"
#include "../utils/StringUtils.h"

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

const std::vector<std::vector<FlightPath>>& AirlineRouter::getOptimalFlightPaths() const {
    return optimalFlightPaths;
}

bool compPathsByTime(FlightPath& path, FlightPath& path1) {
    return path.getTotalTime() < path1.getTotalTime();
}

bool compPathsByCost(FlightPath& path, FlightPath& path1) {
    return path.getTotalCost() < path1.getTotalCost();
}

std::vector<FlightPath> AirlineRouter::findFlightPaths(RequestedFlight& requestedFlight) {
    // create a lambda function to return the right total path cost depending on whether we optimize for flight time or flight cost
    auto compPaths= requestedFlight.getPathOptimization() == FLIGHT_TIME ?
                    compPathsByTime : compPathsByCost;

    // create a vector to store the shortest paths from src to dest
    std::vector<FlightPath> paths{};

    // frontierDepth for dfs with backtracking
    Stack<Flight*> frontier{};
    Stack<int> frontierDepth{};
    // store visited vertices
    std::unordered_map<std::string, bool> visited{};
    // keep track of current path expanding
    std::vector<Flight*> currentPath(airlineNetwork->numLocations());
    int currentPathIndex = 0;

    Flight srcFlight(requestedFlight.getFromLocation(), 0, 0.0f);
    frontier.push(&srcFlight);
    frontierDepth.push(0);

    while (!frontier.empty()) {
        // expand the vertex at top of frontier
        auto current = frontier.pop();
        auto currentDepth = frontierDepth.pop();

        // if the current depth isn't the same as the current path index backtrack to keep state updated
        while (currentPathIndex > 0 && currentPathIndex != currentDepth) {
            currentPathIndex--;
            visited[currentPath[currentPathIndex]->getLocation()] = false;
        }

        visited[current->getLocation()] = true;
        currentPath[currentPathIndex] = current;
        currentPathIndex++;

        // check if the current vertex is the dest vertex
        if (current->getLocation() == requestedFlight.getToLocation()) {
            // add a possible path to the list of all paths
            FlightPath path(currentPath.begin(), currentPath.begin() + currentPathIndex);
            paths.push_back(path);
            continue;
        }

        // iterate through the neighbors of the current vertex and add them to the frontierDepth
        auto neighbors = airlineNetwork->neighborFlights(current->getLocation());
        bool addedNeighbors = false;
        for (auto& neighbor: *neighbors) {
            if (!visited[neighbor.getLocation()]) {
                frontier.push(&neighbor);
                frontierDepth.push(currentDepth + 1);
                addedNeighbors = true;
            }
        }

        // already visited all neighbors at this vertex so backtrack
        if (!addedNeighbors) {
            currentPathIndex--;
            visited[current->getLocation()] = false;
        }
    }

    // sort the paths by total path distance
    sort(paths.begin(), paths.end(), compPaths);

    return paths;
}

void AirlineRouter::addFlightPaths() {
    for (auto& requestedFlight: requestedFlights) {
        optimalFlightPaths.push_back(findFlightPaths(requestedFlight));
    }
}

std::ostream& operator<<(std::ostream& os, AirlineRouter& airlineRouter) {
    // for each requested flight, output the requested flight info, and the best flight paths
    for (int i = 0; i < airlineRouter.requestedFlights.size(); i++) {
        auto requestedFlight = airlineRouter.requestedFlights[i];
        auto flightPaths = airlineRouter.optimalFlightPaths[i];

        // output requested flight info
        os << "Flight " << (i + 1) << ": ";
        os << requestedFlight.getFromLocation() << ", ";
        os << requestedFlight.getToLocation() << " ";
        os << "(" << (requestedFlight.getPathOptimization() == PathOptimization::FLIGHT_COST ? "Cost" : "Time") << ")"
           << "\n";

        // iterate through top 3 optimized paths (already in order) and output
        for (int j = 0; j < flightPaths.size() && j < 3; j++) {
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

std::istream& operator>>(std::istream& is, AirlineRouter& airlineRouter) {
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
        std::vector<std::string> splitStr = utils::splitString(line, '|');

        if (splitStr.size() < 3) {
            throw std::out_of_range("requested flight must contain 3 tokens");
        }

        auto pathOptimization = splitStr[2][0] == 'T' ? PathOptimization::FLIGHT_TIME : PathOptimization::FLIGHT_COST;
        RequestedFlight requestedFlight(splitStr[0], splitStr[1], pathOptimization);
        airlineRouter.requestedFlights.push_back(requestedFlight);
    }

    return is;
}
