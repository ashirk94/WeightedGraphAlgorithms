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

class MinHeap
{
private:
	Edge** arr;
	int capacity;
	int count;

public:
	MinHeap(int size = 20);
	~MinHeap();
	void pushItem(Edge* edge);
	Edge* popItem();
	Edge* peekItem();
	void resize();
	bool isEmpty();
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

