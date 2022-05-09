cmake_minimum_required(VERSION 3.21)
project(AirlineRouter)

set(CMAKE_CXX_STANDARD 14)

add_executable(AirlineRouter main.cpp src/network/AirlineNetwork.cpp src/network/AirlineNetwork.h src/network/Flight.cpp src/network/Flight.h src/routing/AirlineRouter.cpp src/routing/AirlineRouter.h src/routing/RequestedFlight.cpp src/routing/RequestedFlight.h src/utils/StringUtils.cpp src/utils/StringUtils.h src/stack/Stack.h src/routing/FlightPath.cpp src/routing/FlightPath.h)
