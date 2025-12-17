#include "NPC.h"
#include <cmath>

NPC::NPC(NPCType t, const std::string& n, int x, int y)
    : type(t), name(n), x(x), y(y), alive(true) {}

void NPC::move(int dx, int dy, int maxX, int maxY) {
    x = std::max(0, std::min(maxX, x + dx));
    y = std::max(0, std::min(maxY, y + dy));
}

double NPC::distanceTo(const NPC& other) const {
    double dx = x - other.x;
    double dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

Knight::Knight(const std::string& name, int x, int y) : NPC(NPCType::Knight, name, x, y) {}
bool Knight::canKill(const NPC& other) const {
    return other.getType() == NPCType::Elf;
}

Druid::Druid(const std::string& name, int x, int y) : NPC(NPCType::Druid, name, x, y) {}
bool Druid::canKill(const NPC& other) const {
    return other.getType() == NPCType::Druid;
}

Elf::Elf(const std::string& name, int x, int y) : NPC(NPCType::Elf, name, x, y) {}
bool Elf::canKill(const NPC& other) const {
    return other.getType() == NPCType::Druid || other.getType() == NPCType::Knight;
}
