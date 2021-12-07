#include "WGraph.hpp"

#include <stdexcept>
#include <queue>
#include <stack>
#include <string>

//constructor
WGraph::WGraph()
{
	numNodes = 0;
	nodeList = new Node * [SIZE];
}
//destructor
WGraph::~WGraph()
{
	for (int i = 0; i < numNodes; i++)
	{
		//edges
		if (nodeList[i]->connections.front() != nullptr)
		{
			Edge* ptr = nodeList[i]->connections.front();
			while (ptr->next != nullptr)
			{
				Edge* temp = ptr;
				ptr = ptr->next;
				delete temp;
			}
		}
		Node* node = nodeList[i];
		delete node;
	}
	delete[] nodeList;
}
//add a node
void WGraph::addNode(char name)
{
	if (numNodes >= SIZE)
	{
		throw std::overflow_error("Graph size exceeded");
	}

	//create node
	Node* temp = new Node;
	temp->name = name;
	temp->visited = false;
	temp->connections.push_back(nullptr);
	//add it to the array
	nodeList[numNodes++] = temp;
}
bool WGraph::addEdge(char starts, char ends, int weight)
{
	if (starts == ends) //link to self
	{
		return false;
	}

	int startIndex = findNode(starts);
	int endIndex = findNode(ends);

	//if either end doesn't exist
	if (startIndex == -1 or endIndex == -1)
	{
		return false;
	}

	//set links in edgeMatrix
	edgeMatrix[startIndex][endIndex] = weight;
	edgeMatrix[endIndex][startIndex] = weight;

	//new edges - setting connections according to the way from pseudocode
	Edge* startEnd = new Edge;
	startEnd->endIndex = endIndex;
	startEnd->next = nodeList[startIndex]->connections.front();
	nodeList[startIndex]->connections.front() = startEnd;
	startEnd->weight = weight;

	Edge* endStart = new Edge;
	endStart->endIndex = startIndex;
	endStart->next = nodeList[endIndex]->connections.front();
	nodeList[endIndex]->connections.front() = endStart;
	endStart->weight = weight;

	//creating an adjacency list for each node using the indexes of start and end of edges
	nodeList[startIndex]->adjacents.push_front(nodeList[endIndex]);
	return true;
}
//list nodes in order added
string WGraph::listNodes()
{
	string list = "";
	for (int i = 0; i < numNodes; i++)
	{
		list += nodeList[i]->name;
		list += " ";
	}
	return list;
}
//display edges for each node
string WGraph::displayEdges()
{
	string buffer;
	buffer = "";

	for (int i = 0; i < numNodes; i++)
	{
		//node name
		buffer += nodeList[i]->name;
		buffer += ": ";
		//edges
		Edge* ptr = nodeList[i]->connections.front();
		while (ptr != nullptr)
		{
			buffer += nodeList[ptr->endIndex]->name;
			buffer += " ";
			ptr = ptr->next;
		}
		buffer += "\n";
	}
	return buffer;
}
//displays edge matrix
string WGraph::displayMatrix()
{
	string buffer;
	buffer = "";

	for (int i = 0; i < numNodes; i++)
	{
		for (int j = 0; j < numNodes; j++)
		{
			if (i == j)
			{
				buffer += "- ";
			}
			else
			{
				int weight = edgeMatrix[i][j];
				buffer += std::to_string(weight);
				buffer += " ";
			}
		}
		buffer += "\n";
	}
	return buffer;
}

//traverse graph breadth first
string WGraph::breadthFirst(char name)
{
	resetVisited();
	std::queue<Node*> que;
	string buffer;
	//find the starting node
	Node* ptr = nodeList[findNode(name)];

	if (ptr == nullptr)
	{
		throw std::domain_error("Missing node");
	}
	que.push(ptr);
	ptr->visited = true;
	bool first = true;

	while (!que.empty())
	{
		ptr = que.front();

		buffer += ptr->name; //output the node name
		if (first) //colon for first node
		{
			buffer += ":";
		}
		buffer += " ";
		que.pop();

		first = false;
		//check for connections
		if (!ptr->adjacents.empty())
		{
			Node* node = nullptr;
			for (auto i : ptr->adjacents)
			{
				if (!i->visited) //if not visited mark and push on queue
				{
					i->visited = true;
					que.push(i);
					node = i;
				}
			}
			ptr = node;
		}
	}
	return buffer;
}
//traverse graph depth first
string WGraph::depthFirst(char name)
{
	resetVisited();
	string buffer = "";
	//find the starting node
	Node* ptr = nodeList[findNode(name)];

	if (ptr == nullptr)
	{
		throw std::domain_error("Missing node");
	}

	buffer += ptr->name;
	buffer += ":";
	buffer += " ";
	ptr->visited = true;

	//using a stack to traverse in order
	std::stack<Node*> stak;
	stak.push(ptr);

	while (!stak.empty())
	{
		ptr = stak.top();
		stak.pop();

		if (!ptr->visited) //add to buffer if not visited yet
		{
			buffer += ptr->name;
			buffer += " ";
			ptr->visited = true;
		}

		//push any unvisited adjacent nodes
		for (int i = 0; i < numNodes - 1; i++)
		{
			for (int j = 0; j < numNodes - 1; j++)
			{
				if (!nodeList[i]->visited and edgeMatrix[i][j] != 0)
				{
					stak.push(nodeList[i]);
				}
			}

		}
	}
	return buffer;
}
//linear search for node
int WGraph::findNode(char name)
{
	for (int i = 0; i < numNodes; i++)
	{
		if (nodeList[i]->name == name)
		{
			return i; //returning index
		}
	}
	return -1; //not found
}
//reset visited nodes
void WGraph::resetVisited()
{
	for (int i = 0; i < numNodes; i++)
	{
		if (nodeList[i]->visited == true)
		{
			nodeList[i]->visited = false;
		}
	}
}
