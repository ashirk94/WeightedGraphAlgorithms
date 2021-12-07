#pragma once
#include <list>
#include <iostream>
#include <array>

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
	Node* nextNode;
	Node* prevNode;
};
class WGraph
{
private:
	int numNodes;
	Node** nodeList;
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
	string depthRec(Node* node, string buffer);
};

