#include "Human.h"

Organism::position Human::TakeTurn()
{
    //By default just have our current position.
    position ret = { m_CurrentPosition.x,m_CurrentPosition.y };

    direction moveDirVertical = direction::DEFAULT;
    direction moveDirHotizontal = direction::DEFAULT;

    std::vector<std::pair<position,char>> surroundingTiles = GetSurroundingTiles(2);

    std::vector<position> zombieTiles;

    std::vector<position> possibleMoves = GetPossibleMoves();

    for (const std::pair<position, char>& pos : surroundingTiles)
    {
        if (pos.second == 'Z')
        {
            zombieTiles.push_back(pos.first);
        }
    }

    // get a time-based seed
    unsigned seed = std::chrono::system_clock::now()
        .time_since_epoch()
        .count();
    shuffle(possibleMoves.begin(), possibleMoves.end(), std::default_random_engine(seed));
    shuffle(zombieTiles.begin(), zombieTiles.end(), std::default_random_engine(seed));

    //So long as we're not infected we can trundle along fine.
    if (GetIsAlive())
    {
        //To start, if we have no zombies and we can actually move just choose a random tile and move to it
        if (zombieTiles.size() == 0 && possibleMoves.size() > 0)
        {
            ret = possibleMoves[0];
        }
        else if (zombieTiles.size() > 0 && possibleMoves.size() > 0)
        {
            //We have a zombie and we need to move away from the damn thing
            //For our purposes the human will only focus on one zombie at a time. If the human gets into a situation
            //where they're completely surrounded and they die, that's too bad for them.
            //So uhh we need to figure out where the zombie is
            position zomPos = zombieTiles[0];
            position curPos = m_CurrentPosition;

            //NOW. We have an idea of where the hell we need to move to get away from the zombie
            if (zomPos.y < curPos.y)
            {
                moveDirHotizontal = direction::RIGHT;
                //Zombie is to the left so we want to go right
            }
            else if (zomPos.y > curPos.y)
            {
                moveDirHotizontal = direction::LEFT;
                //zombie is to the right so we want to move left
            }

            if (zomPos.x > curPos.x)
            {
                moveDirVertical = direction::UP;
                //Our zombie is below SO at the very least we probably want to move up
            }
            else
            {
                moveDirVertical = direction::DOWN;
                //Zombie is above this means we want to move down.
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

    IncreaseMoveCount();
    SetTurnTaken(true);

    return ret;
}

char Human::GetIcon()
{
    return m_Icon;
}
std::vector<Organism::position> Human::GetPossibleMoves()
{
    std::vector<position> moves;
    for (const std::pair<position, char>& tile : GetSurroundingTiles(1))
    {
        if (GAMEBOARD[tile.first.x][tile.first.y] == nullptr)
        {
            moves.push_back(position{ tile.first.x,tile.first.y });
        }
        else if (GAMEBOARD[tile.first.x][tile.first.y]->GetIcon() == 'Z')
        {
            SetIsAlive(false);
        }
    }
    return moves;
}
