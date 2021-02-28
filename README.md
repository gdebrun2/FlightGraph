# alyssal3-albertv2-gdebrun2
OpenFlights Airport Database 

This code-base implements a graph to analyze the OpenFlights Airline Route Mapper Route Database. Breadth-first search traversal, Dijkstra’s shortest path algorithm, and a landmark path algorithm are implemented to compute and output the geographically shortest route between airports, along with the shortest route that includes the user's desired landmark location. 

Note: this project was designed to be run on EWS. The project may not work as intended on other systems. 

Build/usage instructions: 
1. Download the GitHub repository and extract the zip file into your local directory. 
2. Sample inputs have already been inserted into the ``options.txt`` file. Modify this file with your desired inputs: 
    - The first line is the route data input file. 
    - The second line is the airport data input file. 
    - The third line is the origin airport. 
    - The fourth line is the landmark airport. 
    - The fifth line is the destination airport.
    
    **Make sure to save this file before exiting the window!**
3. After navigating to the working directory, run the command ``make`` in your terminal (this may take approximately 30 seconds). 
4. To run the main executable: ``./main`` (this may take approximately 30 seconds). 
5. To run the test executable: ``./test``

A bread-first search traversal will be executed starting from the user's chosen origin airport. The geographically shortest path between the origin airport and the destination airport will be displayed in the following line, and the geographically shortest path from the origin airport to the destination airport through the landmark airport will be displayed in the final line of the output. 
