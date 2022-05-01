//
// Created by Ltpri on 4/22/2022.
//

#ifndef AIRLINESHORTESTPATHS_FRONTIERNODE_H
#define AIRLINESHORTESTPATHS_FRONTIERNODE_H

#include <memory>
#include "../network/Flight.h"

class FrontierNode {
private:
    std::shared_ptr<FrontierNode> parent;
    Flight* flight;
    float pathCost;
public:
    FrontierNode(std::shared_ptr<FrontierNode>& parent, Flight* flight, float cost);

    FrontierNode(Flight* flight, float cost);

    FrontierNode *getParent() const;

    Flight& getFlight();

    float getPathCost() const;

    friend bool operator<(const FrontierNode& left, const FrontierNode& right);
};

#endif //AIRLINESHORTESTPATHS_FRONTIERNODE_H
