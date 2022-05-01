//
// Created by Ltpri on 4/14/2022.
//

#ifndef AIRLINESHORTESTPATHS_FLIGHT_H
#define AIRLINESHORTESTPATHS_FLIGHT_H

#include <string>

class Flight {
private:
    std::string location;
    float cost = 0.0f;
    int time = 0;
public:
    Flight();

    Flight(const std::string &location, float cost, int time);

    const std::string& getLocation() const;

    float getCost() const;

    int getTime() const;
};


#endif //AIRLINESHORTESTPATHS_FLIGHT_H
