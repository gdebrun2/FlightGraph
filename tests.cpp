#include <map>
#include <set>
#include <string>

#include "catch.hpp"

#include "FlightGraph.h"

/*
Test format copied from mp_lists
Test distances computed using (https://www.nhc.noaa.gov/gccalc.shtml)
*/

/*
routes-test-undirected.dat
          1039       870
      ORD------RDU----------YYZ
     / |        | \          | \
 218/  |     360|  \1071 1050|  \1088
   /   |1290    |   \        |   \
CMI    |       IAD   ----    |    MSP
   \   |  1881/ |        \   |   /
1114\  |  ----  |1912     \  |  /721
     \ | /      |          \ | /
      DFW------IAH----------STL
          361       1075

routes-test-directed.dat
Only the edges between CMI, ORD, and DFW are changed
      ORD
     ^ ^
    /  |
   /   |
CMI    |
   ^   |
    \  |
     \ |
      DFW
*/
TEST_CASE("FlightGraph Undirected") { //Tests constructors and member variables
    FlightGraph graph("routes-test-undirected.dat", "airports-test.dat");

    //airportCodeToLatitudeLongitudeMap
    REQUIRE(graph.airportCodeToLatitudeLongitudeMap == std::map<std::string, std::pair<double, double>> {
        {"YYZ", {43.6772003174, -79.63059997559999}},
        {"IAH", {29.984399795532227, -95.34140014648438}},
        {"RDU", {35.877601623535156, -78.7874984741211}},
        {"DFW", {32.896801, -97.038002}},
        {"STL", {38.748697, -90.370003}},
        {"IAD", {38.94449997, -77.45580292}},
        {"ORD", {41.9786, -87.9048}},
        {"MSP", {44.882, -93.221802}},
        {"CMI", {40.03919983, -88.27809906}}
    });

    //airportCodeMap
    REQUIRE(graph.airportCodeMap == std::map<std::string, int> {
        {"CMI", 0},
        {"DFW", 1},
        {"IAD", 2},
        {"IAH", 3},
        {"MSP", 4},
        {"ORD", 5},
        {"RDU", 6},
        {"STL", 7},
        {"YYZ", 8}
    });

    //airportCodeList
    REQUIRE(graph.airportCodeList == std::vector<std::string> {
        "CMI",
        "DFW",
        "IAD",
        "IAH",
        "MSP",
        "ORD",
        "RDU",
        "STL",
        "YYZ"
    });

    //edgeSet
    REQUIRE(graph.edgeSet == std::set<std::pair<std::string, std::string>> {
        {"CMI", "ORD"},
        {"ORD", "CMI"},
        {"CMI", "DFW"},
        {"DFW", "CMI"},
        {"ORD", "DFW"},
        {"DFW", "ORD"},
        {"ORD", "RDU"},
        {"RDU", "ORD"},
        {"DFW", "IAD"},
        {"IAD", "DFW"},
        {"DFW", "IAH"},
        {"IAH", "DFW"},
        {"RDU", "IAD"},
        {"IAD", "RDU"},
        {"IAD", "IAH"},
        {"IAH", "IAD"},
        {"RDU", "YYZ"},
        {"YYZ", "RDU"},
        {"RDU", "STL"},
        {"STL", "RDU"},
        {"IAH", "STL"},
        {"STL", "IAH"},
        {"YYZ", "STL"},
        {"STL", "YYZ"},
        {"YYZ", "MSP"},
        {"MSP", "YYZ"},
        {"STL", "MSP"},
        {"MSP", "STL"}
    });

    //edgeList
    REQUIRE(graph.edgeList == std::vector<std::pair<std::string, std::string>> {
        {"CMI", "DFW"},
        {"CMI", "ORD"},
        {"DFW", "CMI"},
        {"DFW", "IAD"},
        {"DFW", "IAH"},
        {"DFW", "ORD"},
        {"IAD", "DFW"},
        {"IAD", "IAH"},
        {"IAD", "RDU"},
        {"IAH", "DFW"},
        {"IAH", "IAD"},
        {"IAH", "STL"},
        {"MSP", "STL"},
        {"MSP", "YYZ"},
        {"ORD", "CMI"},
        {"ORD", "DFW"},
        {"ORD", "RDU"},
        {"RDU", "IAD"},
        {"RDU", "ORD"},
        {"RDU", "STL"},
        {"RDU", "YYZ"},
        {"STL", "IAH"},
        {"STL", "MSP"},
        {"STL", "RDU"},
        {"STL", "YYZ"},
        {"YYZ", "MSP"},
        {"YYZ", "RDU"},
        {"YYZ", "STL"}
    });

    //adjacencyMatrix
    std::vector<std::vector<double>> adjacencyMatrixTest = {
        {-1, 1114, -1, -1, -1, 218, -1, -1, -1},
        {1114, -1, 1881, 361, -1, 1290, -1, -1, -1},
        {-1, 1881, -1, 1912, -1, -1, 360, -1, -1},
        {-1, 361, 1912, -1, -1, -1, -1, 1075, -1},
        {-1, -1, -1, -1, -1, -1, -1, 721, 1088},
        {218, 1290, -1, -1, -1, -1, 1039, -1, -1},
        {-1, -1, 360, -1, -1, 1039, -1, 1071, 870},
        {-1, -1, -1, 1075, 721, -1, 1071, -1, 1050},
        {-1, -1, -1, -1, 1088, -1, 870, 1050, -1}
    };
    REQUIRE(graph.adjacencyMatrix.size() == adjacencyMatrixTest.size()); //Size tests
    for (size_t originIndex = 0; originIndex < adjacencyMatrixTest.size(); ++originIndex) {
        REQUIRE(graph.adjacencyMatrix.at(originIndex).size() == adjacencyMatrixTest.at(originIndex).size());
    }
    double relativeErrorTolerance = 0.01;
    for (size_t originIndex = 0; originIndex < adjacencyMatrixTest.size(); ++originIndex) { //Value tests
        for (size_t destinationIndex = 0; destinationIndex < adjacencyMatrixTest.at(originIndex).size(); ++destinationIndex) {
            REQUIRE(graph.adjacencyMatrix.at(originIndex).at(destinationIndex) == Approx(adjacencyMatrixTest.at(originIndex).at(destinationIndex)).epsilon(relativeErrorTolerance));
            REQUIRE_FALSE(graph.adjacencyMatrix.at(originIndex).at(destinationIndex) == 0);
        }
    }
}

