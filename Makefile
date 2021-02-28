make: FlightGraph.cpp catchmain.cpp main.cpp tests.cpp
	clang++ FlightGraph.cpp catchmain.cpp tests.cpp -o test
	clang++ FlightGraph.cpp main.cpp -o main
