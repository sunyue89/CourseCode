#pragma once
#include <vector>
#include "graph.h"

class graph;

//This class takes in the graph class and compute the shortest path between two given nodes, and graph average shortest path with starting node 0
class shortest_path {

public:
	//constructor
	shortest_path() {};
	//destructor
	~shortest_path() {};

	//return the shortest path between two nodes of a graph, using Dijkstra's Algorithm 
	int shortest_path_nodes(graph& graph, int StartNodeIdx, int TrgtNodeIdx);

	//return the average shortest path of graph
	double avg_shortest_path(graph& graph);

private:

	//Store the distance of from starting to all nodes, use vector to dynamically adjust to graph's size 
	vector <int> Dist;

	//The closed set of visited nodes, use vector to dynamically adjust to graph's size 
	vector <bool> ClosedSet;
};