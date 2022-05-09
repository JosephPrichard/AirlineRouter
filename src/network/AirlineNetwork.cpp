//
// Created by Ltpri on 4/14/2022.
//

#include "AirlineNetwork.h"
#include "../utils/StringUtils.h"

AirlineNetwork::AirlineNetwork(int numBuckets) {
    hashTable.resize(numBuckets);
    for (auto& pair: hashTable) {
        pair = nullptr;
    }
}

int AirlineNetwork::addFlight(const std::string& location, const Flight& flight) {
    int count = 0;
    for (int i = 0; count < hashTable.size(); i++) {
        int index = hash(location, i);
        if (hashTable[index] != nullptr) {
            // bucket already contains an element
            if (hashTable[index]->first == location) {
                // bucket contains matching key so we insert flight
                hashTable[index]->second->push_back(flight);
                // first time this key is added, increment size
                size++;
                return 1;
            }
            // otherwise, continue probing for a place to insert flight
            count++;
        } else {
            // bucket is empty insert a new vector containing the flight
            hashTable[index] = std::make_unique<Bucket>(location, std::make_unique<std::list<Flight>>());
            hashTable[index]->second->push_back(flight);
            return 1;
        }
    }
    return 0;
}

std::list<Flight>* AirlineNetwork::neighborFlights(const std::string& location) {
    int count = 0;
    for (int i = 0; count < hashTable.size(); i++) {
        int index = hash(location, i);
        if (hashTable[index] != nullptr) {
            // bucket contains an element
            if (hashTable[index]->first == location) {
                // bucket contains matching key so we return vector
                return hashTable[index]->second.get();
            }
            // otherwise, continue probing to find matching key
            count++;
        } else {
            // bucket is empty so key isn't contained in the table
            return nullptr;
        }
    }
    return nullptr;
}

std::vector<std::string*> AirlineNetwork::locations() {
    std::vector<std::string*> locations;
    // for each pair in hash table
    for (auto& pair: hashTable) {
        if (pair != nullptr) {
            locations.push_back(&pair->first);
        }
    }
    return locations;
}

int AirlineNetwork::numLocations() const {
    return size;
}

int AirlineNetwork::hash(const std::string& location, int i) {
    // linear probing hash function - leads to primary clustering but reaps incredible cache performance
    auto h = (hasher(location) + i) % hashTable.size();
    return (int) h;
}

std::ostream& operator<<(std::ostream& os, AirlineNetwork& airlineNetwork) {
    auto locations = airlineNetwork.locations();
    // for each location retrieve the neighbor flights and output
    for (auto& location: locations) {
        // retrieve neighbor flights for location
        auto flights = airlineNetwork.neighborFlights(*location);
        for (auto& flight: *flights) {
            // output neighbor flight to output stream
            os << *location << "|" << flight.getLocation() << "|";
            os << flight.getCost() << "|" << flight.getTime() << "\n";
        }
    }
    return os;
}

std::istream& operator>>(std::istream& is, AirlineNetwork& airlineNetwork) {
    std::string line;
    while (std::getline(is, line)) {
        std::vector<std::string> splitStr = utils::splitString(line, '|');

        if (splitStr.size() < 4) {
            throw std::out_of_range("flight must contain 4 tokens");
        }

        // create flight objects
        Flight flight0To1(splitStr[1], std::stof(splitStr[2]), std::stoi(splitStr[3]));
        Flight flight1To0(splitStr[0], std::stof(splitStr[2]), std::stoi(splitStr[3]));

        // add fights both says since this is an undirected graph
        if (airlineNetwork.addFlight(splitStr[0], flight0To1) == 0) {
            return is;
        }
        if (airlineNetwork.addFlight(splitStr[1], flight1To0) == 0) {
            return is;
        }
    }
    return is;
}