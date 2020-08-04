#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace GraphCalc {
class Graph {
    std::set<std::string> nodes;
    std::map<std::string, std::set<std::string>> edges;

   public:
    Graph(const std::set<std::string>& nodes,
          const std::map<std::string, std::set<std::string>>& edges);
    ~Graph() = default;
    // Union functionality
    Graph operator+(const Graph& other) const;
    Graph operator-(const Graph& other) const;
    Graph operator*(const Graph& other) const;
    Graph operator^(const Graph& other) const;
    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;
    Graph operator!() const;

    static std::string pair_nodes(const std::string& a, const std::string& b);

    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);
};
}  // namespace GraphCalc
#endif