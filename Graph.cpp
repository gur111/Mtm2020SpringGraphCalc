#include "Graph.h"

#include <algorithm>
#include <iostream>
#include <iterator>

using std::cout;
using std::endl;
using std::map;
using std::set;
using std::string;

namespace GraphCalc {

template <class T>
set<T> operator+(const set<T>& a, const set<T>& b) {
    set<T> new_set(a);
    new_set.insert(b.cbegin(), b.cend());
    return new_set;
}
template <class T>
set<T> operator-(const set<T>& a, const set<T>& b) {
    set<T> new_set;
    std::set_difference(a.cbegin(), a.cend(), b.cbegin(), b.cend(),
                        std::inserter(new_set, new_set.end()));
    return new_set;
}

string Graph::pair_nodes(const string& a, const string& b) {
    return "[" + a + ";" + b + "]";
}

Graph::Graph(const set<string>& nodes, const map<string, set<string>>& edges)
    : nodes(nodes), edges(edges) {
    for (auto node : edges) {
        if (nodes.find(node.first) == nodes.cend()) {
            // TODO: Throw exception
            cout << "Error: node " << node.first << " is not declared" << endl;
        }
        for (auto dst_node : node.second) {
            if (nodes.find(dst_node) == nodes.cend()) {
                // TODO: Throw exception
                cout << "Error: node " << dst_node << " is not declared"
                     << endl;
            }
        }
    }
}

Graph Graph::operator+(const Graph& other) const {
    map<string, set<string>> new_edges = this->edges;
    for (auto edge : other.edges) {
        new_edges[edge.first] = new_edges[edge.first] + edge.second;
    }

    return Graph(this->nodes + other.nodes, new_edges);
}

Graph Graph::operator-(const Graph& other) const {
    map<string, set<string>> new_edges;
    for (auto edge : this->edges) {
        // Check if the src_node is in the `other` graph
        if (other.nodes.find(edge.first) == other.nodes.cend()) {
            // Remove edges to nodes in the `other` graph
            set<string> dst_nodes = edge.second - other.nodes;
            if (dst_nodes.size() > 0) {
                new_edges[edge.first] = dst_nodes;
            }
        }
    }

    return Graph(this->nodes - other.nodes, new_edges);
}

Graph Graph::operator*(const Graph& other) const {
    map<string, set<string>> nedges;
    set<string> nnodes;
    for (auto snode : this->nodes) {
        for (auto onode : this->nodes) {
            nnodes.insert(pair_nodes(snode, onode));
        }
    }

    for (auto sedge : this->edges) {
        for (auto s_dst_node : sedge.second) {
            for (auto oedge : this->edges) {
                for (auto o_dst_node : oedge.second) {
                    nedges[pair_nodes(sedge.first, oedge.first)].insert(
                        pair_nodes(s_dst_node, o_dst_node));
                }
            }
        }
    }

    return Graph(nnodes, nedges);
}

Graph Graph::operator^(const Graph& other) const {}

Graph Graph::operator==(const Graph& other) const {
    return (this->nodes == other.nodes and this->edges == other.edges);
}

Graph Graph::operator!=(const Graph& other) const {
    return not(*this == other);
}

Graph Graph::operator!() const {
    map<string, set<string>> new_edges;
    for (auto node : this->nodes) {
        // Remove edges to nodes in the `other` graph
        set<string> dst_nodes = this->nodes;
        // If the node has edge in the original graph
        if (this->edges.find(node) != this->edges.cend()) {
            dst_nodes = dst_nodes - (this->edges.find(node))->second;
        }
        if (dst_nodes.size() > 0) {
            new_edges[node] = dst_nodes;
        }
    }

    return Graph(this->nodes, new_edges);
}
}  // namespace GraphCalc
