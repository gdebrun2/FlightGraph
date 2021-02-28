#include "FlightGraph.h"

//Read lines of file (https://stackoverflow.com/questions/13035674/how-to-read-line-by-line-or-a-whole-text-file-at-once)
//Process comma-delimited string (https://www.tutorialspoint.com/parsing-a-comma-delimited-std-string-in-cplusplus)
FlightGraph::FlightGraph(const std::string& routeFilepath, const std::string& airportFilepath) {
    //Reads routes.dat and assigns airportCodeMap and edgeSet
    std::ifstream routesFile(routeFilepath);
    std::string routeCurrentLine;
    while (std::getline(routesFile, routeCurrentLine)) {
        std::vector<std::string> substringVector;

        std::stringstream lineStream(routeCurrentLine);
        std::string currentSubstring;
        while (std::getline(lineStream, currentSubstring, ',')) {
            substringVector.push_back(currentSubstring);
        }

        airportCodeMap[substringVector.at(2)] = 0;
        airportCodeMap[substringVector.at(4)] = 0;

        edgeSet.insert(std::make_pair(substringVector.at(2), substringVector.at(4)));
    }

    //Updates the values (indices) of airportCodeMap and assigns airportCodeList
    int i = 0;
    for (auto itr = airportCodeMap.begin(); itr != airportCodeMap.end(); ++itr) {
        itr->second = i;
        airportCodeList.push_back(itr->first);
        ++i;
    }

    //Assigns edgeList
    for (auto itr = edgeSet.begin(); itr != edgeSet.end(); ++itr) {
        edgeList.push_back(*itr);
    }

    //Assigns adjacencyMatrix
    adjacencyMatrix = std::vector<std::vector<double>>(airportCodeList.size(), std::vector<double>(airportCodeList.size(), -1.0));
    for (const std::pair<std::string, std::string>& edge : edgeList) {
        adjacencyMatrix.at(airportCodeMap.at(edge.first)).at(airportCodeMap.at(edge.second)) = 1.0;
    }

    //Reads airports-extended.dat and assigns airportCodeToLatitudeLongitudeMap
    std::ifstream airportFile(airportFilepath);
    std::string airportCurrentLine;
    while (std::getline(airportFile, airportCurrentLine)) {
        std::vector<std::string> substringVector;

        std::stringstream lineStream(airportCurrentLine);
        std::string currentSubstring;
        while (std::getline(lineStream, currentSubstring, ',')) {
            substringVector.push_back(currentSubstring);
        }

        int offset = (int) substringVector.size() - 14; //Some airport names contain extra commas, so I use an offset to correct this issue
        const std::string& currentCode = substringVector.at(4 + offset);
        const std::string& cleanedCode = currentCode.substr(1, currentCode.size() - 2); //Removes the first and last character, which are usually quotation marks
        if (currentCode.size() > 3 && airportCodeMap.count(cleanedCode) == 1) { //Checks if the current code is valid and is an element of airportCodeMap
            airportCodeToLatitudeLongitudeMap[cleanedCode] = std::make_pair(std::stod(substringVector.at(6 + offset)), std::stod(substringVector.at(7 + offset)));
        }
    }

    //Assigns any missing coordinates to the South Pole
    for (auto itr = airportCodeMap.begin(); itr != airportCodeMap.end(); ++itr) {
        if (airportCodeToLatitudeLongitudeMap.count(itr->first) == 0) {
            airportCodeToLatitudeLongitudeMap[itr->first] = std::make_pair(-90.0, 0.0);
        }
    }

    //Modifies the positive entries of adjacencyMatrix such that the magnitude of each value is the distance between the airports in kilometers (i.e., the nonexistent edges are ignored)
    for (const std::pair<std::string, std::string>& edge : edgeList) {
        adjacencyMatrix.at(airportCodeMap.at(edge.first)).at(airportCodeMap.at(edge.second)) = convertCoordinatesToKilometers(airportCodeToLatitudeLongitudeMap.at(edge.first), airportCodeToLatitudeLongitudeMap.at(edge.second));
    }
}

//Retrieves incident airport codes
std::vector<std::string> FlightGraph::getIncidentAirportCodes(const std::string& originAirportCode) {
    std::vector<std::string> adjacencyList;
    for (const std::string& currentAirportCode : airportCodeList) {
        if (areAdjacent(originAirportCode, currentAirportCode)) {
            adjacencyList.push_back(currentAirportCode);
        }
    }
    return adjacencyList;
}

//Checks whether two airports are adjacent
bool FlightGraph::areAdjacent(const std::string& originAirportCode, const std::string& destinationAirportCode) {
    return adjacencyMatrix.at(airportCodeMap.at(originAirportCode)).at(airportCodeMap.at(destinationAirportCode)) > 0.0;
}