TEST_CASE("FlightGraph Directed") {
    FlightGraph graph("routes-test-directed.dat", "airports-test.dat");

    //airportCodeToLatitudeLongitudeMap
    REQUIRE(graph.airportCodeToLatitudeLongitudeMap == std::map<std::string, std::pair<double, double>> {
        {"YYZ", {43.6772003174, -79.63059997559999}},
        {"IAH", {29.984399795532227, -95.34140014648438}},
        {"RDU", {35.877601623535156, -78.7874984741211}},
        {"DFW", {32.896801, -97.038002}},
        {"STL", {38.748697, -90.370003}},
        {"IAD", {38.94449997, -77.45580292}},
        {"ORD", {41.9786, -87.9048}},
        {"MSP", {44.882, -93.221802}},
        {"CMI", {40.03919983, -88.27809906}}
    });

    //airportCodeMap
    REQUIRE(graph.airportCodeMap == std::map<std::string, int> {
        {"CMI", 0},
        {"DFW", 1},
        {"IAD", 2},
        {"IAH", 3},
        {"MSP", 4},
        {"ORD", 5},
        {"RDU", 6},
        {"STL", 7},
        {"YYZ", 8}
    });

    //airportCodeList
    REQUIRE(graph.airportCodeList == std::vector<std::string> {
        "CMI",
        "DFW",
        "IAD",
        "IAH",
        "MSP",
        "ORD",
        "RDU",
        "STL",
        "YYZ"
    });

    //edgeSet
    REQUIRE(graph.edgeSet == std::set<std::pair<std::string, std::string>> {
        {"CMI", "ORD"},
        {"DFW", "CMI"},
        {"DFW", "ORD"},
        {"ORD", "RDU"},
        {"RDU", "ORD"},
        {"DFW", "IAD"},
        {"IAD", "DFW"},
        {"DFW", "IAH"},
        {"IAH", "DFW"},
        {"RDU", "IAD"},
        {"IAD", "RDU"},
        {"IAD", "IAH"},
        {"IAH", "IAD"},
        {"RDU", "YYZ"},
        {"YYZ", "RDU"},
        {"RDU", "STL"},
        {"STL", "RDU"},
        {"IAH", "STL"},
        {"STL", "IAH"},
        {"YYZ", "STL"},
        {"STL", "YYZ"},
        {"YYZ", "MSP"},
        {"MSP", "YYZ"},
        {"STL", "MSP"},
        {"MSP", "STL"}
    });

    //edgeList
    REQUIRE(graph.edgeList == std::vector<std::pair<std::string, std::string>> {
        {"CMI", "ORD"},
        {"DFW", "CMI"},
        {"DFW", "IAD"},
        {"DFW", "IAH"},
        {"DFW", "ORD"},
        {"IAD", "DFW"},
        {"IAD", "IAH"},
        {"IAD", "RDU"},
        {"IAH", "DFW"},
        {"IAH", "IAD"},
        {"IAH", "STL"},
        {"MSP", "STL"},
        {"MSP", "YYZ"},
        {"ORD", "RDU"},
        {"RDU", "IAD"},
        {"RDU", "ORD"},
        {"RDU", "STL"},
        {"RDU", "YYZ"},
        {"STL", "IAH"},
        {"STL", "MSP"},
        {"STL", "RDU"},
        {"STL", "YYZ"},
        {"YYZ", "MSP"},
        {"YYZ", "RDU"},
        {"YYZ", "STL"}
    });

    //adjacencyMatrix
    std::vector<std::vector<double>> adjacencyMatrixTest = {
        {-1, -1, -1, -1, -1, 218, -1, -1, -1},
        {1114, -1, 1881, 361, -1, 1290, -1, -1, -1},
        {-1, 1881, -1, 1912, -1, -1, 360, -1, -1},
        {-1, 361, 1912, -1, -1, -1, -1, 1075, -1},
        {-1, -1, -1, -1, -1, -1, -1, 721, 1088},
        {-1, -1, -1, -1, -1, -1, 1039, -1, -1},
        {-1, -1, 360, -1, -1, 1039, -1, 1071, 870},
        {-1, -1, -1, 1075, 721, -1, 1071, -1, 1050},
        {-1, -1, -1, -1, 1088, -1, 870, 1050, -1}
    };
    REQUIRE(graph.adjacencyMatrix.size() == adjacencyMatrixTest.size()); //Size tests
    for (size_t originIndex = 0; originIndex < adjacencyMatrixTest.size(); ++originIndex) {
        REQUIRE(graph.adjacencyMatrix.at(originIndex).size() == adjacencyMatrixTest.at(originIndex).size());
    }
    double relativeErrorTolerance = 0.01;
    for (size_t originIndex = 0; originIndex < adjacencyMatrixTest.size(); ++originIndex) { //Value tests
        for (size_t destinationIndex = 0; destinationIndex < adjacencyMatrixTest.at(originIndex).size(); ++destinationIndex) {
            REQUIRE(graph.adjacencyMatrix.at(originIndex).at(destinationIndex) == Approx(adjacencyMatrixTest.at(originIndex).at(destinationIndex)).epsilon(relativeErrorTolerance));
            REQUIRE_FALSE(graph.adjacencyMatrix.at(originIndex).at(destinationIndex) == 0);
        }
    }
}

TEST_CASE("getIncidentAirportCodes Undirected") {
    FlightGraph graph("routes-test-undirected.dat", "airports-test.dat");

    REQUIRE(graph.getIncidentAirportCodes("CMI") == std::vector<std::string> {"DFW", "ORD"});
    REQUIRE(graph.getIncidentAirportCodes("DFW") == std::vector<std::string> {"CMI", "IAD", "IAH", "ORD"});
    REQUIRE(graph.getIncidentAirportCodes("IAD") == std::vector<std::string> {"DFW", "IAH", "RDU"});
    REQUIRE(graph.getIncidentAirportCodes("IAH") == std::vector<std::string> {"DFW", "IAD", "STL"});
    REQUIRE(graph.getIncidentAirportCodes("MSP") == std::vector<std::string> {"STL", "YYZ"});
    REQUIRE(graph.getIncidentAirportCodes("ORD") == std::vector<std::string> {"CMI", "DFW", "RDU"});
    REQUIRE(graph.getIncidentAirportCodes("RDU") == std::vector<std::string> {"IAD", "ORD", "STL", "YYZ"});
    REQUIRE(graph.getIncidentAirportCodes("STL") == std::vector<std::string> {"IAH", "MSP", "RDU", "YYZ"});
    REQUIRE(graph.getIncidentAirportCodes("YYZ") == std::vector<std::string> {"MSP", "RDU", "STL"});
}

