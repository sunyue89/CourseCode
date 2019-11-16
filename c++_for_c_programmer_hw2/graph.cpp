#include "pch.h" //make visual studio happy
#include "graph.h"
#include <iostream>
#include <vector>
#include <ctime>

//Find if an edge exists between current node and target node
//If yes return true, else return false
bool node::IsAdjacent(node* TrgtNode, int *EdgeIdx) {
	vector<edge>::iterator EdgeItr = std::find(AdjacentEdges.begin(), AdjacentEdges.end(), edge(TrgtNode, 0));

	if (EdgeItr == AdjacentEdges.end()) {
		return false; //If iterator falls to end, none edge found
	}
	else {
		*EdgeIdx = std::distance(AdjacentEdges.begin(), EdgeItr); //Else return the index of edge
		return true; 
	}
}

//Add an edge (from the current node) to the target node with specified cost, if none existed
bool node::AddEdge(node* TrgtNode, const int EdgeCost) {
	int dummy;
	if (IsAdjacent(TrgtNode,&dummy)) {
		return false; //one already existed, do not add
	}
	else {
		//call constructor of edge
		edge NewEdge(TrgtNode, EdgeCost);
		node::AdjacentEdges.push_back(NewEdge);
		return true;
	}
};

//Delete an edge (from the starting node) to the target node if already existed
bool node::DeleteEdge(node* TrgtNode) {
	int EdgeIdx;
	if (IsAdjacent(TrgtNode, &EdgeIdx)) {
		AdjacentEdges.erase(AdjacentEdges.begin() + EdgeIdx);
		return true;
	}
	else {
		return false;
	}
};

//Find all connected neighbor nodes to the starting node - A key function call in shortest_path
vector <int> node::Neighbors() {
	vector <int> ReturnVector;
	for (vector<edge>::iterator EdgeItr = AdjacentEdges.begin(); EdgeItr != AdjacentEdges.end(); ++EdgeItr) {
		ReturnVector.push_back(EdgeItr->GetTrgtNodeIdx());
	}
	return ReturnVector;
};

//Return the edge cost to the target node if in the adjacent neighbors
int node::GetEdgeCost(node* TrgtNode) {
	int EdgeIdx;
	if (!IsAdjacent(TrgtNode, &EdgeIdx)) {
		return INF; //Not Adjacent
	}
	else {
		return (AdjacentEdges.begin() + EdgeIdx)->GetEdgeCost();
	}

};

//Return the target node index
inline int edge::GetTrgtNodeIdx() { return TrgtNode->GetNodeIdx(); }


//graph constructor, inputs are unsigned integor ID, total number of nodes, density, min and max cost range
//graph is represented in an equivalent AjacencyList Method
//where each node is represented by a node struct pointer
//and each edge is constructed by the target node pointer and cost, and always being referenced by the starting node
graph::graph(int graphid, int nNodes, double den, int dMin, int dMax) {

	if (nNodes <= 0) {
		return;
	}

	this->graphid = graphid;
	this->nNodes = nNodes;

	Nodes = vector<node*> (nNodes);

	for (int i = 0;i < nNodes;i++) {
		Nodes[i] = new node(i); // construct node based on index, then assgin new memory allocation
	}

	srand(static_cast<unsigned int> (time(0)));

	//Use iterator to compose the graph
	for (vector<node*>::iterator it1 = Nodes.begin(); it1 != Nodes.end(); ++it1) {
		for (vector<node*>::iterator it2 = Nodes.begin(); it2 != Nodes.end(); ++it2) {

			// The same Node, don't add any edges.
			if ((*it1) != (*it2)) {
				// Compares the edge density to a randomly generated double between 0 and 1.
				bool bHasEdge = ((rand() % 100) / 100.0 <= den);
				if (bHasEdge) {
					int EdgeCost = rand() % (((dMax + 1) - dMin)) + dMin; // generate a randome number between the minimum and maximum range

					// Add an undirected edge for both nodes towards oneanother
					// If the Edge already exists, then no new Edge is added. 
					(*it1)->AddEdge((*it2), EdgeCost);
					(*it2)->AddEdge((*it1), EdgeCost);
				}
			}
		}
	}

};

//graph destructor
graph::~graph() {
	
	for (unsigned int i = 0; i < Nodes.size(); ++i) {
		delete Nodes[i]; // delete memory allocation of each node
	}

}

//Return the total number of edges of a graph
int graph::GetNumberOfEdges() {
	int nEdges = 0;
	for (vector<node*>::iterator it1 = Nodes.begin(); it1 != Nodes.end(); ++it1) {
		
		nEdges += (*it1)->GetNumberOfEdges();
	}
	return nEdges;
}

//Print the bool Adjacency Matrix
void graph::PrintAdjacencyMatrix() {

	int nodeNo = 0;
	int dummy;
	for (vector<node*>::iterator it1 = Nodes.begin(); it1 != Nodes.end(); ++it1) {
		std::cout << "node:" << nodeNo << "|";

		for (vector<node*>::iterator it2 = Nodes.begin(); it2 != Nodes.end(); ++it2) {
			std::cout << (*it1)->IsAdjacent(*it2,&dummy);
		}
		nodeNo++;
		std::cout << endl;
	}
}