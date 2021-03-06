#include "GraphSwig.h"
#include "Graph.h"

#include <iostream>

using namespace GraphCalc;

Graph *create() {
    try {
        return new Graph();
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}

void disp(Graph *graph) {
    try {
        if (graph == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        std::cout << *graph;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
    }
}

void destroy(Graph *graph) {
    try {
        if (graph == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        delete graph;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
    }
}

Graph *addVertex(Graph *graph, char *v) {
    try {
        if (graph == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        graph->addNode(v);
        return graph;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}

Graph *addEdge(Graph *graph, char *v1, char *v2) {
    try {
        if (graph == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        graph->addEdge(v1, v2);
        return graph;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}

Graph *graphUnion(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    try {
        if (graph_in1 == nullptr or graph_in2 == nullptr or
            graph_out == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        *graph_out = *graph_in1 + *graph_in2;
        return graph_out;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}

Graph *graphIntersection(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    try {
        if (graph_in1 == nullptr or graph_in2 == nullptr or
            graph_out == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        *graph_out = *graph_in1 ^ *graph_in2;
        return graph_out;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}

Graph *graphDifference(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    try {
        if (graph_in1 == nullptr or graph_in2 == nullptr or
            graph_out == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        *graph_out = *graph_in1 - *graph_in2;
        return graph_out;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}

Graph *graphProduct(Graph *graph_in1, Graph *graph_in2, Graph *graph_out) {
    try {
        if (graph_in1 == nullptr or graph_in2 == nullptr or
            graph_out == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        *graph_out = (*graph_in1) * (*graph_in2);
        return graph_out;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}

Graph *graphComplement(Graph *graph_in, Graph *graph_out) {
    try {
        if (graph_in == nullptr or graph_out == nullptr) {
            throw Missing("Input graph cannot be a nullptr.");
        }
        *graph_out = !*graph_in;
        return graph_out;
    } catch (const std::exception &excpt) {
        std::cout << excpt.what() << std::endl;
        return NULL;
    }
}
// }  // namespace GraphCalc