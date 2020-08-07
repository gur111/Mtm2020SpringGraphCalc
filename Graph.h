#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>

#include "Exceptions.h"

namespace GraphCalc {
class Graph {
   private:
    static void readNode(std::ifstream& ifs, char*& name,
                         unsigned int& max_size);
    std::set<std::string> nodes;
    std::map<std::string, std::set<std::string>> edges;

    /**
     * Turns nodes string (comma separated) into nodes and add to graph.
     * Assumes a valid format
     */
    void addNodes(std::string nodes_literal);
    /**
     * Turns edges string (comma separated) into edges and add to graph
     * Assumes a valid format
     */
    void addEdges(std::string edges_literal);
    void addEdge(std::string edge_literal);

   public:
    Graph() = default;
    /**
     * Create a graph from a literal string.
     * Literal string MUST be sanitized
     */
    Graph(std::string literal);
    Graph(const std::set<std::string>& nodes,
          const std::map<std::string, std::set<std::string>>& edges);
    // Union
    Graph operator+(const Graph& other) const;
    // Difference
    Graph operator-(const Graph& other) const;
    // Multiply
    Graph operator*(const Graph& other) const;
    // Intersection
    Graph operator^(const Graph& other) const;
    // Complement
    Graph operator!() const;
    // Graph Comparison
    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;
    void saveToFile(std::string filename) const;
    static std::shared_ptr<Graph> loadFromFile(std::string filename);
    static std::string pairNodes(const std::string& a, const std::string& b);

    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
};
}  // namespace GraphCalc
#endif