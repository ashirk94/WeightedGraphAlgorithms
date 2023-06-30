#include "WGraph.hpp"

#include <stdexcept>
#include <stack>
#include <string>
#include <set>
#include <map>

typedef std::pair<Node*, int> NodeWeight;
typedef std::pair<Node*, Node*> NodePair;


//constructor
WGraph::WGraph()
{
	numNodes = 0;
	numEdges = 0;
	nodeList = new Node*[SIZE];
	edgeMatrix.fill({});
	edgeList = new Edge*[SIZE * 2];
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
	delete[] edgeList;
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
	NodeWeight nw = std::make_pair(nodeList[endIndex], weight);
	nodeList[startIndex]->adjacents.push_back(nw);
	nw = std::make_pair(nodeList[startIndex], weight);
	nodeList[endIndex]->adjacents.push_back(nw);

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

		std::list<NodeWeight> adj = ptr->adjacents;
		//sets the correct direction of adjacents for the driver
		adj.reverse(); 
		for (auto i : adj)
		{
			if (!i.first->visited) //if not visited mark and push on queue
			{
				i.first->visited = true;
				que.push(i.first);
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
		for (auto i: ptr->adjacents)
		{
			if (!i.first->visited)
			{
				stak.push(i.first);
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

	std::priority_queue <NodeWeight, std::vector<NodeWeight>, std::greater<NodeWeight>> pQueue;

	string buffer = "";
	buffer += name;
	buffer += ": ";

	int items = 0;

	Node* first = nodeList[findNode(name)];

	//add nodes to tree

	std::vector<Node*> parents(numEdges, nullptr);
	std::vector<int> nodeKeys(numEdges,INT_MAX);
	std::vector<bool> inMST(numEdges, false);

	nodeKeys[0] = 0;
	//loop while there are unvisited nodes in adjacents
	
	pQueue.push(std::make_pair(first, 0));

	while (!pQueue.empty())
	{
		items++;


		NodeWeight currentNodePair = pQueue.top();
		pQueue.pop();
		Node* node = currentNodePair.first;

		if (node->visited == true) {
			continue;
		}
		node->visited = true;
		int weight = currentNodePair.second;



		//loop while there are unvisited nodes in adjacents


		for (auto i : node->adjacents) {
			if (!i.first->visited && i.second < nodeKeys[items]) {
				nodeKeys[items] = i.second;
				pQueue.push(std::make_pair(i.first, nodeKeys[items]));
				parents[items] = node;
			}
		}		
	}
	for (int i = 0; i < numEdges; i++) {
		if (parents[i] == nullptr) continue;
		buffer += parents[i]->name;
		buffer += ' ';
	}

	resetVisited();
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

