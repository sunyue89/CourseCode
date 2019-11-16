#pragma once
#include <vector>
#include <iterator>
#include <algorithm>
#include <climits>
#include <fstream>


using namespace std;

const int INF = INT_MAX;

//struct node is an element of class graph
struct node;

//struct edge is an element of class gragh
struct edge {

public:
	//constructor
	edge(node* TrgtNode, int EdgeCost) : TrgtNode(TrgtNode), EdgeCost(EdgeCost) {}

	//no need for destructor - elements will be deleted through destructor of graph

	//Return the target node from the edge
	inline struct node* GetTrgtNode() { return TrgtNode; }

	//Return the edge cost
	inline int GetEdgeCost() { return EdgeCost; }

	//Return the target node index, detailed function in cpp source file due to compiling difficulty if declared here
	inline int GetTrgtNodeIdx();

	//operator overloading - if two target nodes are equivalent
	inline friend bool operator==(const edge& Edge1, const edge& Edge2) {
		return (Edge1.TrgtNode == Edge2.TrgtNode) ;
	};

private:
	//private element of each edge class - target node and edge cost
	node *TrgtNode;
	int EdgeCost;

};

//struct node is an element of class graph
struct node {

public:
	//constructor
	node(const int index) : idx(index) {}

	//no need for destructor - elements will be deleted through destructor of graph

	//return index of current node
	inline int GetNodeIdx() { return idx; }

	//return adjecent edges size per node
	inline int GetNumberOfEdges() { return AdjacentEdges.size(); }

	//Find if an edge exists between current node and target node
	//If yes return true, else return false
	//details in cpp source file
	bool IsAdjacent(node* TrgtNode, int* EdgeIdx);

	//Add an edge (from the current node) to the target node with specified cost, if none existed
	//details in cpp source file
	bool AddEdge(node* TrgtNode, const int EdgeCost);

	//Delete an edge (from the starting node) to the target node if already existed
	//details in cpp source file
	bool DeleteEdge(node* TrgtNode);

	//Find all connected neighbor nodes to the starting node - A key function call in shortest_path
	vector <int> Neighbors();

	//Return the edge cost to the target node if in the adjacent neighbors
	int GetEdgeCost(node* TrgtNode);

private:
	//Index
	int idx;
	//All adjacent edges of the current node
	vector<edge> AdjacentEdges;
};


//graph is represented in an equivalent AjacencyList Method
//where each node is represented by a node struct pointer
//and each edge is constructed by the target node pointer and cost, and always being referenced by the starting node
//this creates an efficient way to compose and search through the graph 
class graph {

public:
	//constructor - details in source file
	graph(int graphid, int nNodes, double den, int dMin, int dMax);

	//homework 3 another consturctor - construct graph from a specific data format
	graph(int graphid, vector<int> &data);

	//destructor - details in source file
	~graph();

	//return total nodes per graph
	inline int GetNumberOfNodes() { return nNodes; }

	//return a specific node by index
	inline node* GetNodeByIdx(const int idx) { return Nodes[idx]; }
	
	//return total edges per graph
	int GetNumberOfEdges();

	//print in the form of adjacent matrix, for debugging purposes
	void PrintAdjacencyMatrix();

private:
	//graph id in the form of integer
	int graphid;
	//total number of nodes
	int nNodes;
	//total struct pointer nodes - used to represent the graph connectivity and edge cost
	vector <node*> Nodes;
};



