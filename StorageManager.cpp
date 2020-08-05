#include "StorageManager.h"

using std::shared_ptr;

namespace GraphCalc {
shared_ptr<Graph> StorageManager::set(std::string name, shared_ptr<Graph> graph) {
    return vars[name] = graph;
}

shared_ptr<Graph> StorageManager::get(std::string name) {
    if (vars.find(name) == vars.end()) {
        // TODO: Throw exception var not defined
    } else {
        return vars[name];
    }
}
}  // namespace GraphCalc