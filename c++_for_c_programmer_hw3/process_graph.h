#pragma once
#include <vector>
#include "graph.h"

class graph;

//This class takes in the graph class and process the graph per need
//For Homework 3 it computes the graph's minimum spanning tree using Prim's algorithm - didn't have time to try Krukal's algorithms but maybe after due
//For Homework 2 it computes the shortest path between two given nodes, and graph average shortest path with starting node 0
class process_graph {

public:
	//constructor
	process_graph() {};
	//destructor
	~process_graph() {};

	//calculate and return graph minimum spanning tree weight
	int MST(graph& graph, int *pair_init);

	//access the minimum spanning edge cost per index node [0 : TotalNodes-1] stored in Dist
	inline vector<int> getcost() { return Dist; }

	//access the paired node per index node [0 : TotalNodes -1] stored in Pair
	inline vector<int> getpair() { return Pair; }

	//check if all nodes have been placed in the closed set, if yes return true, else return false
	inline bool AllNodesChecked(vector<bool>& Set) {
		bool tmp = true;
		for (int i = 0; i < Set.size(); ++i) {
			tmp = tmp && Set[i];
		}
		return tmp;
	}

	//Legacy homework 2 functions, keep in class
	//return the shortest path between two nodes of a graph, using Dijkstra's Algorithm 
	int shortest_path_nodes(graph& graph, int StartNodeIdx, int TrgtNodeIdx);

	//Legacy homework 2 functions, keep in class
	//return the average shortest path of graph
	double avg_shortest_path(graph& graph);

private:

	//Homework 3: Store minimum spanning edge cost per index node 
	//Homework 2: Store the distance of from starting to all nodes, use vector to dynamically adjust to graph's size 
	vector <int> Dist;

	//Homework 3: Same function as Homework 2
	//Homework 2: The closed set of visited nodes, use vector to dynamically adjust to graph's size 
	vector <bool> ClosedSet;

	//Homework 3: Store paired node per index node
	vector<int> Pair;

};