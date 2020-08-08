/* Graph.i SWIG interface file */
%module example 
%{
#define SWIG_FILE_WITH_INIT
    /* Put header files here or function declarations like below */
#include "GraphSwig.h"
#include "Exceptions.h"
%}

GraphCalc::Graph *create();