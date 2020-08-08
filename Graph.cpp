#include "Graph.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#define DEFAULT_NAME_SIZE 10
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::shared_ptr;
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
    int next_delim_pos, delim_pos = 0;
    string node;
    while ((next_delim_pos = nodes_literal.substr(delim_pos).find(",")) !=
           string::npos) {
        node = nodes_literal.substr(delim_pos, next_delim_pos);
        if (nodes.find(node) != nodes.end()) {
            throw MultipleDeclarations(node + " has already been declared.");
        }
        nodes.insert(node);
        delim_pos += next_delim_pos + 1;
    }
    nodes.insert(nodes_literal.substr(delim_pos));
}

void Graph::addEdge(string edge_literal) {
    int nodes_delim;
    if ((nodes_delim = edge_literal.find(",")) == string::npos) {
        throw InvalidFormat("The edge \"" + edge_literal +
                            "\" is in an invalid format.");
    }
    string first = edge_literal.substr(0, nodes_delim);
    string second = edge_literal.substr(nodes_delim + 1);

    if (nodes.find(first) == nodes.end()) {
        throw Missing("The node: " + first + ".");
    } else if (nodes.find(second) == nodes.end()) {
        throw Missing("The node: " + second + ".");
    } else if (first == second) {
        throw InvalidFormat("In " + edge_literal + " Self loops not allowed.");
    } else if (edges[first].find(second) != edges[first].end()) {
        throw InvalidFormat("In " + edge_literal +
                            " Parallel edges not allowed.");
    }
    edges[first] += second;
}

void Graph::addEdges(string edges_literal) {
    // Add edges
    int next_delim_pos, delim_pos = edges_literal.find("<");
    if (delim_pos == string::npos) {
        throw InvalidFormat("In: " + edges_literal + ".");
    }
    string delim = string(">,<");
    while ((next_delim_pos = edges_literal.substr(delim_pos).find(delim)) !=
           string::npos) {
        addEdge(edges_literal.substr(delim_pos + 1, next_delim_pos - 1));
        delim_pos += next_delim_pos + delim.length() - 1;
    }
    addEdge(edges_literal.substr(delim_pos + 1,
                                 edges_literal.length() - delim_pos - 2));
}

Graph::Graph(std::string literal) {
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
            throw Missing("The node: " + node.first + ".");
        }
        for (auto dst_node : node.second) {
            if (nodes.find(dst_node) == nodes.cend()) {
                throw Missing("The node: " + dst_node + ".");
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
        for (auto onode : other.nodes) {
            nnodes.insert(pairNodes(snode, onode));
        }
    }

    for (auto sedge : this->edges) {
        for (auto s_dst_node : sedge.second) {
            for (auto oedge : other.edges) {
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
        dst_nodes.erase(node);
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

void Graph::readNode(std::ifstream& ifs, char*& name, unsigned int& max_size) {
    unsigned name_size;
    ifs.read((char*)&name_size, sizeof(name_size));
    // Expand name allocation if needed
    if (name_size >= max_size) {
        delete[] name;
        max_size = name_size + 1;
        name = new char[max_size];
    }
    name[name_size] = '\0';
    ifs.read(name, name_size);
}

std::shared_ptr<Graph> Graph::loadFromFile(std::string filename) {
    shared_ptr<Graph> graph(new Graph());
    std::ifstream infile;
    infile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    infile.open(filename, std::ios_base::binary);
    unsigned int node_count, edge_count;
    // Get nodes and edges count
    infile.read((char*)&node_count, sizeof(node_count));
    infile.read((char*)&edge_count, sizeof(edge_count));
    // Alocate space for a temp node names variable
    unsigned int name_size;
    unsigned int name_max_size = DEFAULT_NAME_SIZE;
    char* name = new char[name_max_size];
    // Load nodes
    for (int i = 0; i < node_count; i++) {
        readNode(infile, name, name_max_size);
        graph->nodes += string(name);
    }
    // Load edges
    string snode;
    for (int i = 0; i < edge_count; i++) {
        // src node
        readNode(infile, name, name_max_size);
        snode = name;
        // dst node
        readNode(infile, name, name_max_size);
        graph->edges[snode] += name;
    }

    delete[] name;
    return graph;
}

void Graph::saveToFile(std::string filename) const {
    std::ofstream outfile;
    outfile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    outfile.open(filename, std::ios_base::binary);
    // Node count
    unsigned int tmp_num = nodes.size();
    outfile.write((const char*)&tmp_num, sizeof(tmp_num));
    tmp_num = 0;
    for (auto edge : edges) {
        tmp_num += edge.second.size();
    }
    outfile.write((const char*)&tmp_num, sizeof(tmp_num));
    for (auto node : nodes) {
        tmp_num = node.length();
        outfile.write((const char*)&tmp_num, sizeof(tmp_num));
        outfile.write(node.c_str(), tmp_num);
    }
    for (auto edge : edges) {
        for (auto dnode : edge.second) {
            tmp_num = edge.first.length();
            outfile.write((const char*)&tmp_num, sizeof(tmp_num));
            outfile.write(edge.first.c_str(), tmp_num);
            tmp_num = dnode.length();
            outfile.write((const char*)&tmp_num, sizeof(tmp_num));
            outfile.write(dnode.c_str(), tmp_num);
        }
    }
    outfile.close();
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
