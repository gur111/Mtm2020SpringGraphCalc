#ifndef GRAPH_SWIG_H
#define GRAPH_SWIG_H

#include "Graph.h"

GraphCalc::Graph *create();
GraphCalc::Graph *create();
void disp(GraphCalc::Graph *graph);
void destroy(GraphCalc::Graph *graph);

GraphCalc::Graph *addVertex(GraphCalc::Graph *graph, char *v);
GraphCalc::Graph *addEdge(GraphCalc::Graph *graph, char *v1, char *v2);
GraphCalc::Graph *graphUnion(GraphCalc::Graph *graph_in1,
                             GraphCalc::Graph *graph_in2,
                             GraphCalc::Graph *graph_out);
GraphCalc::Graph *graphIntersection(GraphCalc::Graph *graph_in1,
                                    GraphCalc::Graph *graph_in2,
                                    GraphCalc::Graph *graph_out);
GraphCalc::Graph *graphDifference(GraphCalc::Graph *graph_in1,
                                  GraphCalc::Graph *graph_in2,
                                  GraphCalc::Graph *graph_out);
GraphCalc::Graph *graphProduct(GraphCalc::Graph *graph_in1,
                               GraphCalc::Graph *graph_in2,
                               GraphCalc::Graph *graph_out);
GraphCalc::Graph *graphComplement(GraphCalc::Graph *graph_in,
                                  GraphCalc::Graph *graph_out);

#endif