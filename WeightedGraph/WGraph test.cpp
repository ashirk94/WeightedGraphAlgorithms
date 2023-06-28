//
//  main.cpp
//  CS 260 Lab 9
//
//  Created by Jim Bailey on June 2, 2020.
//  Licensed under a Creative Commons Attribution 4.0 International License.
//

#include <iostream>
#include "WGraph.hpp"

#define MIN_COST_TREE
#define MIN_COST_PATHS

int main()
{
    
    std::cout << "This test displays a minimum cost spanning tree for a weighted graph"
            << std::endl << std::endl;
    
    // create the graph
    WGraph graph;
    
    // Add nodes to graph
    graph.addNode('A');
    graph.addNode('B');
    graph.addNode('C');
    graph.addNode('D');
    graph.addNode('E');
    graph.addNode('F');
    graph.addNode('G');
    graph.addNode('H');
    graph.addNode('I');
    graph.addNode('J');

    // add edges to the graph
    graph.addEdge('A', 'B', 4);
    graph.addEdge('A', 'H', 8);
    graph.addEdge('B', 'C', 8);
    graph.addEdge('B', 'H', 11);
    graph.addEdge('C', 'D', 7);
    graph.addEdge('C', 'F', 4);
    graph.addEdge('C', 'I', 2);
    graph.addEdge('D', 'E', 9);
    graph.addEdge('D', 'F', 14);
    graph.addEdge('E', 'F', 10);
    graph.addEdge('F', 'G', 2);
    graph.addEdge('G', 'H', 1);
    graph.addEdge('G', 'I', 6);
    graph.addEdge('H', 'I', 7);
    

    std::cout << "The list of nodes " << std::endl;
    std::cout << " expected A B C D E F G H I J" << std::endl;
    std::cout << " actually " << graph.listNodes() << std::endl << std::endl;
        
    std::cout << "The edge list is: " << std::endl;
    std::cout << graph.displayEdges() << std::endl << std::endl;

    std::cout << "The adjcency or edge edge matrix is" << std::endl;
    std::cout << graph.displayMatrix() << std::endl;

    std::cout << "The breadth first traversal starting at A" << std::endl;
    std::cout << " expected A H B I G C F D E Unreached J" << std::endl;
    std::cout << " actually " << graph.breadthFirst('A') << std::endl;
    
    std::cout << "The depth first traversal starting at A"<< std::endl;
    std::cout << " expected A H I G F E D C B Unreached J" << std::endl;
    std::cout << " actually " << graph.depthFirst('A') << std::endl;

#ifdef MIN_COST_TREE
    std::cout << "The min cost tree starting at A " << std::endl;
    std::cout << " Expected A: A-B A-H H-G G-F F-C C-I C-D D-E Unreached J" << std::endl;
    std::cout << " Actually " << graph.minCostTree('A') << std::endl;
    
    std::cout << "The min cost tree starting at D " << std::endl;
    std::cout << " Expected D: D-C C-I C-F F-G G-H C-B B-A D-E Unreached J" << std::endl;
    std::cout << " Actually " << graph.minCostTree('D') << std::endl;
    
    std::cout << "Done with testing min cost spanning tree " << std::endl << std::endl;
#endif // MIN_COST_TREE
    
#ifdef MIN_COST_PATHS
    std::cout << "The minimum cost paths starting at A " << std::endl;
    std::cout << " Expected B(4) C(12) D(19) E(21) F(11) G(9) H(8) I(14) J(inf) " << std::endl;
    std::cout << " Actually " << graph.minCostPaths('A') << std::endl;
    
    std::cout << "The minimum cost paths starting at D " << std::endl;
    std::cout << " Expected A(19) B(15) C(7) E(9) F(11) G(13) H(14) I(9) J(inf) " << std::endl;
    std::cout << " Actually " << graph.minCostPaths('D') << std::endl;
    
    std::cout << "Done with testing min cost paths " << std::endl << std::endl;
#endif // MIN_COST_PATHS

    return 0;
}
