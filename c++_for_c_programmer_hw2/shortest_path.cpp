#include "pch.h" //make visual studio happy
#include "shortest_path.h"
#include <iostream>
#include <queue>
#include <climits>

using namespace std;

//return the shortest path between two nodes of a graph, using Dijkstra's Algorithm 
int shortest_path::shortest_path_nodes(graph& graph, int StartNodeIdx, int TrgtNodeIdx) {

	//Initilize Distance with values of INF and ClosedSet with values of false, Vector size will be same as number of graph nodes
	for (int i = 0; i < graph.GetNumberOfNodes(); i++) {
		Dist.push_back(INF);
		ClosedSet.push_back(false);
	}

	//Create priority queue that stores a pair of node index and distance(path cost) to node, with the rule of min-Heap based on the second element (distance) of pair
	//The method used priority_queue class provided by library queue from STL 
	class QueueRule { public: bool operator()(pair<int, int>&p1, pair<int, int>&p2) { return p1.second > p2.second; } };
	priority_queue<pair<int, int>, vector<pair <int,int>>, QueueRule> PriorityQueue;

	//Push the starting node index and distance (measured zero to self) pair into the priority queue
	PriorityQueue.push(make_pair(StartNodeIdx, Dist[StartNodeIdx]=0));

	//Main Loop - when element still exists in priority queue
	while (!PriorityQueue.empty()) {
		pair<int, int> CurrentNodePair = PriorityQueue.top(); // Pull Current Node and Shortest Distance from Top of the queue, which natually is the current shortest path pair

		//Obtain node index and distance
		int CurrentNodeIndex = CurrentNodePair.first;
		int CurrentNodeDist = CurrentNodePair.second;

		// Has found the shortest path between start and target node, return the value and exit function call
		if (CurrentNodeIndex == TrgtNodeIdx) { return CurrentNodeDist;}

		PriorityQueue.pop(); //Remove the Top Element

		ClosedSet[CurrentNodeIndex] = true; //Add the node to closed set

		node* CNode = graph.GetNodeByIdx(CurrentNodeIndex); //Obtain node in the form of pointer from graph
		vector <int> CurrentNodeNeigbors = CNode->Neighbors(); //Return all neighbor(connected) of current node

		//Loop through neighbors of current node
		for (unsigned int i = 0; i < CurrentNodeNeigbors.size(); ++i) {
				
			//If the neighbor node has not been visited
			if (!ClosedSet[CurrentNodeNeigbors[i]]) {

				//Access the neigobor node element from graph
				node* TNode = graph.GetNodeByIdx(CurrentNodeNeigbors[i]);

				//Get the extra cost from current node to target neighbor node
				int ExtraCost = CNode->GetEdgeCost(TNode);

				//If the current node to start node cost plus the extra cost is less than the distance of the visited node to start node
				if ((CurrentNodeDist + ExtraCost) < Dist[CurrentNodeNeigbors[i]]) {
					PriorityQueue.push(make_pair(CurrentNodeNeigbors[i], Dist[CurrentNodeNeigbors[i]] = (CurrentNodeDist + ExtraCost)));
				}
			}
		}
		 
	}
	//No shortest path was found from start and target node, return INF as indicated by Dist[TrgtNode]
	return Dist[TrgtNodeIdx];

}

//Return the shortest path of a graph, taking average of shortest path of [1 2] [1 3]...[1 50]...
double shortest_path::avg_shortest_path(graph& graph) {

	double AvgCost = 0;

	int GraphNodeSize = graph.GetNumberOfNodes();

	int AvgPaths = GraphNodeSize - 1;

	int tmp;

	//Find the sum of all cost with starting Node 0 and targeting Node iterating from 1 to Size -1
	//If no shortest path was found, eliminate this iteration by reducing the denominator
	for (int i = 1; i < GraphNodeSize; i++) {

		if ((tmp = shortest_path_nodes(graph, 0, i)) < INF) {
			AvgCost += tmp;
		}
		else {
			AvgPaths--;
		}
	}

	//Protection of "all failed to connect"
	if (AvgPaths > 0) { 
		return (AvgCost / static_cast <double> (AvgPaths));
	}
	else {
		return static_cast <double> (INF);
	}
}
