#include "World.h"


World::World() {

}

World::~World() {

}

bool World::checkNodeTypeInternal(const WorldNodeType &type) const {
    if (type.unique()) {
        for (const std::unique_ptr<WorldNode> & node : _nodes) {
            if (node->type() == type) {
                return false;
            }
        }
    }

    return true;
}
