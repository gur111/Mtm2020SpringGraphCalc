#include "StorageManager.h"

#include "Exceptions.h"

using std::shared_ptr;

namespace GraphCalc {
void StorageManager::who() {
    for (auto graph : vars) {
        std::cout << graph.first << std::endl;
    }
}
void StorageManager::reset() { this->vars.clear(); }
shared_ptr<Graph> StorageManager::set(std::string name,
                                      shared_ptr<Graph> graph) {
    return vars[name] = graph;
}
void StorageManager::remove(std::string name) {
    if (this->vars.erase(name) == 0) {
        throw Missing("Graph: " + name + " not declared");
    }
}

shared_ptr<Graph> StorageManager::get(std::string name) {
    if (vars.find(name) == vars.end()) {
        throw Missing("Graph: " + name + " not declared");
    } else {
        return vars[name];
    }
}
}  // namespace GraphCalc