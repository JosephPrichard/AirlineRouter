#include <iostream>
#include <fstream>
#include "src/network/AirlineNetwork.h"
#include "src/routing/AirlineRouter.h"
#include "src/routing/FrontierNode.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: ./executable <flightGraphFile> <requestedFlightsFile> <outputFile>");
        return 1;
    }

    std::ifstream graphFile;
    graphFile.open(argv[1], std::ios::in);

    std::ifstream flightsFile;
    flightsFile.open(argv[2], std::ios::in);

    std::ofstream pathsFile;
    pathsFile.open(argv[3], std::ios::out);

    std::string line;
    std::getline(graphFile, line);
    int size = std::stoi(line);

    // allocate 2x the buckets we need to maintain a 50% load factor (ideal for an open addressed hash table)
    auto airlineNetwork = std::make_unique<AirlineNetwork>(2 * size);
    try {
        graphFile >> *airlineNetwork;
    } catch (std::exception& e) {
        printf("Failed to parse graphFile: %s %s\n", argv[1], e.what());
        return 1;
    }
    std::cout << "AirlineNetwork: \n" << *airlineNetwork;

    AirlineRouter airlineRouter(airlineNetwork);
    try {
        flightsFile >> airlineRouter;
    } catch (std::exception& e) {
        printf("Failed to parse requestedFlightsFile: %s %s\n", argv[2], e.what());
        return 1;
    }

    airlineRouter.addFlightPaths();

    pathsFile << airlineRouter;
    std::cout << "\nFlightPaths: \n" << airlineRouter;

    graphFile.close();
    flightsFile.close();
    pathsFile.close();

    return 0;
}
