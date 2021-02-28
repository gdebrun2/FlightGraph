#pragma once

#include <cmath>
#include <limits>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <queue>

/*
Notes:
All distances in this project are in kilometers

See OpenFlights (https://openflights.org/data.html) for information about the format of its databases

Some airports do not have an OpenFlights ID and are not in airports.dat (see routes.dat line 67503)
Some distinct routes have the same starting and ending airport (e.g. the route is offered by two different airlines) (see routes.dat lines 66149 and 667663)
Some airport names contain a comma
Some airports do not have a code
Some airport routes contain airports that are not stored in the OpenFlights Airports Database (see routes.dat line 30550)

If testing on very large datasets, there may be memory issues since this graph uses an adjacency matrix

FlightGraph has undefined behavior on files that do not follow the OpenFlight format

This graph performs all the insertions/removals during construction
After construction, insertions/removals are not supported

If there is an airport that does not have a known coordinate, its coordinate is set to the South Pole

FlightGraph variables should only be modified by FlightGraph.cpp

For this dataset, |V| < |E| << |V|^2

FlightGraph is designed to run on EWS

Methods may produce undefined behavior if there is more than one connected component or there is a lack of outgoing routes
*/

class FlightGraph {
    public:
        FlightGraph(const std::string& routeFilepath, const std::string& airportFilepath); //Constructor assigns all the variables
        std::vector<std::string> getIncidentAirportCodes(const std::string& originAirportCode); //Returns vertices (airport codes) incident to the given vertex
        bool areAdjacent(const std::string& originAirportCode, const std::string& destinationAirportCode); //Returns whether two airports have an edge between them - note that invalid airport codes will result in undefined behavior

        double convertCoordinatesToKilometers(const std::pair<double, double>& originCoordinates, const std::pair<double, double>& destinationCoordinates); //Computes Great-circle distance in kilometers (note that the haversine formula is numerically well-conditioned) - note that invalid coordinates will result in undefined behavior

        std::vector<std::vector<std::string>> breadthFirstSearch(const std::string& rootAirportCode); //Returns vector of airport codes found in breadth-first search order

        std::vector<std::string> breadthFirstSearch(const std::string& airportCode, std::vector<bool>& visited); //Helper function to find breadth-first search traversal of components disconnected from root airport code

        std::vector<std::string> findShortestPath(const std::string& originAirportCode, const std::string& destinationAirportCode); //Returns the shortest path (a vector of airport codes) using Dijkstra’s Algorithm

        std::vector<std::string> findShortestLandmarkPath(const std::vector<std::string>& airportCodeVector); //Takes in a vector where the first code is the origin, the last code is the final destination, and the middle code is the intermediate landmark - the function returns the shortest path from the origin to the destination through the landmark

        std::map<std::string, std::pair<double, double>> airportCodeToLatitudeLongitudeMap; //Map from an airport's code to its coordinates, which is incomplete due to the incomplete database (see notes above)

        std::map<std::string, int> airportCodeMap; //Map from an airport's code to its index in aiportCodeList (ordered map was used for testing purposes)
        std::vector<std::string> airportCodeList; //List of all airport codes

        std::set<std::pair<std::string, std::string>> edgeSet; //Set of edges (airport code to airport code)
        std::vector<std::pair<std::string, std::string>> edgeList; //List of edges (airport code to airport code)

        //Adjacency matrix is formatted such that you access data using adjacencyMatrix.at(origin).at(destination)
        //If the value is postive, the edge exists
        //If the value is negative, the edge does not exist
        //There should be no zero values
        //The magnitude of positve values represents the number of kilometers between airports
        std::vector<std::vector<double>> adjacencyMatrix;
};
