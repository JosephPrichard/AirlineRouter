cmake_minimum_required(VERSION 3.21)
project(AirlineShortestPaths)

set(CMAKE_CXX_STANDARD 14)

add_executable(AirlineShortestPaths main.cpp src/network/AirlineNetwork.cpp src/network/AirlineNetwork.h src/network/Flight.cpp src/network/Flight.h src/routing/AirlineRouter.cpp src/routing/AirlineRouter.h src/routing/RequestedFlight.cpp src/routing/RequestedFlight.h src/Utils.cpp src/Utils.h src/routing/FrontierNode.cpp src/routing/FrontierNode.h)
