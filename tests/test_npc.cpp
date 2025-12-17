#include <gtest/gtest.h>
#include "NPC.h"
#include "NPCFactory.h"

TEST(NPCFactory, CreateKnight) {
    auto npc = NPCFactory::create(NPCType::Knight, "K1", 50, 50);
    EXPECT_EQ(npc->getType(), NPCType::Knight);
    EXPECT_EQ(npc->getName(), "K1");
    EXPECT_TRUE(npc->isAlive());
}

TEST(NPCFactory, CreateDruid) {
    auto npc = NPCFactory::create(NPCType::Druid, "D1", 10, 20);
    EXPECT_EQ(npc->getType(), NPCType::Druid);
    EXPECT_EQ(npc->getSymbol(), 'D');
}

TEST(NPCFactory, CreateElf) {
    auto npc = NPCFactory::create(NPCType::Elf, "E1", 30, 40);
    EXPECT_EQ(npc->getType(), NPCType::Elf);
    EXPECT_EQ(npc->getSymbol(), 'E');
}

TEST(NPCFactory, CreateMany) {
    auto npcs = NPCFactory::createMany(10, 100, 100);
    EXPECT_EQ(npcs.size(), 10);
}

TEST(NPC, Distance) {
    auto a = NPCFactory::create(NPCType::Knight, "A", 0, 0);
    auto b = NPCFactory::create(NPCType::Elf, "B", 3, 4);
    EXPECT_DOUBLE_EQ(a->distanceTo(*b), 5.0);
}

TEST(NPC, Move) {
    auto npc = NPCFactory::create(NPCType::Knight, "K", 50, 50);
    npc->move(10, 10, 100, 100);
    EXPECT_EQ(npc->getX(), 60);
    EXPECT_EQ(npc->getY(), 60);
}

TEST(NPC, MoveClamp) {
    auto npc = NPCFactory::create(NPCType::Knight, "K", 95, 5);
    npc->move(20, -20, 100, 100);
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 0);
}

TEST(NPC, KnightParams) {
    auto npc = NPCFactory::create(NPCType::Knight, "K", 0, 0);
    EXPECT_EQ(npc->getMoveDistance(), 30);
    EXPECT_EQ(npc->getKillDistance(), 10);
}

TEST(NPC, DruidParams) {
    auto npc = NPCFactory::create(NPCType::Druid, "D", 0, 0);
    EXPECT_EQ(npc->getMoveDistance(), 10);
    EXPECT_EQ(npc->getKillDistance(), 10);
}

TEST(NPC, ElfParams) {
    auto npc = NPCFactory::create(NPCType::Elf, "E", 0, 0);
    EXPECT_EQ(npc->getMoveDistance(), 10);
    EXPECT_EQ(npc->getKillDistance(), 50);
}

TEST(Battle, KnightKillsElf) {
    auto knight = NPCFactory::create(NPCType::Knight, "K", 0, 0);
    auto elf = NPCFactory::create(NPCType::Elf, "E", 0, 0);
    EXPECT_TRUE(knight->canKill(*elf));
}

TEST(Battle, KnightNotKillsDruid) {
    auto knight = NPCFactory::create(NPCType::Knight, "K", 0, 0);
    auto druid = NPCFactory::create(NPCType::Druid, "D", 0, 0);
    EXPECT_FALSE(knight->canKill(*druid));
}

TEST(Battle, ElfKillsDruid) {
    auto elf = NPCFactory::create(NPCType::Elf, "E", 0, 0);
    auto druid = NPCFactory::create(NPCType::Druid, "D", 0, 0);
    EXPECT_TRUE(elf->canKill(*druid));
}

TEST(Battle, ElfKillsKnight) {
    auto elf = NPCFactory::create(NPCType::Elf, "E", 0, 0);
    auto knight = NPCFactory::create(NPCType::Knight, "K", 0, 0);
    EXPECT_TRUE(elf->canKill(*knight));
}

TEST(Battle, DruidKillsDruid) {
    auto d1 = NPCFactory::create(NPCType::Druid, "D1", 0, 0);
    auto d2 = NPCFactory::create(NPCType::Druid, "D2", 0, 0);
    EXPECT_TRUE(d1->canKill(*d2));
}

TEST(NPC, Kill) {
    auto npc = NPCFactory::create(NPCType::Knight, "K", 0, 0);
    EXPECT_TRUE(npc->isAlive());
    npc->kill();
    EXPECT_FALSE(npc->isAlive());
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
