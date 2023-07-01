#pragma once
#include <list>
#include <iostream>
#include <array>
#include <queue>


using std::string;

const int SIZE = 10;

struct Edge
{
	int startIndex;
	int endIndex;
	int weight;
	Edge* next;
};

struct Node
{
	char name = '.';
	bool visited = false;
	std::list<Edge*> connections;
	std::list<std::pair<int, int>> adjacents;
};

class WGraph
{
private:
	int numNodes;
	int numEdges;
	std::vector<Node*> nodeList;
	std::vector<Edge*> edgeList;
	std::array<std::array<int, SIZE>, SIZE> edgeMatrix;
	int findNode(char name);
	void resetVisited();
	std::list<std::pair<int, int>> *adjacentList;

public:
	WGraph();
	~WGraph();
	void addNode(char name);
	bool addEdge(char starts, char ends, int weight);
	string listNodes();
	string displayEdges();
	string displayMatrix();
	string breadthFirst(char name);
	string depthFirst(char name);
	string minCostTree(char name);
	string minCostPaths(char name);
};

