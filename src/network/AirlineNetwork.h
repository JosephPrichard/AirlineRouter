//
// Created by Joseph Prichard on 4/14/2022.
//

#ifndef AIRLINESHORTESTPATHS_AIRLINENETWORK_H
#define AIRLINESHORTESTPATHS_AIRLINENETWORK_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <list>
#include "Flight.h"

typedef std::pair<std::string, std::unique_ptr<std::list<Flight>>> Bucket;

// undirected graph ADT to store which flights are available for each location
// implemented with an adjacency list which is in turn implemented with an open addressed hash table
class AirlineNetwork {
private:
    std::vector<std::unique_ptr<Bucket>> hashTable;
    std::hash<std::string> hasher;
    int size = 0;

public:
    explicit AirlineNetwork(int numBuckets);

    // add an edge (flight) to the graph
    int addFlight(const std::string& location, const Flight& flight);

    // retrieve all neighbor edges (flights) for a given location in the graph
    std::list<Flight>* neighborFlights(const std::string& location);

    // retrieve all vertices (locations) in the graph
    std::vector<std::string*> locations();

    // retrieve the number of vertices (location) in the graph
    int numLocations() const;

    // hashes a location for the hash table using linear probing
    int hash(const std::string& location, int i);

    // read an input stream containing edges (flights) into the airline network
    friend std::ostream& operator<<(std::ostream& os, AirlineNetwork& airlineNetwork);

    // write edges (flights) to an output stream
    friend std::istream& operator>>(std::istream& is, AirlineNetwork& airlineNetwork);
};

#endif //AIRLINESHORTESTPATHS_AIRLINENETWORK_H
