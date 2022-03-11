#include "Zombie.h"

Organism::position Zombie::TakeTurn()
{
    //By default just use our current position.
    position ret = { m_CurrentPosition.x,m_CurrentPosition.y };

    direction moveDirVertical = direction::DEFAULT;
    direction moveDirHotizontal = direction::DEFAULT;

    std::vector<std::pair<position, char>> surroundingTiles = GetSurroundingTiles(2);

    std::vector<position> humanTiles;

    std::vector<position> possibleMoves = GetPossibleMoves();

    for (const std::pair<position, char>& pos : surroundingTiles)
    {
        if (pos.second == 'H')
        {
            humanTiles.push_back(pos.first);
        }
    }

    // get a time-based seed
    unsigned seed = std::chrono::system_clock::now()
        .time_since_epoch()
        .count();
    shuffle(possibleMoves.begin(), possibleMoves.end(), std::default_random_engine(seed));
    shuffle(humanTiles.begin(), humanTiles.end(), std::default_random_engine(seed));

    if (GetIsAlive())
    {
        //To start, if we have no humans and we can actually move just choose a random tile and move to it
        if (humanTiles.size() == 0 && possibleMoves.size() > 0)
        {
            ret = possibleMoves[0];
        }
        else if (humanTiles.size() > 0 && possibleMoves.size() > 0)
        {
            position humanPos = humanTiles[0];
            position curPos = m_CurrentPosition;

            //We now make our way towards the human
            //Generally just move in the direction of the human until we reach them.
            if (humanPos.y < curPos.y)
            {
                moveDirHotizontal = direction::LEFT;
            }
            else if (humanPos.y > curPos.y)
            {
                moveDirHotizontal = direction::RIGHT;
            }

            if (humanPos.x > curPos.x)
            {
                moveDirVertical = direction::DOWN;
            }
            else
            {
                moveDirVertical = direction::UP;
            }

            bool directionChosen = false;

            //Now let's figure out if we have somewhere to go
            for (const position& pos : possibleMoves)
            {
                direction tilePosVertical = direction::DEFAULT;
                direction tilePosHorizontal = direction::DEFAULT;

                if (pos.x > curPos.x)
                {
                    tilePosVertical = direction::DOWN;
                }
                else
                {
                    tilePosVertical = direction::UP;
                }
                if (pos.y > curPos.y)
                {
                    tilePosHorizontal = direction::RIGHT;
                }
                else
                {
                    tilePosHorizontal = direction::LEFT;
                }

                if ((tilePosHorizontal == moveDirHotizontal) && (tilePosVertical == moveDirVertical))
                {
                    ret = pos;
                    directionChosen = true;
                    break;
                }
            }
            //in the event we don't have a move that meets our criteria we just choose a random direction
            //from the move list.
            if (!directionChosen)
            {
                ret = possibleMoves[0];
            }
        }
    }

    //Quick death for zombies
    if (GetMoveCount() >= m_DeathLimit)
    {
        SetIsAlive(false);
    }

    SetTurnTaken(true);
    IncreaseMoveCount();

    return ret;
}

char Zombie::GetIcon()
{
    return m_Icon;
}

std::vector<Organism::position> Zombie::GetPossibleMoves()
{
    std::vector<position> moves;
    for (const std::pair<position, char>& tile : GetSurroundingTiles(1))
    {
        if (GAMEBOARD[tile.first.x][tile.first.y] == nullptr)
        {
            moves.push_back(position{ tile.first.x,tile.first.y });
        }
        else if (GAMEBOARD[tile.first.x][tile.first.y]->GetIcon() == 'H')
        {
            m_DeathLimit += 3;
        }
    }
    return moves;
}