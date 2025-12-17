#ifndef NPC_H
#define NPC_H

#include <string>
#include <memory>

enum class NPCType { Knight, Druid, Elf };

class NPC {
protected:
    NPCType type;
    std::string name;
    int x, y;
    bool alive;

public:
    NPC(NPCType t, const std::string& n, int x, int y);
    virtual ~NPC() = default;

    NPCType getType() const { return type; }
    std::string getName() const { return name; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool isAlive() const { return alive; }
    void kill() { alive = false; }

    void move(int dx, int dy, int maxX, int maxY);
    double distanceTo(const NPC& other) const;

    virtual int getMoveDistance() const = 0;
    virtual int getKillDistance() const = 0;
    virtual bool canKill(const NPC& other) const = 0;
    virtual std::string getTypeName() const = 0;
    virtual char getSymbol() const = 0;
};

class Knight : public NPC {
public:
    Knight(const std::string& name, int x, int y);
    int getMoveDistance() const override { return 30; }
    int getKillDistance() const override { return 10; }
    bool canKill(const NPC& other) const override;
    std::string getTypeName() const override { return "Knight"; }
    char getSymbol() const override { return 'K'; }
};

class Druid : public NPC {
public:
    Druid(const std::string& name, int x, int y);
    int getMoveDistance() const override { return 10; }
    int getKillDistance() const override { return 10; }
    bool canKill(const NPC& other) const override;
    std::string getTypeName() const override { return "Druid"; }
    char getSymbol() const override { return 'D'; }
};

class Elf : public NPC {
public:
    Elf(const std::string& name, int x, int y);
    int getMoveDistance() const override { return 10; }
    int getKillDistance() const override { return 50; }
    bool canKill(const NPC& other) const override;
    std::string getTypeName() const override { return "Elf"; }
    char getSymbol() const override { return 'E'; }
};

#endif
