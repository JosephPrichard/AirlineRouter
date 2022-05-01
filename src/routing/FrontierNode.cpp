//
// Created by Ltpri on 4/22/2022.
//

#include "FrontierNode.h"

FrontierNode::FrontierNode(std::shared_ptr<FrontierNode>& parent, Flight* flight, float cost) {
    this->parent = parent;
    this->flight = flight;
    this->pathCost = cost;
}

FrontierNode::FrontierNode(Flight *flight, float cost) {
    this->parent = nullptr;
    this->flight = flight;
    this->pathCost = cost;
}


Flight& FrontierNode::getFlight() {
    return *flight;
}

float FrontierNode::getPathCost() const {
    return pathCost;
}

bool operator<(const FrontierNode& left, const FrontierNode& right) {
    return left.getPathCost() > right.getPathCost();
}

FrontierNode* FrontierNode::getParent() const {
    return parent.get();
}