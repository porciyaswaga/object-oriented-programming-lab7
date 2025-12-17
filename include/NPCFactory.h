#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H

#include "NPC.h"
#include <vector>

class NPCFactory {
public:
    static std::shared_ptr<NPC> create(NPCType type, const std::string& name, int x, int y);
    static std::shared_ptr<NPC> createRandom(int id, int maxX, int maxY);
    static std::vector<std::shared_ptr<NPC>> createMany(int count, int maxX, int maxY);
};

#endif
