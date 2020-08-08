#include "GraphSwig.h"

namespace GraphCalc {
Graph *create() { return new Graph(); }

void disp(Graph *graph) { std::cout << *graph; }

void destroy(Graph *graph) { delete graph; }

Graph *addVertex(Graph *graph, char *v) {
    graph->addNode(v);
    return graph;
}

Graph *addEdge(Graph *graph, char *v1, char *v2) {
    graph->addEdge(v1, v2);
    return graph;
}

Graph *graphUnion(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    *graph_out = *graph_in1 + *graph_in2;
    return graph_out;
}

Graph *graphIntersection(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    *graph_out = *graph_in1 ^ *graph_in2;
    return graph_out;
}

Graph *graphDifference(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    *graph_out = *graph_in1 - *graph_in2;
    return graph_out;
}

Graph *graphProduct(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    *graph_out = (*graph_in1) * (*graph_in2);
    return graph_out;
}

Graph *graphComplement(Graph *graph_in, Graph *graph_out) {
    *graph_out = !*graph_in;
    return graph_out;
}
}  // namespace GraphCalc