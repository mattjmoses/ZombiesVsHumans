#include "Organism.h"


Organism::~Organism()
{
    GAMEBOARD.clear();
}

std::vector<std::pair<Organism::position, char>> Organism::GetSurroundingTiles(int range)
{
    std::vector<std::pair<position,char>> surroundingTiles;

    int row = m_CurrentPosition.x;
    int col = m_CurrentPosition.y;

    for (int i = (-1 * range); i < (1 + range); i++)
    {
        for (int j = (-1 * range); j < (1 + range); j++)
        {
            // skip rows out of range.
            if ((i + row) < 0 || (i + row >= GAMEBOARD.size()))
            {
                continue;
            }
            // skip columns out of range.
            if ((j + col) < 0 || (j + col >= GAMEBOARD[0].size()))
            {
                continue;
            }

            int newRow = i + row;
            int newCol = j + col;
            char icon = '.';

            if (newRow == row && newCol == col)
            {
                continue;
            }

            if (GAMEBOARD[newRow][newCol] != nullptr)
            {
                icon = GAMEBOARD[newRow][newCol]->GetIcon();
            }

            surroundingTiles.push_back(std::make_pair(position{ newRow,newCol }, icon));
        }
    }

    return surroundingTiles;
}

Organism::position Organism::GetFreeAdjacentTile()
{
    std::vector<position> surroundingTiles;
    
    int row = m_CurrentPosition.x;
    int col = m_CurrentPosition.y;

    position ret = position{ row,col };

    for (int i = (-1); i < (2); i++)
    {
        for (int j = (-1); j < (2); j++)
        {
            // skip rows out of range.
            if ((i + row) < 0 || (i + row >= GAMEBOARD.size()))
            {
                continue;
            }
            // skip columns out of range.
            if ((j + col) < 0 || (j + col >= GAMEBOARD[0].size()))
            {
                continue;
            }

            int newRow = i + row;
            int newCol = j + col;
            char icon = '.';

            if (newRow == row && newCol == col)
            {
                continue;
            }

            if (GAMEBOARD[newRow][newCol] == nullptr)
            {
                surroundingTiles.push_back(position{ newRow,newCol });
            }
        }
    }

    //Shuffle the tiles
    unsigned seed = std::chrono::system_clock::now()
        .time_since_epoch()
        .count();
    shuffle(surroundingTiles.begin(), surroundingTiles.end(), std::default_random_engine(seed));

    if (surroundingTiles.size() > 0)
    {
        ret = surroundingTiles[0];
    }

    return ret;
}
