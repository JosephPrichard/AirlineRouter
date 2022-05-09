# AirlineRouter
Airline Router application written in C++ that finds the most optimal paths (retrieves the top 3) between two cities on an airline network using an exhaustive depth first search algorithm. 

Written for my CS3345 Algorithms and Datastructures class at UTD.

## Explanation

Application is comprised of two primary modules AirlineNetwork and AirlineRouter. 
AirlineNetwork is an undirected graph implemented using an open addressed hash table as an adjacency list.
AirlineRouter stores a unique pointer to an AirlineRouter instance and is responsible for running dfs algorithm to calculate the most optimal path for a requested flight path.

Application makes heavy use of smart pointers and modern cpp features to achieve both efficiency and memory safety.

## Usage

./airlineRouter airlineNetworkFile requestedFlightsFile outputFile
