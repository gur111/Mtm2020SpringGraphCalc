#include "Graph.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <regex>

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
set<T>& operator+=(set<T>& a, const set<T>& b) {
    a.insert(b.cbegin(), b.cend());
    return a;
}

template <class T>
set<T>& operator+=(set<T>& a, const string& str) {
    a.insert(str);
    return a;
}

template <class T>
set<T> operator-(const set<T>& a, const set<T>& b) {
    set<T> new_set;
    std::set_difference(a.cbegin(), a.cend(), b.cbegin(), b.cend(),
                        std::inserter(new_set, new_set.end()));
    return new_set;
}

template <class T>
set<T> operator^(const set<T>& a, const set<T>& b) {
    set<T> new_set;
    std::set_intersection(a.cbegin(), a.cend(), b.cbegin(), b.cend(),
                          std::inserter(new_set, new_set.end()));
    return new_set;
}

void Graph::addNodes(string nodes_literal) {
    if (nodes_literal == "") {
        // TODO: Throw exception no nodes found
        return;
    }
    int next_delim_pos, delim_pos = 0;
    while ((next_delim_pos = nodes_literal.substr(delim_pos).find(",")) !=
           string::npos) {
        nodes.insert(nodes_literal.substr(delim_pos, next_delim_pos));
        delim_pos += next_delim_pos + 1;
    }
    nodes.insert(nodes_literal.substr(delim_pos));
}

void Graph::addEdge(string edge_literal) {
    int nodes_delim;
    if ((nodes_delim = edge_literal.find(",")) == string::npos) {
        // TODO: Throw invalid format
        return;
    }
    string first = edge_literal.substr(1, nodes_delim - 1);
    string second = edge_literal.substr(nodes_delim + 1);
    second = second.substr(0, second.length() - 1);

    if (nodes.find(first) == nodes.end() or nodes.find(second) == nodes.end()) {
        // TODO: Throw exception node doesn't exist;
        return;
    }
    edges[first] += second;
}

void Graph::addEdges(string edges_literal) {
    // Add edges
    int next_delim_pos, delim_pos = edges_literal.find("<");
    if (delim_pos == string::npos) {
        // TODO: Throw exception invalid format/no edges found
        return;
    }
    string delim = string(">,<");
    while ((next_delim_pos = edges_literal.substr(delim_pos).find(delim)) !=
           string::npos) {
        addEdge(edges_literal.substr(delim_pos, next_delim_pos));
        delim_pos += next_delim_pos + delim.length();
    }
    addEdge(edges_literal.substr(delim_pos));
}

Graph::Graph(std::string literal) {
    std::smatch matches;
    string name_prefix = "\\s*([A-z\\[])";
    string name_chars = "[A-z0-9;\\[\\]]";
    string node_regex = "(" + name_prefix + name_chars + "*)\\s*";
    string edge_regex = "\\s*<" + node_regex + "," + node_regex + ">\\s*";

    int delim_pos = literal.find("|");
    string nodes_literal;
    string edges_literal;
    // Split string into the edges and nodes parts
    if (delim_pos == string::npos) {
        nodes_literal = literal.substr(1, literal.length() - 2);
    } else {
        nodes_literal = literal.substr(1, delim_pos - 1);
        edges_literal =
            literal.substr(delim_pos + 1, literal.length() - 2 - delim_pos);
    }
    if (nodes_literal.length() != 0) {
        addNodes(nodes_literal);
    }
    if (edges_literal.length() != 0) {
        addEdges(edges_literal);
    }
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

string Graph::pairNodes(const string& a, const string& b) {
    return "[" + a + ";" + b + "]";
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
            nnodes.insert(pairNodes(snode, onode));
        }
    }

    for (auto sedge : this->edges) {
        for (auto s_dst_node : sedge.second) {
            for (auto oedge : this->edges) {
                for (auto o_dst_node : oedge.second) {
                    nedges[pairNodes(sedge.first, oedge.first)].insert(
                        pairNodes(s_dst_node, o_dst_node));
                }
            }
        }
    }

    return Graph(nnodes, nedges);
}

Graph Graph::operator^(const Graph& other) const {
    map<string, set<string>> new_edges;
    for (auto edge : this->edges) {
        // Check if the src_node is in the `other` graph
        if (other.edges.find(edge.first) != other.edges.cend()) {
            // Remove edges to nodes in the `other` graph
            set<string> dst_nodes =
                edge.second ^ other.edges.find(edge.first)->second;
            if (dst_nodes.size() > 0) {
                new_edges[edge.first] = dst_nodes;
            }
        }
    }

    return Graph(this->nodes ^ other.nodes, new_edges);
}

bool Graph::operator==(const Graph& other) const {
    return (this->nodes == other.nodes and this->edges == other.edges);
}

bool Graph::operator!=(const Graph& other) const { return not(*this == other); }

Graph Graph::operator!() const {
    map<string, set<string>> new_edges;
    for (auto node : this->nodes) {
        // Remove edges to nodes in the `other` graph
        set<string> dst_nodes = this->nodes;
        // If the node has edge in the original graph
        if (this->edges.find(node) != this->edges.cend()) {
            dst_nodes = dst_nodes - this->edges.find(node)->second;
        }
        if (dst_nodes.size() > 0) {
            new_edges[node] = dst_nodes;
        }
    }

    return Graph(this->nodes, new_edges);
}
std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    for (auto node : graph.nodes) {
        os << node << endl;
    }
    os << "$" << endl;
    for (auto edge : graph.edges) {
        for (auto dnode : edge.second) {
            os << edge.first << " " << dnode << endl;
        }
    }
    return os;
}
}  // namespace GraphCalc
