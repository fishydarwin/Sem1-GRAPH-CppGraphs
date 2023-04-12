//
// Created by Rares Bozga on 08.04.2023.
//

#include "graphtraverser.h"

#include <utility>
#include <vector>
#include <tuple>

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

    vector<int> queue = vector<int>();
    queue.push_back(startNode);
    map<int, int> visited = map<int, int>();
    visited[startNode] = 0;

    while (!queue.empty()) {
        int current = queue[0];
        queue.erase(queue.begin());

        for (const auto &x : graph.getVerticesOut(current)) {
            if (visited.find(x) == visited.end()) {
                children[x] = vector<int>();
                parent[x] = current;
                distances[x] = distances[current] + 1;

                visited[x] = 0;
                queue.push_back(x);
                children[current].push_back(x);
            }
        }
    }

    return { parent, children, distances };
}
