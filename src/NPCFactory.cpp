#include "NPCFactory.h"
#include <random>

std::shared_ptr<NPC> NPCFactory::create(NPCType type, const std::string& name, int x, int y) {
    switch (type) {
        case NPCType::Knight: return std::make_shared<Knight>(name, x, y);
        case NPCType::Druid:  return std::make_shared<Druid>(name, x, y);
        case NPCType::Elf:    return std::make_shared<Elf>(name, x, y);
    }
    return nullptr;
}

std::shared_ptr<NPC> NPCFactory::createRandom(int id, int maxX, int maxY) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> typeDist(0, 2);
    std::uniform_int_distribution<> xDist(0, maxX);
    std::uniform_int_distribution<> yDist(0, maxY);

    NPCType type = static_cast<NPCType>(typeDist(gen));
    std::string name = (type == NPCType::Knight ? "K" : type == NPCType::Druid ? "D" : "E") + std::to_string(id);
    return create(type, name, xDist(gen), yDist(gen));
}

std::vector<std::shared_ptr<NPC>> NPCFactory::createMany(int count, int maxX, int maxY) {
    std::vector<std::shared_ptr<NPC>> npcs;
    for (int i = 0; i < count; ++i)
        npcs.push_back(createRandom(i, maxX, maxY));
    return npcs;
}
