#include "GraphSwig.h"

#include <iostream>

using namespace GraphCalc;

Graph *create() { return new Graph(); }

void disp(Graph *graph) {
    if (graph == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    std::cout << *graph;
}

void destroy(Graph *graph) {
    if (graph == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    delete graph;
}

Graph *addVertex(Graph *graph, char *v) {
    if (graph == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    graph->addNode(v);
    return graph;
}

Graph *addEdge(Graph *graph, char *v1, char *v2) {
    if (graph == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    graph->addEdge(v1, v2);
    return graph;
}

Graph *graphUnion(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    if (graph_in1 == nullptr or graph_in2 == nullptr or graph_out == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    *graph_out = *graph_in1 + *graph_in2;
    return graph_out;
}

Graph *graphIntersection(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    if (graph_in1 == nullptr or graph_in2 == nullptr or graph_out == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    *graph_out = *graph_in1 ^ *graph_in2;
    return graph_out;
}

Graph *graphDifference(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    if (graph_in1 == nullptr or graph_in2 == nullptr or graph_out == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    *graph_out = *graph_in1 - *graph_in2;
    return graph_out;
}

Graph *graphProduct(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    if (graph_in1 == nullptr or graph_in2 == nullptr or graph_out == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    *graph_out = (*graph_in1) * (*graph_in2);
    return graph_out;
}

Graph *graphComplement(Graph *graph_in, Graph *graph_out) {
    if (graph_in == nullptr or graph_out == nullptr) {
        throw Missing("Input graph cannot be nullptr.");
    }
    *graph_out = !*graph_in;
    return graph_out;
}
// }  // namespace GraphCalc