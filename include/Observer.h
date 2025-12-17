#ifndef OBSERVER_H
#define OBSERVER_H

#include "NPC.h"
#include <fstream>
#include <mutex>
#include <memory>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onKill(const NPC& killer, const NPC& victim) = 0;
};

class ConsoleObserver : public Observer {
    std::mutex& printMutex;
public:
    ConsoleObserver(std::mutex& mtx) : printMutex(mtx) {}
    void onKill(const NPC& killer, const NPC& victim) override;
};

class FileObserver : public Observer {
    std::ofstream file;
    std::mutex fileMutex;
public:
    FileObserver(const std::string& filename);
    ~FileObserver();
    void onKill(const NPC& killer, const NPC& victim) override;
};

#endif
