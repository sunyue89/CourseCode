#include "pch.h" //make visual studio happy
#include "process_graph.h"
#include <iostream>
#include <queue>
#include <climits>
#include <ctime>

using namespace std;

//Minimum Spanning Tree using Prim's algorithm via priority queue method
int process_graph::MST(graph& graph, int *pair_init) {

	//Get graph size of total nodes
	int gsize = graph.GetNumberOfNodes();

	//Initilize Dist vector with graph size and value INF
	Dist = vector<int>(gsize,INF);

	//Initilize CloseSet vector with graph size and value false
	ClosedSet = vector<bool>(gsize,false);

	//Initilize Pair vector with graph size and value -1, output this value 
	Pair = vector<int>(gsize,(*pair_init = -1));

	//Create priority queue that stores a pair of node index and distance(path cost) to node, with the rule of min-Heap based on the second element (distance) of pair
	//The method used priority_queue class provided by library queue from STL 
	class QueueRule { public: bool operator()(pair<int, int>&p1, pair<int, int>&p2) { return p1.second > p2.second; } };
	priority_queue<pair<int, int>, vector<pair <int, int>>, QueueRule> PriorityQueue;

	//Randomly choose a start node in [0 : TotalNodes -1]
	srand(static_cast<unsigned int> (time(0)));
	int startnode = rand() % gsize;

	//Push the starting node index and distance (measured zero to self) pair into the priority queue
	PriorityQueue.push(make_pair(startnode, Dist[startnode] = 0));

	//Main loop - continue if priority queue is not empty or not all nodes have been visited
	while (!PriorityQueue.empty() && !AllNodesChecked(ClosedSet)) {
		pair<int, int> CurrentNodePair = PriorityQueue.top(); // Pull Current Node and Shortest Distance from Top of the queue, which natually is the current shortest path pair

		//Obtain node index and weight
		int CurrentNodeIndex = CurrentNodePair.first;
		int CurrentNodeDist = CurrentNodePair.second;

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

				//Get the cost from current node to target neighbor node
				int Cost = CNode->GetEdgeCost(TNode);

				//If the registered neighbor node's minimum spanning cost is found greater than the new cost from current node
				if (Dist[CurrentNodeNeigbors[i]]>Cost) {
					//Update this lower cost to the minimum spanning register
					Dist[CurrentNodeNeigbors[i]] = Cost;
					//Push this pair to priority queue
					PriorityQueue.push(make_pair(CurrentNodeNeigbors[i], Dist[CurrentNodeNeigbors[i]]));
					//Update the neighbor's pair to current node
					Pair[CurrentNodeNeigbors[i]] = CurrentNodeIndex;
				}
			}
		}
	}
	//All nodes have been accessed, or priority queue is empty, calculate the MST weight
	int TotalCost = 0;
	for (int i = 0; i < gsize; ++i) {
		TotalCost += Dist[i];
	}
	return TotalCost;
};

//Legacy homework 2 function - return the shortest path between two nodes of a graph, using Dijkstra's Algorithm 
int process_graph::shortest_path_nodes(graph& graph, int StartNodeIdx, int TrgtNodeIdx) {

	//Initilize Distance with values of INF and ClosedSet with values of false, Vector size will be same as number of graph nodes
	int gsize = graph.GetNumberOfNodes();
	Dist = vector<int>(gsize, INF);
	ClosedSet = vector<bool>(gsize, false);

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

//Legacy homework 2 function - Return the shortest path of a graph, taking average of shortest path of [1 2] [1 3]...[1 50]...
double process_graph::avg_shortest_path(graph& graph) {

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
