#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <map>
#include <memory>
#include <set>
#include <string>

#include "Graph.h"

namespace GraphCalc {
class StorageManager {
    std::map<std::string, std::shared_ptr<Graph>> vars;

   public:
    /**
     * Clears the map
     */
    void reset();
    void who();
    std::shared_ptr<Graph> set(std::string name, std::shared_ptr<Graph> graph);
    std::shared_ptr<Graph> get(std::string name);
    void remove(std::string name);
};
}  // namespace GraphCalc

#endif