#pragma once
#include "Organism.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <time.h>

class Zombie :
    public Organism
{
public:

    using Organism::Organism;

    // Inherited via Organism
    virtual position TakeTurn() override;
    
    virtual char GetIcon() override;

    virtual std::vector<position> GetPossibleMoves() override;

    bool CheckIsAlive() { return m_MoveCount <= m_DeathLimit; };

private:

    //How many turns before the zombie dies
    int m_DeathLimit = 3;//This may result in immortal zombies

    const char m_Icon = 'Z';
};
