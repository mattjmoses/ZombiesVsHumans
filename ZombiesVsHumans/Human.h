#pragma once

#include "Organism.h"
#include "Zombie.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <time.h>

class Human :
    public Organism
{
public:

    //Inheriting the constructor from Organism
    using Organism::Organism;
 
    // Inherited via Organism
    virtual position TakeTurn() override;

    virtual char GetIcon() override;

    virtual std::vector<position> GetPossibleMoves() override;

private:

    const char m_Icon = 'H';

};