TEST_CASE("getIncidentAirportCodes Directed") {
    FlightGraph graph("routes-test-directed.dat", "airports-test.dat");

    REQUIRE(graph.getIncidentAirportCodes("CMI") == std::vector<std::string> {"ORD"});
    REQUIRE(graph.getIncidentAirportCodes("DFW") == std::vector<std::string> {"CMI", "IAD", "IAH", "ORD"});
    REQUIRE(graph.getIncidentAirportCodes("IAD") == std::vector<std::string> {"DFW", "IAH", "RDU"});
    REQUIRE(graph.getIncidentAirportCodes("IAH") == std::vector<std::string> {"DFW", "IAD", "STL"});
    REQUIRE(graph.getIncidentAirportCodes("MSP") == std::vector<std::string> {"STL", "YYZ"});
    REQUIRE(graph.getIncidentAirportCodes("ORD") == std::vector<std::string> {"RDU"});
    REQUIRE(graph.getIncidentAirportCodes("RDU") == std::vector<std::string> {"IAD", "ORD", "STL", "YYZ"});
    REQUIRE(graph.getIncidentAirportCodes("STL") == std::vector<std::string> {"IAH", "MSP", "RDU", "YYZ"});
    REQUIRE(graph.getIncidentAirportCodes("YYZ") == std::vector<std::string> {"MSP", "RDU", "STL"});
}

