//
// Created by Rares Bozga on 08.04.2023.
//

#include "graphtraverser.h"

#include <utility>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

GraphTraverser::GraphTraverser(Graph graph) : graph(std::move(graph))
{}

/**
 * Grabs all vertices that can be reached from a start node using BFS.
 * @return A tuple of 3 maps: parent, children, and distances.
 * The first maps the first is a dictionary mapping each accessible vertex to its parent.
 * The second maps each accessible vertex to the list of its children.
 * The third maps mapping each accessible vertex to the distance from startNode to it.
 */
tuple<map<int, int>, map<int, vector<int>>, map<int, int>> GraphTraverser::breadthFirstSearch(int startNode) {
    map<int, int> parent = map<int, int>();
    map<int, vector<int>> children = map<int, vector<int>>();
    map<int, int> distances = map<int, int>();

    // parent of startNode is always null
    children[startNode] = vector<int>();
    distances[startNode] = 0;

    vector<int> queue = vector<int>(); // create a queue
    queue.push_back(startNode); // queue starting node
    map<int, int> visited = map<int, int>(); // visited nodes here
    visited[startNode] = 0; // we say that start node has been visited

    while (!queue.empty()) { // while there's stuff in the queue
        int current = queue[0]; // we pop the first element in the queue
        queue.erase(queue.begin());

        for (const auto &x : graph.getVerticesOut(current)) { // iterate through out vertices
            if (visited.find(x) == visited.end()) { // if not yet visited
                children[x] = vector<int>(); // initialize the children of this unvisited node
                parent[x] = current; // the parent of this node is the one we're iterating
                distances[x] = distances[current] + 1; // the distance from the start node is previous dist + 1

                visited[x] = 0; // mark as visited
                queue.push_back(x); // add this to the queue behind everyone else
                children[current].push_back(x); // for this node, say this is is one of the children of it
            }
        }
    }

    return { parent, children, distances }; // return the magic tuple
}

/**
 * Generates an n x n matrix containing all pairs shortest paths (APSP).
 * This is implemented as slowAPSP, meaning we don't use the cool squaring trick.
 * For small matrix sizes (n < 50) this will be fine (and as such, matrix obj is capped)
 *
 * When the matrix finds a new minimum, a path 2D array gets updated such that paths[i][j]
 * will always give you the next node to travel to such that you're on the min cost path.
 *
 * That way, travelling upon the nodes of paths[i][j] = your new node, and from that new node
 * again paths[i][j] to get a newer node, until you get to j, will get you to your destination.
 */
std::pair<matrix, std::vector<int>> GraphTraverser::allPairsShortestPath(bool printVerbose, int x, int y) {
    // Note: D^0 = identity matrix
    int n = graph.nodeCount();
    matrix W = matrix();

    vector<vector<int>> paths;
    paths.resize(n, vector<int>(n, -1)); // initialize paths with -1 as the default value
    for (int i = 0; i < graph.nodeCount(); i++) {
        for (int j : graph.getVerticesOut(i)) { // if path exists, then put J as the initial best target.
            paths[i][j] = j;
        }
    }

    for (int i = 0; i < n; i++) // fill up W matrix, aka D^1
        for (int j = 0; j < n; j++) {
            if (i == j) {
                W[i][j] = 0; // if i == j, then 0
                continue;
            }
            if (graph.isEdge(i, j)) { // if edge exists, then the cost
                W[i][j] = graph.getCost(i, j);
            } else {
                W[i][j] = COST_INFINITY; // if edge doesn't exist, then infinity
            }
        }

    auto D = matrix(W); // make a copy...

    for (int m = 2; m < n; m++) { // perform extending/matrix multiplication...

        if (printVerbose) {  // Simply log output - nothing mysterious here
            std::cout << "D^" << m - 1 << std::endl;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++)
                    std::cout << D[i][j] << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
            std::cout << "Paths Matrix" << std::endl;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++)
                    std::cout << paths[i][j] << " ";
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << "= = = = = = = = = = = = = =" << std::endl;
        } // end of logging

        D = apspExtend(D, W, paths); // D^m = extend(D^m-1, W)

    }

    if (printVerbose) { // Simply log output - nothing mysterious here
        std::cout << "D^" << n - 1 << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                std::cout << D[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Paths Matrix" << std::endl;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                std::cout << paths[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "= = = = = = = = = = = = = =" << std::endl;
    } // end of logging

    // negative cycles?
    for (int i = 0; i < n; i++)
        if (D[i][i] < 0)
            return {D, vector<int>(1, -1) };

    std::cout << std::endl;
    vector<int> path = apspGetPath(paths, x, y);
    return {D, path}; // return D^n-1
}

matrix GraphTraverser::apspExtend(matrix D, matrix W, vector<vector<int>> &paths) {
    matrix C = D; // copy matrix D upon C; note a std::move() may have worked better if implemented.
    for (int i = 0; i < graph.nodeCount(); i++)
        for (int j = 0; j < graph.nodeCount(); j++) {
            for (int k = 0; k < graph.nodeCount(); k++) {
                // we perform the matrix multiplication step here.
                if (C[i][j] > D[i][k] + W[k][j]) { // new minimum found!
                    C[i][j] = D[i][k] + W[k][j];
                    if (paths[i][k] != -1) { // needed! otherwise will fill the matrix wrong.
                        paths[i][j] = paths[i][k]; // we update our paths matrix accordingly.
                    }
                }
            }
        }
    return C;
}

vector<int> GraphTraverser::apspGetPath(vector<vector<int>>& paths, int x, int y) {
    if (paths[x][y] == -1) {
        return {}; // No path exists from x to y
    } else {
        vector<int> path = {x}; // first node on path is start node
        while (x != y) {
            x = paths[x][y]; // keep searching for next node until none left
            path.push_back(x);
        }
        return path; // returned path
    }
}

/**
 * Using the provided directed graph, attempts to perform topological sorting.
 *
 * Returns a vector of the nodes in order, provided there is no cycle in the graph.
 * If there is some cycle, will instead return a list with -1 as the first element.
 */
vector<int> GraphTraverser::topologicalSortPredecessor() {
    vector<int> sorted = vector<int>();
    vector<int> queue = vector<int>();
    map<int, int> count = map<int, int>();

    GraphIterator it = graph.iterator();
    it.first();
    while (it.valid()) {
        int x = it.getCurrent();
        count[x] = (int) graph.getVerticesIn(x).size();
        if (count[x] == 0) {
            queue.push_back(x);
        }
        it.next();
    }

    while (!queue.empty()) {
        int x = queue[0];
        queue.erase(queue.begin());
        sorted.push_back(x);
        for (int y : graph.getVerticesOut(x)) {
            count[y] = count[y] - 1;
            if (count[y] == 0) {
                queue.push_back(y);
            }
        }
    }

    if (sorted.size() < graph.nodeCount()) {
        sorted = vector<int>();
        sorted.push_back(-1);
    }

    return sorted;
}
