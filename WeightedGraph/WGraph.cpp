#include "WGraph.hpp"

#include <stdexcept>
#include <stack>
#include <string>
#include <set>
#include <map>

typedef std::pair<int, int> IntPair;

//constructor
WGraph::WGraph()
{
	numNodes = 0;
	numEdges = 0;
	nodeList = std::vector<Node*>(SIZE);
	edgeMatrix.fill({});
	edgeList = std::vector<Edge*>(SIZE * 3);
	adjacentList = new std::list<std::pair<int, int>>[SIZE];
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
	/*delete[] nodeList;
	delete[] edgeList;*/
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
	startEnd->startIndex = startIndex;
	startEnd->next = nodeList[startIndex]->connections.front();
	nodeList[startIndex]->connections.front() = startEnd;
	startEnd->weight = weight;

	Edge* endStart = new Edge;
	endStart->endIndex = startIndex;
	endStart->startIndex = endIndex;
	endStart->next = nodeList[endIndex]->connections.front();
	nodeList[endIndex]->connections.front() = endStart;
	endStart->weight = weight;

	//creating an adjacency list for each node using the indexes of start and end of edges
	IntPair ip = std::make_pair(endIndex, weight);
	nodeList[startIndex]->adjacents.push_back(ip);
	ip = std::make_pair(startIndex, weight);
	nodeList[endIndex]->adjacents.push_back(ip);

	adjacentList[startIndex].push_back(std::make_pair(endIndex, weight));
	adjacentList[endIndex].push_back(std::make_pair(startIndex, weight));

	//creating edgelist for mincost
	edgeList[numEdges++] = startEnd;
	edgeList[numEdges++] = endStart;
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
				string sWeight = std::to_string(weight);
				buffer += sWeight;
				if (sWeight.length() == 1)
				{
					buffer += " ";
				}
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

	while (!que.empty())
	{
		ptr = que.front();
		buffer += ptr->name; //output the node name
		buffer += " ";
		que.pop();

		std::list<IntPair> adj = ptr->adjacents;
		//sets the correct direction of adjacents for the driver
		adj.reverse();
		for (auto i : adj)
		{
			if (!nodeList[i.first]->visited) //if not visited mark and push on queue
			{
				nodeList[i.first]->visited = true;
				que.push(nodeList[i.first]);
			}
		}
	}
	//print nonvisited nodes
	for (int i = 0; i < numNodes; i++) {
		if (nodeList[i]->visited == false) {
			buffer += "Unreached ";
			buffer += nodeList[i]->name;
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
	buffer += " ";
	ptr->visited = true;
	bool check = false; //check for unvisited neighbor nodes

	//using a stack to traverse in order
	std::stack<Node*> stak;
	stak.push(ptr);

	while (!stak.empty())
	{
		if (!check)
		{
			ptr = stak.top();
			stak.pop();

			if (!ptr->visited) //add to buffer if not visited yet
			{
				buffer += ptr->name;
				buffer += " ";
				ptr->visited = true;
			}
		}
		bool check = false;
		//push any unvisited adjacent nodes
		for (auto i : ptr->adjacents)
		{
			if (!nodeList[i.first]->visited)
			{
				stak.push(nodeList[i.first]);
				check = true;
			}
		}
	}

	for (int i = 0; i < numNodes; i++) {
		if (nodeList[i]->visited == false) {
			buffer += "Unreached ";
			buffer += nodeList[i]->name;
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
//minimum cost(spanning) tree
string WGraph::minCostTree(char name)
{
	resetVisited();

	std::priority_queue <IntPair, std::vector<IntPair>, std::greater<IntPair>> pQueue;

	string buffer = "";
	buffer += name;
	buffer += ": ";

	int nodeIndex = findNode(name);

	std::vector<int> parents(numNodes, -1);
	std::vector<int> nodeKeys(numNodes, INT_MAX);
	std::vector<bool> inMST(numNodes, false);

	nodeKeys[nodeIndex] = 0;

	pQueue.push(std::make_pair(0, nodeIndex));

	while (!pQueue.empty())
	{
		IntPair currentNodePair = pQueue.top();
		Node* node = nodeList[currentNodePair.second];
		nodeIndex = findNode(node->name);
		pQueue.pop();

		if (inMST[nodeIndex] == true) {
			continue;
		}
		inMST[nodeIndex] = true;

		for (std::pair<int, int> i : adjacentList[nodeIndex]) {
			int weight = i.second;
			int nextIndex = i.first;

			if (!inMST[nextIndex] && nodeKeys[nextIndex] > weight) {
				nodeKeys[nextIndex] = weight;
				pQueue.push(std::make_pair(nodeKeys[nextIndex], nextIndex));
				parents[nextIndex] = nodeIndex;

				// Append the edge to the buffer
				buffer += nodeList[nodeIndex]->name;
				buffer += '-';
				buffer += nodeList[nextIndex]->name;
				buffer += ' ';
			}
		}
	}

	return buffer;
}

string WGraph::minCostPaths(char name)
{
	resetVisited();
	string buffer = "";
	buffer += name;
	buffer += ": ";
	std::set<bool> spSet;
	for (int i = 0; i < numNodes - 1; i++)
	{
		spSet.insert(false);
	}
	std::set<int> distance;
	for (int i = 0; i < numNodes - 1; i++)
	{
		distance.insert(INT_MAX);
	}
	return buffer;
}

