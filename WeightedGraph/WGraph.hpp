#pragma once
#include <list>
#include <iostream>
#include <array>
#include <queue>

using std::string;

const int SIZE = 20;

struct Edge
{
	int startIndex;
	int endIndex;
	int weight;
	Edge* next;
};

struct Node
{
	char name;
	bool visited;
	std::list<Edge*> connections;
	std::list<Node*> adjacents;
};
class WGraph
{
private:
	int numNodes;
	int numEdges;
	Node** nodeList;
	Edge** edgeList;
	std::array<std::array<int, SIZE>, SIZE> edgeMatrix;
	int findNode(char name);
	void resetVisited();
	std::priority_queue<Edge*> pQueue;
	int pQItems;

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
	Edge* removeSmallest();
	void pQueueAdd(Edge* edge);
	string minCostPaths(char name);
};

