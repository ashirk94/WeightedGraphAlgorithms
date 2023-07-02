#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <climits>
#include <string>
#include <set>
#include <stack>

using namespace std;
typedef std::pair<int, int> intPair;

class WGraph {
private:
    struct Edge {
        char source;
        char destination;
        int weight;

        Edge(char src, char dest, int w) : source(src), destination(dest), weight(w) {}
    };

    struct CompareEdges {
        bool operator()(const Edge& e1, const Edge& e2) {
            if (e1.weight == e2.weight) {
                // If weights are equal, compare based on the order of vertices
                if (e1.source == e2.source)
                    return e1.destination > e2.destination;
                return e1.source > e2.source;
            }
            return e1.weight > e2.weight;
        }
    };


    std::map<char, std::vector<Edge>> adjacencyList;
    std::vector<char> nodeList;

public:
    void addNode(char node) {
        adjacencyList[node] = std::vector<Edge>();
        nodeList.push_back(node);
    }

    void addEdge(char src, char dest, int weight) {
        adjacencyList[src].push_back(Edge(src, dest, weight));
        adjacencyList[dest].push_back(Edge(dest, src, weight));
    }

    std::string listNodes() const {
        std::string result;
        for (const auto& node : adjacencyList)
            result += node.first + std::string(" ");
        return result;
    }

    std::string displayEdges() const {
        std::string result;
        for (const auto& node : adjacencyList) {
            result += node.first + std::string(": ");
            for (const auto& edge : node.second)
                result += "(" + std::string(1, edge.destination) + "," + std::to_string(edge.weight) + ") ";
            result += "\n";
        }
        return result;
    }

    std::string displayMatrix() const {
        std::string result;
        for (const auto& node : adjacencyList) {
            result += node.first + std::string(": ");
            for (const auto& edge : node.second) {
                result += std::to_string(edge.weight) + " ";
            }
            result += "\n";
        }
        return result;
    }

    std::string breadthFirst(char start) const {
        std::string result;
        std::set<char> visited;
        std::queue<char> queue;

        visited.insert(start);
        queue.push(start);

        while (!queue.empty()) {
            char current = queue.front();
            queue.pop();

            result += " " + std::string(1, current);

            // Reverse the order of exploration for neighbors
            std::vector<Edge> neighbors = adjacencyList.at(current);
            std::reverse(neighbors.begin(), neighbors.end());

            for (const auto& edge : neighbors) {
                char destination = edge.destination;
                if (visited.find(destination) == visited.end()) {
                    visited.insert(destination);
                    queue.push(destination);
                }
            }
        }

        for (const auto& node : nodeList) {
            if (visited.find(node) == visited.end()) {
                result += " Unreached " + std::string(1, node);
            }
        }

        return result.substr(1); // Remove the leading space
    }



    std::string depthFirst(char start) const {
        std::string result;
        std::set<char> visited;
        std::stack<char> stack;

        stack.push(start);

        while (!stack.empty()) {
            char current = stack.top();
            stack.pop();

            if (visited.find(current) != visited.end()) {
                continue;  // Skip if already visited
            }

            visited.insert(current);
            result += " " + std::string(1, current);

            for (const auto& edge : adjacencyList.at(current)) {
                char destination = edge.destination;
                if (visited.find(destination) == visited.end()) {
                    stack.push(destination);
                }
            }
        }

        for (const auto& node : nodeList) {
            if (visited.find(node) == visited.end()) {
                result += " Unreached " + std::string(1, node);
            }
        }

        return result.substr(1); // Remove the leading space
    }




    std::string minCostTree(char start) const {
        std::string result;
        std::map<char, bool> visited;
        std::priority_queue<Edge, std::vector<Edge>, CompareEdges> minHeap;

        result += start;
        result += ": ";

        for (const auto& node : adjacencyList)
            visited[node.first] = false;

        visited[start] = true;

        for (const auto& edge : adjacencyList.at(start))
            minHeap.push(edge);

        while (!minHeap.empty()) {
            Edge currentEdge = minHeap.top();
            minHeap.pop();

            if (visited[currentEdge.destination])
                continue;

            result += currentEdge.source + std::string("-") + currentEdge.destination;
            result += " ";

            visited[currentEdge.destination] = true;

            for (const auto& edge : adjacencyList.at(currentEdge.destination)) {
                if (!visited[edge.destination])
                    minHeap.push(edge);
            }
        }

        for (const auto& node : adjacencyList) {
            if (!visited[node.first]) {
                result += "Unreached ";
                result += node.first + std::string(" ");
            }
        }

        return result;
    }


    std::string minCostPaths(char start) const {
        std::string result;
        std::map<char, int> cost;
        std::map<char, char> parent;

        // Initialize cost and parent maps
        for (auto node : adjacencyList) {
            cost[node.first] = (node.first == start) ? 0 : INT_MAX;
            parent[node.first] = '\0';
        }

        // Relax edges repeatedly
        for (int i = 0; i < nodeList.size() - 1; i++) {
            for (auto node : adjacencyList) {
                char u = node.first;

                for (auto edge : node.second) {
                    char v = edge.destination;
                    int weight = edge.weight;

                    if (cost[u] != INT_MAX && cost[u] + weight < cost[v]) {
                        cost[v] = cost[u] + weight;
                        parent[v] = u;
                    }
                }
            }
        }

        // Check for negative cycles
        for (auto node : adjacencyList) {
            char u = node.first;

            for (auto edge : node.second) {
                char v = edge.destination;
                int weight = edge.weight;

                if (cost[u] != INT_MAX && cost[u] + weight < cost[v]) {
                    // Negative cycle found
                    return "Negative cycle detected!";
                }
            }
        }

        // Build the result string
        for (auto node : adjacencyList) {
            if (node.first == start) continue;
            if (cost[node.first] == INT_MAX) {
                result += node.first + std::string("(inf) ");
            }
            else {
                result += node.first + std::string("(") + std::to_string(cost[node.first]) + std::string(") ");
            }

        }

        return result;
    }

};