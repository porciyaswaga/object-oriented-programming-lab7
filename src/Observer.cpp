#include "Observer.h"
#include <iostream>

void ConsoleObserver::onKill(const NPC& killer, const NPC& victim) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << killer.getTypeName() << " " << killer.getName()
              << " killed " << victim.getTypeName() << " " << victim.getName() << "\n";
}

FileObserver::FileObserver(const std::string& filename) { file.open(filename); }
FileObserver::~FileObserver() { if (file.is_open()) file.close(); }

void FileObserver::onKill(const NPC& killer, const NPC& victim) {
    std::lock_guard<std::mutex> lock(fileMutex);
    if (file.is_open())
        file << killer.getTypeName() << " " << killer.getName()
             << " killed " << victim.getTypeName() << " " << victim.getName() << "\n";
}