TEST_CASE("areAdjacent Undirected") {
    FlightGraph graph("routes-test-undirected.dat", "airports-test.dat");

    REQUIRE_FALSE(graph.areAdjacent("CMI", "CMI"));
    REQUIRE(graph.areAdjacent("CMI", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "MSP"));
    REQUIRE(graph.areAdjacent("CMI", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "YYZ"));

    REQUIRE(graph.areAdjacent("DFW", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "DFW"));
    REQUIRE(graph.areAdjacent("DFW", "IAD"));
    REQUIRE(graph.areAdjacent("DFW", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "MSP"));
    REQUIRE(graph.areAdjacent("DFW", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("IAD", "CMI"));
    REQUIRE(graph.areAdjacent("IAD", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "IAD"));
    REQUIRE(graph.areAdjacent("IAD", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "ORD"));
    REQUIRE(graph.areAdjacent("IAD", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("IAH", "CMI"));
    REQUIRE(graph.areAdjacent("IAH", "DFW"));
    REQUIRE(graph.areAdjacent("IAH", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "RDU"));
    REQUIRE(graph.areAdjacent("IAH", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("MSP", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "RDU"));
    REQUIRE(graph.areAdjacent("MSP", "STL"));
    REQUIRE(graph.areAdjacent("MSP", "YYZ"));

    REQUIRE(graph.areAdjacent("ORD", "CMI"));
    REQUIRE(graph.areAdjacent("ORD", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "ORD"));
    REQUIRE(graph.areAdjacent("ORD", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("RDU", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "DFW"));
    REQUIRE(graph.areAdjacent("RDU", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "MSP"));
    REQUIRE(graph.areAdjacent("RDU", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "RDU"));
    REQUIRE(graph.areAdjacent("RDU", "STL"));
    REQUIRE(graph.areAdjacent("RDU", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("STL", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "IAD"));
    REQUIRE(graph.areAdjacent("STL", "IAH"));
    REQUIRE(graph.areAdjacent("STL", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "ORD"));
    REQUIRE(graph.areAdjacent("STL", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "STL"));
    REQUIRE(graph.areAdjacent("STL", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("YYZ", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "IAH"));
    REQUIRE(graph.areAdjacent("YYZ", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "ORD"));
    REQUIRE(graph.areAdjacent("YYZ", "RDU"));
    REQUIRE(graph.areAdjacent("YYZ", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "YYZ"));
}

TEST_CASE("areAdjacent Directed") {
    FlightGraph graph("routes-test-directed.dat", "airports-test.dat");

    REQUIRE_FALSE(graph.areAdjacent("CMI", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "MSP"));
    REQUIRE(graph.areAdjacent("CMI", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("CMI", "YYZ"));

    REQUIRE(graph.areAdjacent("DFW", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "DFW"));
    REQUIRE(graph.areAdjacent("DFW", "IAD"));
    REQUIRE(graph.areAdjacent("DFW", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "MSP"));
    REQUIRE(graph.areAdjacent("DFW", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("DFW", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("IAD", "CMI"));
    REQUIRE(graph.areAdjacent("IAD", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "IAD"));
    REQUIRE(graph.areAdjacent("IAD", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "ORD"));
    REQUIRE(graph.areAdjacent("IAD", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("IAD", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("IAH", "CMI"));
    REQUIRE(graph.areAdjacent("IAH", "DFW"));
    REQUIRE(graph.areAdjacent("IAH", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "RDU"));
    REQUIRE(graph.areAdjacent("IAH", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("IAH", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("MSP", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("MSP", "RDU"));
    REQUIRE(graph.areAdjacent("MSP", "STL"));
    REQUIRE(graph.areAdjacent("MSP", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("ORD", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "ORD"));
    REQUIRE(graph.areAdjacent("ORD", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("ORD", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("RDU", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "DFW"));
    REQUIRE(graph.areAdjacent("RDU", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "IAH"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "MSP"));
    REQUIRE(graph.areAdjacent("RDU", "ORD"));
    REQUIRE_FALSE(graph.areAdjacent("RDU", "RDU"));
    REQUIRE(graph.areAdjacent("RDU", "STL"));
    REQUIRE(graph.areAdjacent("RDU", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("STL", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "IAD"));
    REQUIRE(graph.areAdjacent("STL", "IAH"));
    REQUIRE(graph.areAdjacent("STL", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "ORD"));
    REQUIRE(graph.areAdjacent("STL", "RDU"));
    REQUIRE_FALSE(graph.areAdjacent("STL", "STL"));
    REQUIRE(graph.areAdjacent("STL", "YYZ"));

    REQUIRE_FALSE(graph.areAdjacent("YYZ", "CMI"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "DFW"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "IAD"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "IAH"));
    REQUIRE(graph.areAdjacent("YYZ", "MSP"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "ORD"));
    REQUIRE(graph.areAdjacent("YYZ", "RDU"));
    REQUIRE(graph.areAdjacent("YYZ", "STL"));
    REQUIRE_FALSE(graph.areAdjacent("YYZ", "YYZ"));
}

TEST_CASE("breadthFirstSearch Undirected") {
    FlightGraph graph("routes-test-undirected.dat", "airports-test.dat");

    REQUIRE(graph.breadthFirstSearch("CMI") == std::vector<std::vector<std::string>> {{"CMI", "DFW", "ORD", "IAD", "IAH", "RDU", "STL", "YYZ", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("DFW") == std::vector<std::vector<std::string>> {{"DFW", "CMI", "IAD", "IAH", "ORD", "RDU", "STL", "YYZ", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("IAD") == std::vector<std::vector<std::string>> {{"IAD", "DFW", "IAH", "RDU", "CMI", "ORD", "STL", "YYZ", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("IAH") == std::vector<std::vector<std::string>> {{"IAH", "DFW", "IAD", "STL", "CMI", "ORD", "RDU", "MSP", "YYZ"}});
    REQUIRE(graph.breadthFirstSearch("MSP") == std::vector<std::vector<std::string>> {{"MSP", "STL", "YYZ", "IAH", "RDU", "DFW", "IAD", "ORD", "CMI"}});
    REQUIRE(graph.breadthFirstSearch("ORD") == std::vector<std::vector<std::string>> {{"ORD", "CMI", "DFW", "RDU", "IAD", "IAH", "STL", "YYZ", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("RDU") == std::vector<std::vector<std::string>> {{"RDU", "IAD", "ORD", "STL", "YYZ", "DFW", "IAH", "CMI", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("STL") == std::vector<std::vector<std::string>> {{"STL", "IAH", "MSP", "RDU", "YYZ", "DFW", "IAD", "ORD", "CMI"}});
    REQUIRE(graph.breadthFirstSearch("YYZ") == std::vector<std::vector<std::string>> {{"YYZ", "MSP", "RDU", "STL", "IAD", "ORD", "IAH", "DFW", "CMI"}});
}

TEST_CASE("breadthFirstSearch Directed") {
    FlightGraph graph("routes-test-directed.dat", "airports-test.dat");

    REQUIRE(graph.breadthFirstSearch("CMI") == std::vector<std::vector<std::string>> {{"CMI", "ORD", "RDU", "IAD", "STL", "YYZ", "DFW", "IAH", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("ORD") == std::vector<std::vector<std::string>> {{"ORD", "RDU", "IAD", "STL", "YYZ", "DFW", "IAH", "MSP", "CMI"}});
    REQUIRE(graph.breadthFirstSearch("DFW") == std::vector<std::vector<std::string>> {{"DFW", "CMI", "IAD", "IAH", "ORD", "RDU", "STL", "YYZ", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("IAD") == std::vector<std::vector<std::string>> {{"IAD", "DFW", "IAH", "RDU", "CMI", "ORD", "STL", "YYZ", "MSP"}});
    REQUIRE(graph.breadthFirstSearch("IAH") == std::vector<std::vector<std::string>> {{"IAH", "DFW", "IAD", "STL", "CMI", "ORD", "RDU", "MSP", "YYZ"}});
    REQUIRE(graph.breadthFirstSearch("MSP") == std::vector<std::vector<std::string>> {{"MSP", "STL", "YYZ", "IAH", "RDU", "DFW", "IAD", "ORD", "CMI"}});
    REQUIRE(graph.breadthFirstSearch("RDU") == std::vector<std::vector<std::string>> {{"RDU", "IAD", "ORD", "STL", "YYZ", "DFW", "IAH", "MSP", "CMI"}});
    REQUIRE(graph.breadthFirstSearch("STL") == std::vector<std::vector<std::string>> {{"STL", "IAH", "MSP", "RDU", "YYZ", "DFW", "IAD", "ORD", "CMI"}});
    REQUIRE(graph.breadthFirstSearch("YYZ") == std::vector<std::vector<std::string>> {{"YYZ", "MSP", "RDU", "STL", "IAD", "ORD", "IAH", "DFW", "CMI"}});
}

TEST_CASE("findShortestPath Undirected") {
    FlightGraph graph("routes-test-undirected.dat", "airports-test.dat");

    REQUIRE(graph.findShortestPath("CMI", "CMI") == std::vector<std::string> {"CMI"});
    REQUIRE(graph.findShortestPath("CMI", "DFW") == std::vector<std::string> {"CMI", "DFW"});
    REQUIRE(graph.findShortestPath("CMI", "IAD") == std::vector<std::string> {"CMI", "ORD", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("CMI", "IAH") == std::vector<std::string> {"CMI", "DFW", "IAH"});
    REQUIRE(graph.findShortestPath("CMI", "MSP") == std::vector<std::string> {"CMI", "ORD", "RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("CMI", "ORD") == std::vector<std::string> {"CMI", "ORD"});
    REQUIRE(graph.findShortestPath("CMI", "RDU") == std::vector<std::string> {"CMI", "ORD", "RDU"});
    REQUIRE(graph.findShortestPath("CMI", "STL") == std::vector<std::string> {"CMI", "ORD", "RDU", "STL"});
    REQUIRE(graph.findShortestPath("CMI", "YYZ") == std::vector<std::string> {"CMI", "ORD", "RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("DFW", "CMI") == std::vector<std::string> {"DFW", "CMI"});
    REQUIRE(graph.findShortestPath("DFW", "DFW") == std::vector<std::string> {"DFW"});
    REQUIRE(graph.findShortestPath("DFW", "IAD") == std::vector<std::string> {"DFW", "IAD"});
    REQUIRE(graph.findShortestPath("DFW", "IAH") == std::vector<std::string> {"DFW", "IAH"});
    REQUIRE(graph.findShortestPath("DFW", "MSP") == std::vector<std::string> {"DFW", "IAH", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("DFW", "ORD") == std::vector<std::string> {"DFW", "ORD"});
    REQUIRE(graph.findShortestPath("DFW", "RDU") == std::vector<std::string> {"DFW", "IAD", "RDU"});
    REQUIRE(graph.findShortestPath("DFW", "STL") == std::vector<std::string> {"DFW", "IAH", "STL"});
    REQUIRE(graph.findShortestPath("DFW", "YYZ") == std::vector<std::string> {"DFW", "IAH", "STL", "YYZ"});

    REQUIRE(graph.findShortestPath("IAD", "CMI") == std::vector<std::string> {"IAD", "RDU", "ORD", "CMI"});
    REQUIRE(graph.findShortestPath("IAD", "DFW") == std::vector<std::string> {"IAD", "DFW"});
    REQUIRE(graph.findShortestPath("IAD", "IAD") == std::vector<std::string> {"IAD"});
    REQUIRE(graph.findShortestPath("IAD", "IAH") == std::vector<std::string> {"IAD", "IAH"});
    REQUIRE(graph.findShortestPath("IAD", "MSP") == std::vector<std::string> {"IAD", "RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("IAD", "ORD") == std::vector<std::string> {"IAD", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("IAD", "RDU") == std::vector<std::string> {"IAD", "RDU"});
    REQUIRE(graph.findShortestPath("IAD", "STL") == std::vector<std::string> {"IAD", "RDU", "STL"});
    REQUIRE(graph.findShortestPath("IAD", "YYZ") == std::vector<std::string> {"IAD", "RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("IAH", "CMI") == std::vector<std::string> {"IAH", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("IAH", "DFW") == std::vector<std::string> {"IAH", "DFW"});
    REQUIRE(graph.findShortestPath("IAH", "IAD") == std::vector<std::string> {"IAH", "IAD"});
    REQUIRE(graph.findShortestPath("IAH", "IAH") == std::vector<std::string> {"IAH"});
    REQUIRE(graph.findShortestPath("IAH", "MSP") == std::vector<std::string> {"IAH", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("IAH", "ORD") == std::vector<std::string> {"IAH", "DFW", "ORD"});
    REQUIRE(graph.findShortestPath("IAH", "RDU") == std::vector<std::string> {"IAH", "STL", "RDU"});
    REQUIRE(graph.findShortestPath("IAH", "STL") == std::vector<std::string> {"IAH", "STL"});
    REQUIRE(graph.findShortestPath("IAH", "YYZ") == std::vector<std::string> {"IAH", "STL", "YYZ"});

    REQUIRE(graph.findShortestPath("MSP", "CMI") == std::vector<std::string> {"MSP", "STL", "RDU", "ORD", "CMI"});
    REQUIRE(graph.findShortestPath("MSP", "DFW") == std::vector<std::string> {"MSP", "STL", "IAH", "DFW"});
    REQUIRE(graph.findShortestPath("MSP", "IAD") == std::vector<std::string> {"MSP", "STL", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("MSP", "IAH") == std::vector<std::string> {"MSP", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("MSP", "MSP") == std::vector<std::string> {"MSP"});
    REQUIRE(graph.findShortestPath("MSP", "ORD") == std::vector<std::string> {"MSP", "STL", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("MSP", "RDU") == std::vector<std::string> {"MSP", "STL", "RDU"});
    REQUIRE(graph.findShortestPath("MSP", "STL") == std::vector<std::string> {"MSP", "STL"});
    REQUIRE(graph.findShortestPath("MSP", "YYZ") == std::vector<std::string> {"MSP", "YYZ"});

    REQUIRE(graph.findShortestPath("ORD", "CMI") == std::vector<std::string> {"ORD", "CMI"});
    REQUIRE(graph.findShortestPath("ORD", "DFW") == std::vector<std::string> {"ORD", "DFW"});
    REQUIRE(graph.findShortestPath("ORD", "IAD") == std::vector<std::string> {"ORD", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("ORD", "IAH") == std::vector<std::string> {"ORD", "DFW", "IAH"});
    REQUIRE(graph.findShortestPath("ORD", "MSP") == std::vector<std::string> {"ORD", "RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("ORD", "ORD") == std::vector<std::string> {"ORD"});
    REQUIRE(graph.findShortestPath("ORD", "RDU") == std::vector<std::string> {"ORD", "RDU"});
    REQUIRE(graph.findShortestPath("ORD", "STL") == std::vector<std::string> {"ORD", "RDU", "STL"});
    REQUIRE(graph.findShortestPath("ORD", "YYZ") == std::vector<std::string> {"ORD", "RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("RDU", "CMI") == std::vector<std::string> {"RDU", "ORD", "CMI"});
    REQUIRE(graph.findShortestPath("RDU", "DFW") == std::vector<std::string> {"RDU", "IAD", "DFW"});
    REQUIRE(graph.findShortestPath("RDU", "IAD") == std::vector<std::string> {"RDU", "IAD"});
    REQUIRE(graph.findShortestPath("RDU", "IAH") == std::vector<std::string> {"RDU", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("RDU", "MSP") == std::vector<std::string> {"RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("RDU", "ORD") == std::vector<std::string> {"RDU", "ORD"});
    REQUIRE(graph.findShortestPath("RDU", "RDU") == std::vector<std::string> {"RDU"});
    REQUIRE(graph.findShortestPath("RDU", "STL") == std::vector<std::string> {"RDU", "STL"});
    REQUIRE(graph.findShortestPath("RDU", "YYZ") == std::vector<std::string> {"RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("STL", "CMI") == std::vector<std::string> {"STL", "RDU", "ORD", "CMI"});
    REQUIRE(graph.findShortestPath("STL", "DFW") == std::vector<std::string> {"STL", "IAH", "DFW"});
    REQUIRE(graph.findShortestPath("STL", "IAD") == std::vector<std::string> {"STL", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("STL", "IAH") == std::vector<std::string> {"STL", "IAH"});
    REQUIRE(graph.findShortestPath("STL", "MSP") == std::vector<std::string> {"STL", "MSP"});
    REQUIRE(graph.findShortestPath("STL", "ORD") == std::vector<std::string> {"STL", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("STL", "RDU") == std::vector<std::string> {"STL", "RDU"});
    REQUIRE(graph.findShortestPath("STL", "STL") == std::vector<std::string> {"STL"});
    REQUIRE(graph.findShortestPath("STL", "YYZ") == std::vector<std::string> {"STL", "YYZ"});

    REQUIRE(graph.findShortestPath("YYZ", "CMI") == std::vector<std::string> {"YYZ", "RDU", "ORD", "CMI"});
    REQUIRE(graph.findShortestPath("YYZ", "DFW") == std::vector<std::string> {"YYZ", "STL", "IAH", "DFW"});
    REQUIRE(graph.findShortestPath("YYZ", "IAD") == std::vector<std::string> {"YYZ", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("YYZ", "IAH") == std::vector<std::string> {"YYZ", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("YYZ", "MSP") == std::vector<std::string> {"YYZ", "MSP"});
    REQUIRE(graph.findShortestPath("YYZ", "ORD") == std::vector<std::string> {"YYZ", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("YYZ", "RDU") == std::vector<std::string> {"YYZ", "RDU"});
    REQUIRE(graph.findShortestPath("YYZ", "STL") == std::vector<std::string> {"YYZ", "STL"});
    REQUIRE(graph.findShortestPath("YYZ", "YYZ") == std::vector<std::string> {"YYZ"});
}

TEST_CASE("findShortestPath Directed") {
    FlightGraph graph("routes-test-directed.dat", "airports-test.dat");

    REQUIRE(graph.findShortestPath("CMI", "CMI") == std::vector<std::string> {"CMI"});
    REQUIRE(graph.findShortestPath("CMI", "DFW") == std::vector<std::string> {"CMI", "ORD", "RDU", "IAD", "DFW"});
    REQUIRE(graph.findShortestPath("CMI", "IAD") == std::vector<std::string> {"CMI", "ORD", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("CMI", "IAH") == std::vector<std::string> {"CMI", "ORD", "RDU", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("CMI", "MSP") == std::vector<std::string> {"CMI", "ORD", "RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("CMI", "ORD") == std::vector<std::string> {"CMI", "ORD"});
    REQUIRE(graph.findShortestPath("CMI", "RDU") == std::vector<std::string> {"CMI", "ORD", "RDU"});
    REQUIRE(graph.findShortestPath("CMI", "STL") == std::vector<std::string> {"CMI", "ORD", "RDU", "STL"});
    REQUIRE(graph.findShortestPath("CMI", "YYZ") == std::vector<std::string> {"CMI", "ORD", "RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("DFW", "CMI") == std::vector<std::string> {"DFW", "CMI"});
    REQUIRE(graph.findShortestPath("DFW", "DFW") == std::vector<std::string> {"DFW"});
    REQUIRE(graph.findShortestPath("DFW", "IAD") == std::vector<std::string> {"DFW", "IAD"});
    REQUIRE(graph.findShortestPath("DFW", "IAH") == std::vector<std::string> {"DFW", "IAH"});
    REQUIRE(graph.findShortestPath("DFW", "MSP") == std::vector<std::string> {"DFW", "IAH", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("DFW", "ORD") == std::vector<std::string> {"DFW", "ORD"});
    REQUIRE(graph.findShortestPath("DFW", "RDU") == std::vector<std::string> {"DFW", "IAD", "RDU"});
    REQUIRE(graph.findShortestPath("DFW", "STL") == std::vector<std::string> {"DFW", "IAH", "STL"});
    REQUIRE(graph.findShortestPath("DFW", "YYZ") == std::vector<std::string> {"DFW", "IAH", "STL", "YYZ"});

    REQUIRE(graph.findShortestPath("IAD", "CMI") == std::vector<std::string> {"IAD", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("IAD", "DFW") == std::vector<std::string> {"IAD", "DFW"});
    REQUIRE(graph.findShortestPath("IAD", "IAD") == std::vector<std::string> {"IAD"});
    REQUIRE(graph.findShortestPath("IAD", "IAH") == std::vector<std::string> {"IAD", "IAH"});
    REQUIRE(graph.findShortestPath("IAD", "MSP") == std::vector<std::string> {"IAD", "RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("IAD", "ORD") == std::vector<std::string> {"IAD", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("IAD", "RDU") == std::vector<std::string> {"IAD", "RDU"});
    REQUIRE(graph.findShortestPath("IAD", "STL") == std::vector<std::string> {"IAD", "RDU", "STL"});
    REQUIRE(graph.findShortestPath("IAD", "YYZ") == std::vector<std::string> {"IAD", "RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("IAH", "CMI") == std::vector<std::string> {"IAH", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("IAH", "DFW") == std::vector<std::string> {"IAH", "DFW"});
    REQUIRE(graph.findShortestPath("IAH", "IAD") == std::vector<std::string> {"IAH", "IAD"});
    REQUIRE(graph.findShortestPath("IAH", "IAH") == std::vector<std::string> {"IAH"});
    REQUIRE(graph.findShortestPath("IAH", "MSP") == std::vector<std::string> {"IAH", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("IAH", "ORD") == std::vector<std::string> {"IAH", "DFW", "ORD"});
    REQUIRE(graph.findShortestPath("IAH", "RDU") == std::vector<std::string> {"IAH", "STL", "RDU"});
    REQUIRE(graph.findShortestPath("IAH", "STL") == std::vector<std::string> {"IAH", "STL"});
    REQUIRE(graph.findShortestPath("IAH", "YYZ") == std::vector<std::string> {"IAH", "STL", "YYZ"});

    REQUIRE(graph.findShortestPath("MSP", "CMI") == std::vector<std::string> {"MSP", "STL", "IAH", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("MSP", "DFW") == std::vector<std::string> {"MSP", "STL", "IAH", "DFW"});
    REQUIRE(graph.findShortestPath("MSP", "IAD") == std::vector<std::string> {"MSP", "STL", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("MSP", "IAH") == std::vector<std::string> {"MSP", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("MSP", "MSP") == std::vector<std::string> {"MSP"});
    REQUIRE(graph.findShortestPath("MSP", "ORD") == std::vector<std::string> {"MSP", "STL", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("MSP", "RDU") == std::vector<std::string> {"MSP", "STL", "RDU"});
    REQUIRE(graph.findShortestPath("MSP", "STL") == std::vector<std::string> {"MSP", "STL"});
    REQUIRE(graph.findShortestPath("MSP", "YYZ") == std::vector<std::string> {"MSP", "YYZ"});

    REQUIRE(graph.findShortestPath("ORD", "CMI") == std::vector<std::string> {"ORD", "RDU", "IAD", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("ORD", "DFW") == std::vector<std::string> {"ORD", "RDU", "IAD", "DFW"});
    REQUIRE(graph.findShortestPath("ORD", "IAD") == std::vector<std::string> {"ORD", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("ORD", "IAH") == std::vector<std::string> {"ORD", "RDU", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("ORD", "MSP") == std::vector<std::string> {"ORD", "RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("ORD", "ORD") == std::vector<std::string> {"ORD"});
    REQUIRE(graph.findShortestPath("ORD", "RDU") == std::vector<std::string> {"ORD", "RDU"});
    REQUIRE(graph.findShortestPath("ORD", "STL") == std::vector<std::string> {"ORD", "RDU", "STL"});
    REQUIRE(graph.findShortestPath("ORD", "YYZ") == std::vector<std::string> {"ORD", "RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("RDU", "CMI") == std::vector<std::string> {"RDU", "IAD", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("RDU", "DFW") == std::vector<std::string> {"RDU", "IAD", "DFW"});
    REQUIRE(graph.findShortestPath("RDU", "IAD") == std::vector<std::string> {"RDU", "IAD"});
    REQUIRE(graph.findShortestPath("RDU", "IAH") == std::vector<std::string> {"RDU", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("RDU", "MSP") == std::vector<std::string> {"RDU", "STL", "MSP"});
    REQUIRE(graph.findShortestPath("RDU", "ORD") == std::vector<std::string> {"RDU", "ORD"});
    REQUIRE(graph.findShortestPath("RDU", "RDU") == std::vector<std::string> {"RDU"});
    REQUIRE(graph.findShortestPath("RDU", "STL") == std::vector<std::string> {"RDU", "STL"});
    REQUIRE(graph.findShortestPath("RDU", "YYZ") == std::vector<std::string> {"RDU", "YYZ"});

    REQUIRE(graph.findShortestPath("STL", "CMI") == std::vector<std::string> {"STL", "IAH", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("STL", "DFW") == std::vector<std::string> {"STL", "IAH", "DFW"});
    REQUIRE(graph.findShortestPath("STL", "IAD") == std::vector<std::string> {"STL", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("STL", "IAH") == std::vector<std::string> {"STL", "IAH"});
    REQUIRE(graph.findShortestPath("STL", "MSP") == std::vector<std::string> {"STL", "MSP"});
    REQUIRE(graph.findShortestPath("STL", "ORD") == std::vector<std::string> {"STL", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("STL", "RDU") == std::vector<std::string> {"STL", "RDU"});
    REQUIRE(graph.findShortestPath("STL", "STL") == std::vector<std::string> {"STL"});
    REQUIRE(graph.findShortestPath("STL", "YYZ") == std::vector<std::string> {"STL", "YYZ"});

    REQUIRE(graph.findShortestPath("YYZ", "CMI") == std::vector<std::string> {"YYZ", "STL", "IAH", "DFW", "CMI"});
    REQUIRE(graph.findShortestPath("YYZ", "DFW") == std::vector<std::string> {"YYZ", "STL", "IAH", "DFW"});
    REQUIRE(graph.findShortestPath("YYZ", "IAD") == std::vector<std::string> {"YYZ", "RDU", "IAD"});
    REQUIRE(graph.findShortestPath("YYZ", "IAH") == std::vector<std::string> {"YYZ", "STL", "IAH"});
    REQUIRE(graph.findShortestPath("YYZ", "MSP") == std::vector<std::string> {"YYZ", "MSP"});
    REQUIRE(graph.findShortestPath("YYZ", "ORD") == std::vector<std::string> {"YYZ", "RDU", "ORD"});
    REQUIRE(graph.findShortestPath("YYZ", "RDU") == std::vector<std::string> {"YYZ", "RDU"});
    REQUIRE(graph.findShortestPath("YYZ", "STL") == std::vector<std::string> {"YYZ", "STL"});
    REQUIRE(graph.findShortestPath("YYZ", "YYZ") == std::vector<std::string> {"YYZ"});
}

TEST_CASE("findShortestLandmarkPath Undirected") { //Since findShortestLandmarkPath heavily relies on findShortestPath (which has its own tests) and since there are too many possibilities to test individually, this test only tests a few possibilities
    FlightGraph graph("routes-test-undirected.dat", "airports-test.dat");

    //Easy path (3 airports)
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "ORD", "DFW"}) == std::vector<std::string> {"CMI", "ORD", "DFW"});

    //Easy path (4 airports)
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"DFW", "STL", "MSP"}) == std::vector<std::string> {"DFW", "IAH", "STL", "MSP"});

    //Origin and destination are the same
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "ORD", "CMI"}) == std::vector<std::string> {"CMI", "ORD", "CMI"});

    //Origin, landmark, and destination are the same
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "CMI", "CMI"}) == std::vector<std::string> {"CMI"});

    //Origin and landmark are the same
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "CMI", "ORD"}) == std::vector<std::string> {"CMI", "ORD"});

    //Landmark and destination are the same
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"ORD", "CMI", "CMI"}) == std::vector<std::string> {"ORD", "CMI"});
    
    //Long path with landmark on the way
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "IAD", "MSP"}) == std::vector<std::string> {"CMI", "ORD", "RDU", "IAD", "RDU", "STL", "MSP"});

    //Out-of-the-way landmark that revisits initial airport
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"ORD", "MSP", "CMI"}) == std::vector<std::string> {"ORD", "RDU", "STL", "MSP", "STL", "RDU", "ORD", "CMI"});
}

TEST_CASE("findShortestLandmarkPath Directed") {
    FlightGraph graph("routes-test-directed.dat", "airports-test.dat");

    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "ORD", "DFW"}) == std::vector<std::string> {"CMI", "ORD", "RDU", "IAD", "DFW"});

    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"DFW", "STL", "MSP"}) == std::vector<std::string> {"DFW", "IAH", "STL", "MSP"});

    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "ORD", "CMI"}) == std::vector<std::string> {"CMI", "ORD", "RDU", "IAD", "DFW", "CMI"});

    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "CMI", "CMI"}) == std::vector<std::string> {"CMI"});

    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "CMI", "ORD"}) == std::vector<std::string> {"CMI", "ORD"});

    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"ORD", "CMI", "CMI"}) == std::vector<std::string> {"ORD", "RDU", "IAD", "DFW", "CMI"});
    
    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"CMI", "IAD", "MSP"}) == std::vector<std::string> {"CMI", "ORD", "RDU", "IAD", "RDU", "STL", "MSP"});

    REQUIRE(graph.findShortestLandmarkPath(std::vector<std::string> {"ORD", "MSP", "CMI"}) == std::vector<std::string> {"ORD", "RDU", "STL", "MSP", "STL", "IAH", "DFW", "CMI"});
}

/*
Coordinates come from airports-extended.dat
Note that the Earth is not a perfect sphere so the tests use a relative error tolerance
*/
TEST_CASE("convertCoordinatesToKilometers") {
    double relativeErrorTolerance = 0.01;
    double absoluteErrorTolerance = 0.0001; //Absolute error used to test numerical zeros

    FlightGraph graph("routes-test-undirected.dat", "airports-test.dat");

    //Extreme data values
    std::pair<double, double> nullIsland(0.0, 0.0);
    std::pair<double, double> nullIslandAntipodeEast(0.0, 180.0);
    std::pair<double, double> nullIslandAntipodeWest(0.0, -180.0);
    std::pair<double, double> northPole(90.0, 0.0);
    std::pair<double, double> northPoleAntimeridianEast(90.0, 180.0);
    std::pair<double, double> northPoleAntimeridianWest(90.0, -180.0);
    std::pair<double, double> southPole(-90.0, 0.0);
    std::pair<double, double> southPoleAntimeridianEast(-90.0, 180.0);
    std::pair<double, double> southPoleAntimeridianWest(-90.0, -180.0);
    
    //Quadrant 1
    std::pair<double, double> HND(35.552299, 139.779999);
    std::pair<double, double> ICN(37.46910095214844, 126.45099639892578);

    //Quadrant 2
    std::pair<double, double> CGK(-6.1255698204, 106.65599823);
    std::pair<double, double> SYD(-33.94609832763672, 151.177001953125);

    //Quadrant 3
    std::pair<double, double> GRU(-23.435556411743164, -46.47305679321289);
    std::pair<double, double> LIM(-12.0219, -77.114305);    

    //Quadrant 4
    std::pair<double, double> JFK(40.63980103, -73.77890015);
    std::pair<double, double> ORD(41.9786, -87.9048);

    //Simple test and its reverse
    REQUIRE(graph.convertCoordinatesToKilometers(ORD, JFK) == Approx(1187).epsilon(relativeErrorTolerance));
    REQUIRE(graph.convertCoordinatesToKilometers(JFK, ORD) == Approx(1187).epsilon(relativeErrorTolerance));

    //Null island to its antipodes (and the reverse)
    REQUIRE(graph.convertCoordinatesToKilometers(nullIsland, nullIslandAntipodeEast) == Approx(20002).epsilon(relativeErrorTolerance));
    REQUIRE(graph.convertCoordinatesToKilometers(nullIslandAntipodeEast, nullIsland) == Approx(20002).epsilon(relativeErrorTolerance));
    REQUIRE(graph.convertCoordinatesToKilometers(nullIsland, nullIslandAntipodeWest) == Approx(20002).epsilon(relativeErrorTolerance));

    //Simple test to itself
    REQUIRE(graph.convertCoordinatesToKilometers(ORD, ORD) == Approx(0).margin(absoluteErrorTolerance));

    //Null island's antipodes to its other antipodes
    REQUIRE(graph.convertCoordinatesToKilometers(nullIslandAntipodeEast, nullIslandAntipodeWest) == Approx(0).margin(absoluteErrorTolerance));

    //North pole to south pole
    REQUIRE(graph.convertCoordinatesToKilometers(northPole, southPole) == Approx(20002).epsilon(relativeErrorTolerance));
    REQUIRE(graph.convertCoordinatesToKilometers(northPoleAntimeridianEast, southPoleAntimeridianWest) == Approx(20002).epsilon(relativeErrorTolerance));
    REQUIRE(graph.convertCoordinatesToKilometers(northPoleAntimeridianWest, southPoleAntimeridianEast) == Approx(20002).epsilon(relativeErrorTolerance));

    //North pole to null island's antipodes
    REQUIRE(graph.convertCoordinatesToKilometers(northPoleAntimeridianEast, nullIslandAntipodeWest) == Approx(10001).epsilon(relativeErrorTolerance));
    REQUIRE(graph.convertCoordinatesToKilometers(northPoleAntimeridianWest, nullIslandAntipodeEast) == Approx(10001).epsilon(relativeErrorTolerance));

    //Quadrant 1 to quadrant 1
    REQUIRE(graph.convertCoordinatesToKilometers(HND, ICN) == Approx(1208).epsilon(relativeErrorTolerance));

    //Quadrant 1 to quadrant 2
    REQUIRE(graph.convertCoordinatesToKilometers(ICN, CGK) == Approx(5261).epsilon(relativeErrorTolerance));

    //Quadrant 1 to quadrant 3
    REQUIRE(graph.convertCoordinatesToKilometers(HND, GRU) == Approx(18527).epsilon(relativeErrorTolerance));

    //Quadrant 1 to quadrant 4
    REQUIRE(graph.convertCoordinatesToKilometers(ICN, JFK) == Approx(11081).epsilon(relativeErrorTolerance));

    //Quadrant 2 to quadrant 2
    REQUIRE(graph.convertCoordinatesToKilometers(CGK, SYD) == Approx(5515).epsilon(relativeErrorTolerance));

    //Quadrant 2 to quadrant 3
    REQUIRE(graph.convertCoordinatesToKilometers(SYD, LIM) == Approx(12785).epsilon(relativeErrorTolerance));

    //Quadrant 2 to quadrant 4
    REQUIRE(graph.convertCoordinatesToKilometers(CGK, ORD) == Approx(15766).epsilon(relativeErrorTolerance));

    //Quadrant 3 to quadrant 3
    REQUIRE(graph.convertCoordinatesToKilometers(GRU, LIM) == Approx(3472).epsilon(relativeErrorTolerance));

    //Quadrant 3 to quadrant 4
    REQUIRE(graph.convertCoordinatesToKilometers(LIM, JFK) == Approx(5862).epsilon(relativeErrorTolerance));

    //Quadrant 4 to quadrant 4
    REQUIRE(graph.convertCoordinatesToKilometers(JFK, ORD) == Approx(1187).epsilon(relativeErrorTolerance));
}
