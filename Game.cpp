#include "Game.h"
#include "NPCFactory.h"
#include <iostream>
#include <random>
#include <chrono>

Game::Game(int w, int h, int dur) : mapWidth(w), mapHeight(h), duration(dur) {}

void Game::addObserver(std::shared_ptr<Observer> obs) { observers.push_back(obs); }

void Game::spawnNPCs(int count) { npcs = NPCFactory::createMany(count, mapWidth, mapHeight); }

void Game::notifyKill(const NPC& killer, const NPC& victim) {
    for (auto& obs : observers) obs->onKill(killer, victim);
}

int Game::countAlive() const {
    int c = 0;
    for (const auto& npc : npcs) if (npc->isAlive()) c++;
    return c;
}

void Game::movementLoop() {
    std::random_device rd;
    std::mt19937 gen(rd());

    while (running) {
        {
            std::unique_lock<std::shared_mutex> lock(npcMutex);
            for (auto& npc : npcs) {
                if (!npc->isAlive()) continue;
                int r = npc->getMoveDistance();
                std::uniform_int_distribution<> dist(-r, r);
                npc->move(dist(gen), dist(gen), mapWidth, mapHeight);
            }

            for (size_t i = 0; i < npcs.size(); ++i) {
                if (!npcs[i]->isAlive()) continue;
                for (size_t j = i + 1; j < npcs.size(); ++j) {
                    if (!npcs[j]->isAlive()) continue;
                    double d = npcs[i]->distanceTo(*npcs[j]);
                    if (d <= npcs[i]->getKillDistance()) {
                        std::lock_guard<std::mutex> qLock(queueMutex);
                        battleQueue.push({npcs[i], npcs[j]});
                        battleCV.notify_one();
                    }
                    if (d <= npcs[j]->getKillDistance()) {
                        std::lock_guard<std::mutex> qLock(queueMutex);
                        battleQueue.push({npcs[j], npcs[i]});
                        battleCV.notify_one();
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::battleLoop() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dice(1, 6);

    while (running || !battleQueue.empty()) {
        BattleTask task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (battleQueue.empty()) {
                battleCV.wait_for(lock, std::chrono::milliseconds(50));
                continue;
            }
            task = battleQueue.front();
            battleQueue.pop();
        }

        std::unique_lock<std::shared_mutex> lock(npcMutex);
        if (!task.attacker->isAlive() || !task.defender->isAlive()) continue;
        if (!task.attacker->canKill(*task.defender)) continue;

        if (dice(gen) > dice(gen)) {
            task.defender->kill();
            notifyKill(*task.attacker, *task.defender);
        }
    }
}

void Game::printMap() {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << "\033[2J\033[H";

    std::vector<std::string> map(mapHeight + 1, std::string(mapWidth + 1, '.'));
    {
        std::shared_lock<std::shared_mutex> npcLock(npcMutex);
        for (const auto& npc : npcs)
            if (npc->isAlive())
                map[npc->getY()][npc->getX()] = npc->getSymbol();
    }

    std::cout << "K=Knight D=Druid E=Elf\n";
    for (int y = 0; y <= mapHeight; y += 2) {
        for (int x = 0; x <= mapWidth; x += 2)
            std::cout << map[y][x];
        std::cout << '\n';
    }
    std::cout << "Alive: " << countAlive() << "\n";
}

void Game::printSurvivors() {
    std::lock_guard<std::mutex> lock(printMutex);
    std::shared_lock<std::shared_mutex> npcLock(npcMutex);
    std::cout << "\n=== GAME OVER ===\nSurvivors:\n";
    int n = 0;
    for (const auto& npc : npcs)
        if (npc->isAlive())
            std::cout << ++n << ". " << npc->getTypeName() << " " << npc->getName()
                      << " (" << npc->getX() << "," << npc->getY() << ")\n";
    std::cout << "Total: " << n << "\n";
}

void Game::run() {
    running = true;
    std::thread mover([this]() { movementLoop(); });
    std::thread fighter([this]() { battleLoop(); });

    auto start = std::chrono::steady_clock::now();
    while (running) {
        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - start).count();
        if (elapsed >= duration) { running = false; break; }
        printMap();
        { std::lock_guard<std::mutex> lock(printMutex); std::cout << "Time: " << (duration - elapsed) << "s\n"; }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    battleCV.notify_all();
    mover.join();
    fighter.join();
    printSurvivors();
}
