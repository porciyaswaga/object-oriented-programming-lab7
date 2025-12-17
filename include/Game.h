#ifndef GAME_H
#define GAME_H

#include "NPC.h"
#include "Observer.h"
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <atomic>
#include <condition_variable>

struct BattleTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
};

class Game {
    int mapWidth, mapHeight, duration;
    std::vector<std::shared_ptr<NPC>> npcs;
    std::shared_mutex npcMutex;
    std::queue<BattleTask> battleQueue;
    std::mutex queueMutex;
    std::condition_variable battleCV;
    std::vector<std::shared_ptr<Observer>> observers;
    std::atomic<bool> running{false};
    std::mutex printMutex;

    void movementLoop();
    void battleLoop();
    void notifyKill(const NPC& killer, const NPC& victim);

public:
    Game(int w, int h, int dur);
    void addObserver(std::shared_ptr<Observer> obs);
    void spawnNPCs(int count);
    void run();
    void printMap();
    void printSurvivors();
    std::mutex& getPrintMutex() { return printMutex; }
    int countAlive() const;
};

#endif
