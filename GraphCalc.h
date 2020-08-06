#ifndef GRAPH_CALC_H
#define GRAPH_CALC_H

#include "StorageManager.h"

namespace GraphCalc {
class GraphCalc {
    StorageManager storage;

   public:
   void runCmd(std::string command);
};
}  // namespace GraphCalc
#endif