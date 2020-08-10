/* graph.i SWIG interface file */
%module graph 
%{
#define SWIG_FILE_WITH_INIT
#include "GraphSwig.h"
#include "GraphSwig.h"
// #include "Exceptions.h"
// static PyObject* pGraphCalcError;
%}


// %init %{
//     pGraphCalcError = PyErr_NewException("_Graph.GraphCalcError", NULL, NULL);
//     Py_INCREF(pGraphCalcError);
//     PyModule_AddObject(m, "GraphCalcError", pGraphCalcError);
// %}

// %exception {
//     try {
//         $action
//     } catch (GraphCalc::GraphCalcError &e) {
//         // This is ugly and stupid imho with all due respect.
//         printf("%s\n", e.what());
//         // Errors should be handled like this:
//         /* PyErr_SetString(pGraphCalcError, const_cast<char*>(e.what()));
//         SWIG_fail; */
//     }
// }


// %pythoncode %{
//     GraphCalcError = _Graph.GraphCalcError
// %}

%include "GraphSwig.h"
