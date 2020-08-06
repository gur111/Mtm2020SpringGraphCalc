#include "StorageManager.h"

using std::shared_ptr;

namespace GraphCalc {
void StorageManager::who() {
    for (auto graph : vars) {
        std::cout << graph.first;
    }
}
void StorageManager::reset() { this->vars.clear(); }
shared_ptr<Graph> StorageManager::set(std::string name,
                                      shared_ptr<Graph> graph) {
    return vars[name] = graph;
}
void StorageManager::remove(std::string name) {
    if (this->vars.erase(name) == 0) {
        // TODO: Throw error, element doesn't exist
    }
}

shared_ptr<Graph> StorageManager::get(std::string name) {
    if (vars.find(name) == vars.end()) {
        // TODO: Throw exception var not defined
        return nullptr;
    } else {
        return vars[name];
    }
}
}  // namespace GraphCalc