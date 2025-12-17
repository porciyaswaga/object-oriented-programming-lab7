#include "Game.h"
#include <iostream>

int main() {
    Game game(100, 100, 60);
    game.addObserver(std::make_shared<ConsoleObserver>(game.getPrintMutex()));
    game.addObserver(std::make_shared<FileObserver>("log.txt"));
    game.spawnNPCs(50);
    
    std::cout << "Press Enter to start...\n";
    std::cin.get();
    
    game.run();
    return 0;
}
