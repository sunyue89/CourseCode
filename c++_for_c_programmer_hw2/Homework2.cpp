// Homework2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h" //make visual studio happy
#include "graph.h"
#include "shortest_path.h"
#include <iostream>

int main()
{
	int NumNodes = 50;
	int MinRange = 1;
	int MaxRange = 10;
	double Density = 0.2;
	int SimuIter = 20;
	double AvgStDisGraph = 0;

	//First Monte Carlo simulation, 20 sets of graph with density of 0.2
	for (int i = 0; i < SimuIter; i++) {

		graph Graph(i, NumNodes, Density, MinRange, MaxRange);
		shortest_path Path;
		AvgStDisGraph += Path.avg_shortest_path(Graph);
	}
	AvgStDisGraph = AvgStDisGraph / static_cast <double> (SimuIter);
	std::cout << SimuIter << " Graph Simulations with Desity " << Density << " And Range of [ " << MinRange <<" "<< MaxRange << " ] returns average shortest distance of " << AvgStDisGraph << endl;

	std::cout << "Press any key to run second simulation..." << endl;
	std::cin.ignore();

	//Second Monte Carlo simulation, 20 sets of graph with density of 0.4
	Density = 0.4;
	AvgStDisGraph = 0;

	for (int i = 0; i < SimuIter; i++) {

		graph Graph(i, NumNodes, Density, MinRange, MaxRange);
		shortest_path Path;
		AvgStDisGraph += Path.avg_shortest_path(Graph);
	}
	AvgStDisGraph = AvgStDisGraph / static_cast <double> (SimuIter);
	std::cout << SimuIter << " Graph Simulations with Desity " << Density << " And Range of [ " << MinRange << " " << MaxRange << " ] returns average shortest distance of " << AvgStDisGraph << endl;
	std::cin.ignore();

	return 0;

}