//Great-circle distance (https://en.wikipedia.org/wiki/Haversine_formula)
//(https://stackoverflow.com/questions/21867617/best-platform-independent-pi-constant)
double FlightGraph::convertCoordinatesToKilometers(const std::pair<double, double>& originCoordinates, const std::pair<double, double>& destinationCoordinates) {
    double radiusOfEarth = 6378.137;
    double PI = std::atan(1.0) * 4.0;

    double originLatitudeRadians = originCoordinates.first * PI / 180.0;
    double originLongitudeRadians = originCoordinates.second * PI / 180.0;
    double destinationLatitudeRadians = destinationCoordinates.first * PI / 180.0;
    double destinationLongitudeRadians = destinationCoordinates.second * PI / 180.0;

    double differenceLatitudeRadians = destinationLatitudeRadians - originLatitudeRadians;
    double differenceLongitudeRadians = destinationLongitudeRadians - originLongitudeRadians;

    return 2.0 * radiusOfEarth * std::asin(std::sqrt(std::pow(std::sin(differenceLatitudeRadians / 2.0), 2.0) + std::cos(originLatitudeRadians) * std::cos(destinationLatitudeRadians) * std::pow(std::sin(differenceLongitudeRadians / 2.0), 2.0)));
}

std::vector<std::vector<std::string>> FlightGraph::breadthFirstSearch(const std::string& rootAirportCode) {
    std::vector<bool> visited = std::vector<bool>(airportCodeList.size(), false);

    std::vector<std::vector<std::string>> bfs;

    bfs.push_back(breadthFirstSearch(rootAirportCode, visited));

    for (const std::string& code : airportCodeList) {
        if (!visited.at(airportCodeMap.at(code))) {
            bfs.push_back(breadthFirstSearch(code, visited)); //Insert traversal of new component into bfs
        }
    }

    return bfs;
}

//Helper function for BFS above 
std::vector<std::string> FlightGraph::breadthFirstSearch(const std::string& airportCode, std::vector<bool>& visited) {
    std::vector<std::string> traversal;

    std::queue<std::string> queue;
    visited.at(airportCodeMap.at(airportCode)) = true;

    queue.push(airportCode);
    while (!queue.empty()) {
        const std::string front = queue.front();
        traversal.push_back(front);
        queue.pop();
        const std::vector<std::string>& incident = getIncidentAirportCodes(front);
        for (const std::string& i : incident) {
            if (!visited.at(airportCodeMap.at(i))) {
                visited.at(airportCodeMap.at(i)) = true;
                queue.push(i);
            }
        }
    }

    return traversal;
}

//(https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-set-in-stl/)
std::vector<std::string> FlightGraph::findShortestPath(const std::string& originAirportCode, const std::string& destinationAirportCode) {
    std::vector<double> distance = std::vector<double>(airportCodeList.size(), std::numeric_limits<double>::max());
    std::vector<std::string> predecessor = std::vector<std::string>(airportCodeList.size(), "");
    distance.at(airportCodeMap.at(originAirportCode)) = 0;

    std::set<std::pair<double, std::string>> minDistanceSet;
    for (size_t i = 0; i < airportCodeList.size(); ++i) {
        minDistanceSet.insert(std::pair<double, std::string>(distance.at(i), airportCodeList.at(i)));
    }

    std::set<std::string> visitedAirportSet;

    while (!minDistanceSet.empty()) {
        const std::string minVertex = minDistanceSet.begin()->second;
        minDistanceSet.erase(minDistanceSet.begin());
        visitedAirportSet.insert(minVertex);
        const std::vector<std::string>& incident = getIncidentAirportCodes(minVertex);
        for (const std::string& code : incident) {
            if (visitedAirportSet.count(code) == 0 && adjacencyMatrix.at(airportCodeMap.at(minVertex)).at(airportCodeMap.at(code)) + distance.at(airportCodeMap.at(minVertex)) < distance.at(airportCodeMap.at(code))) {
                minDistanceSet.erase(minDistanceSet.find(std::pair<double, std::string>(distance.at(airportCodeMap.at(code)), code)));
                distance.at(airportCodeMap.at(code)) = adjacencyMatrix.at(airportCodeMap.at(minVertex)).at(airportCodeMap.at(code)) + distance.at(airportCodeMap.at(minVertex));
                predecessor.at(airportCodeMap.at(code)) = minVertex;
                minDistanceSet.insert(std::pair<double, std::string>(distance.at(airportCodeMap.at(code)), code));
            }
        }
    }

    std::vector<std::string> shortestPath {destinationAirportCode};
    if (predecessor.at(airportCodeMap.at(destinationAirportCode)).compare("") == 0) {
        return shortestPath;
    }
    std::string currentCode = destinationAirportCode;
    while (currentCode != originAirportCode) {
        currentCode = predecessor.at(airportCodeMap.at(currentCode));
        shortestPath.push_back(currentCode);
    }
    std::reverse(shortestPath.begin(), shortestPath.end());
    return shortestPath;
}

//(https://stackoverflow.com/questions/27663775/remove-consecutive-duplicate-values-in-a-string)
std::vector<std::string> FlightGraph::findShortestLandmarkPath(const std::vector<std::string>& airportCodeVector) {
    std::vector<std::string> shortestLandmarkPath;

    if (airportCodeVector.size() < 2) {
        return shortestLandmarkPath;
    }

    for (size_t i = 0; i < airportCodeVector.size() - 1 ; ++i) {
        const std::vector<std::string>& path = findShortestPath(airportCodeVector.at(i), airportCodeVector.at(i + 1));
        shortestLandmarkPath.insert(shortestLandmarkPath.end(), path.begin(), path.end());
    }

    shortestLandmarkPath.erase(std::unique(shortestLandmarkPath.begin(), shortestLandmarkPath.end()), shortestLandmarkPath.end()); //Remove consecutive duplicate elements

    return shortestLandmarkPath;
}
