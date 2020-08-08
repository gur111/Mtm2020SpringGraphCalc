#ifndef GRAPH_CALC_H
#define GRAPH_CALC_H

#include "StorageManager.h"

namespace GraphCalc {
class GraphCalc {
    StorageManager storage;

   public:
    bool runCmd(std::string command);
};
}  // namespace GraphCalc
#endif