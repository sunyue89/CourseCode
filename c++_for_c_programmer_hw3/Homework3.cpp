// Homework3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h" //make visual studio happy
#include "graph.h"
#include "process_graph.h"
#include <iostream>

int main()
{
	//Load data downloaded from website and stored in Graph.txt
	ifstream datafile("Graph.txt");

	istream_iterator<int> start(datafile), end;
	vector<int> data(start, end);

	//Construct graph based on data
	graph Graph(0, data);

	std::cout << "Graph has total nodes of "<<Graph.GetNumberOfNodes() << endl;
	std::cout << "Graph has total edges of "<<Graph.GetNumberOfEdges() << endl;
	std::cout << "Graph's adjacency matrix representation is given by:" << endl;
	Graph.PrintAdjacencyMatrix();
	std::cout << endl;

	//Construct a graph process class
	process_graph MinSpanTree;

	//Initilize a register to return the value of pair initiliazation
	int Pair_Init;

	//Call Minimum Spanning Tree 
	int MST_Value = MinSpanTree.MST(Graph, &Pair_Init);

	//Return the Cost and Pair from MST
	vector <int> EdgeCost;
	vector <int> IdxPair;
	EdgeCost = MinSpanTree.getcost();
	IdxPair = MinSpanTree.getpair();

	//Print MST edge cost
	std::cout << "Graph's minimum spanning tree weight is " << MST_Value << endl;

	//Print MST Edge Pairs and Cost
	std::cout << "Detailed Tree: " << endl;
	for (int i = 0; i< Graph.GetNumberOfNodes(); ++i) {
		//Skip the initializer index by comparing with the pair initilizer value
		if (IdxPair[i] != Pair_Init) {
			std::cout <<"("<< i << " - " << IdxPair[i] << ") ";
			std::cout << "with cost "<< EdgeCost[i] << endl;
		}
	}
	std::cout << endl;
	std::cout << "Run the program again to start from a (different) random generated start node " << endl;
	std::cin.ignore();
	return 0;

}

