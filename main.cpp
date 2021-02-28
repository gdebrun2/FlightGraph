#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "FlightGraph.h"

int main() {
    //Store options from options.txt in optionsVector
    std::vector<std::string> optionsVector;

    std::ifstream stream("options.txt");
    std::string currentOption;
    while (std::getline(stream, currentOption)) {
        optionsVector.push_back(currentOption);
    }

    const std::string& inputRouteFile = optionsVector.at(0);
    const std::string& inputAirportFile = optionsVector.at(1);
    const std::string& origin = optionsVector.at(2);
    const std::string& landmark = optionsVector.at(3);
    const std::string& destination = optionsVector.at(4);
    
    //Initialize graph
    FlightGraph graph(inputRouteFile, inputAirportFile);

    std::cout << "Input route file: " << inputRouteFile << " (" << graph.edgeList.size() << " unique routes)" << std::endl;
    std::cout << "Input airport file: " << inputAirportFile << " (" << graph.airportCodeList.size() << " unique airports)" << std::endl;
    
    //Create and print breadth-first search traversal
    const std::vector<std::vector<std::string>>& bfs = graph.breadthFirstSearch(origin);
    std::cout << "Breadth-first search starting from " << origin << ": (" << bfs.size() << " components)" << std::endl;
    size_t airportCounter = 0;
    for (size_t componentIndex = 0; componentIndex < bfs.size(); ++componentIndex) {
        std::cout << "Component " << componentIndex + 1 << ": ";
        for (size_t i = 0; i < bfs.at(componentIndex).size(); ++i) {
            std::cout << bfs.at(componentIndex).at(i);
            if (i < bfs.at(componentIndex).size() - 1) {
                std::cout << ", ";
            }
            ++airportCounter;
        }
        std::cout << std::endl;
    }
    std::cout << "(" << airportCounter << " unique airports)" << std::endl;
    
    //Compute and print shortest geographical path
    std::cout << "Geographically shortest path from " << origin << " to " << destination << ": ";
    const std::vector<std::string>& shortestPath = graph.findShortestPath(origin, destination);
    for (size_t i = 0; i < shortestPath.size(); ++i) {
        std::cout << shortestPath.at(i);
        if (i < shortestPath.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    
    //Compute and print shortest landmark path
    std::cout << "Geographically shortest path from " << origin << " to landmark " << landmark << " to final destination " << destination << ": ";
    const std::vector<std::string>& shortestLandmarkPath = graph.findShortestLandmarkPath(std::vector<std::string> {origin, landmark, destination});
    for (size_t i = 0; i < shortestLandmarkPath.size(); ++i) {
        std::cout << shortestLandmarkPath.at(i);
        if (i < shortestLandmarkPath.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}
