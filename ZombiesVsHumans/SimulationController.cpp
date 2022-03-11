#include "SimulationController.h"

#include <algorithm>
#include <random>
#include <windows.h>

HANDLE  colour = GetStdHandle(STD_OUTPUT_HANDLE);

SimulationController::SimulationController()
{
	PopulateBoard();
}

SimulationController::SimulationController(int width, int height, int humans, int zombies):
	BOARDWIDTH(width),
	BOARDHEIGHT(height),
	TOTALHUMANS(humans),
	TOTALZOMBIES(zombies)
{
	PopulateBoard();
}

SimulationController::~SimulationController()
{
	m_GameBoard.clear();
}

void SimulationController::PopulateBoard()
{
	//Initalising the board with null objects based on the height and width
	m_GameBoard = std::vector<std::vector<Organism*>>(BOARDHEIGHT, std::vector<Organism*>(BOARDWIDTH, nullptr));

	//Placing the humans
	std::srand(time(0));
	for (int i = 0; i < TOTALHUMANS; i++)
	{
		Human* human =  new Human(m_GameBoard);

		bool checkTile = true;

		//We make sure the tile isn't occupied
		while (checkTile)
		{
			int x = (rand() % BOARDHEIGHT);
			int y = (rand() % BOARDWIDTH);
			if (CheckTileIsFree(x, y))
			{
				//Recording the initial position of the character for use later
				human->SetPosition(x, y);
				m_GameBoard[x][y] = human;
				checkTile = false;
			}
		}
	}

	for (int i = 0; i < TOTALZOMBIES; i++)
	{
		Zombie* zombie = new Zombie(m_GameBoard);

		bool checkTile = true;

		while (checkTile)
		{
			int x = (rand() % BOARDHEIGHT);
			int y = (rand() % BOARDWIDTH);
			if (CheckTileIsFree(x, y))
			{
				zombie->SetPosition(x, y);
				m_GameBoard[x][y] = zombie;
				checkTile = false;
			}
		}
	}
}

void SimulationController::RunThroughBoard()
{
	//Everybody takes their turn
	for (int i = 0; i < BOARDHEIGHT; i++)
	{
		for (int j = 0; j < BOARDWIDTH; j++)
		{
			if (m_GameBoard[i][j] != nullptr)
			{
				if (!m_GameBoard[i][j]->GetTurnTaken())
				{
					Organism::position nextPosition = m_GameBoard[i][j]->TakeTurn();

					//We'll need to refactor this business.
					if (m_GameBoard[i][j]->GetIcon() == 'H')
					{
						//To begin we should check if we're alive or dead
						if (m_GameBoard[i][j]->GetIsAlive())
						{
							//First we make a new human
							if (m_GameBoard[i][j]->GetMoveCount() > 8)
							{
								m_GameBoard[i][j]->ResetMoveCount();

								Organism::position newPosition = m_GameBoard[i][j]->GetFreeAdjacentTile();
								Organism::position curPosition = m_GameBoard[i][j]->GetPosition();

								//We want to check that we can make a new human. No making humans on the same space
								//we're currently on
								if ((newPosition.x != curPosition.x) || (newPosition.y != curPosition.y))
								{
									Human* human = new Human(m_GameBoard);
									human->SetPosition(newPosition.x, newPosition.y);
									m_GameBoard[newPosition.x][newPosition.y] = human;
								}
							}

							//Next we move.
							m_GameBoard[i][j]->SetPosition(nextPosition.x, nextPosition.y);
							m_GameBoard[nextPosition.x][nextPosition.y] = m_GameBoard[i][j];
							m_GameBoard[i][j] = nullptr;
							//And that's basically it for the human
						}
						else
						{
							//When bitten the human changes into a zombie
							Zombie* zombie = new Zombie(m_GameBoard);
							zombie->SetPosition(i, j);
							m_GameBoard[i][j] = zombie;
						}
					}
					else if (m_GameBoard[i][j]->GetIcon() == 'Z')
					{
						//So the alive and dead scenarios for humans and zombies are slightly different..
						if (m_GameBoard[i][j]->GetIsAlive())
						{
							if (m_GameBoard[i][j]->GetMoveCount() > 8)
							{
								m_GameBoard[i][j]->ResetMoveCount();

								Organism::position newPosition = m_GameBoard[i][j]->GetFreeAdjacentTile();
								Organism::position curPosition = m_GameBoard[i][j]->GetPosition();
								//We make a new zombie
								if ((newPosition.x != curPosition.x) || (newPosition.y != curPosition.y))
								{
									Zombie* zombie = new Zombie(m_GameBoard);
									zombie->SetPosition(newPosition.x, newPosition.y);
									m_GameBoard[newPosition.x][newPosition.y] = zombie;
								}
							}

							m_GameBoard[i][j]->SetPosition(nextPosition.x, nextPosition.y);
							m_GameBoard[nextPosition.x][nextPosition.y] = m_GameBoard[i][j];
							m_GameBoard[i][j] = nullptr;
						}
						else
						{
							//When a zombie starves it reverts to being a human
							Human* human = new Human(m_GameBoard);
							human->SetPosition(i, j);
							m_GameBoard[i][j] = human;
						}
					}
				}
			}
		}
	}
	
	//Tally the population on the map
	TallyPopulation();

	//Once we run through the board we reset everybody's turns
	for (int i = 0; i < BOARDHEIGHT; i++)
	{
		for (int j = 0; j < BOARDWIDTH; j++)
		{
			if (m_GameBoard[i][j] != nullptr)
			{
				m_GameBoard[i][j]->SetTurnTaken(false);
			}
		}
	}

	//Keep track of how many turns happened
	TURNCOUNT++;
}

void SimulationController::TallyPopulation()
{
	TOTALHUMANS = 0;
	TOTALZOMBIES = 0;
	for (int i = 0; i < m_GameBoard.size(); i++)
	{
		for (int j = 0; j < m_GameBoard[0].size(); j++)
		{
			if (m_GameBoard[i][j] != nullptr)
			{
				if (m_GameBoard[i][j]->GetIcon() == 'H')
				{
					TOTALHUMANS++;
				}
				else if (m_GameBoard[i][j]->GetIcon() == 'Z')
				{
					TOTALZOMBIES++;
				}
			}
		}
	}
}

bool SimulationController::CheckEndGameConditions()
{
	bool ret = true;
	if (TOTALZOMBIES <= 0)
	{
		ENDINGMESSAGE = "Zombies defeated by humans in " + std::to_string(TURNCOUNT) + " turns";
		ret = false;
	}
	else if (TOTALHUMANS <= 0)
	{
		ENDINGMESSAGE = "Humans defeated by zombies in " + std::to_string(TURNCOUNT) + " turns";
		ret = false;
	}

	return ret;
}

bool SimulationController::CheckTileIsFree(int x, int y)
{
	return m_GameBoard[x][y] == nullptr;
}

std::ostream& operator<<(std::ostream& os, const SimulationController& sim)
{
	for (int i = 0; i < sim.BOARDHEIGHT; i++)
	{
		SetConsoleTextAttribute(colour, 2);
		os << "*";
		for (int j = 0; j < sim.BOARDWIDTH; j++)
		{
			if (sim.m_GameBoard[i][j] == nullptr)
			{
				os << " " << "  ";
			}
			else
			{
				if (sim.m_GameBoard[i][j]->GetIcon() == 'H')
				{
					SetConsoleTextAttribute(colour, 11);
					os << sim.HUMAN << "  ";
				}
				else if(sim.m_GameBoard[i][j]->GetIcon() == 'Z')
				{
					SetConsoleTextAttribute(colour, 4);
					os << sim.ZOMBIE << "  ";
				}
			}
		}
		SetConsoleTextAttribute(colour, 2);
		os << "*";
		os << std::endl;
	}
	SetConsoleTextAttribute(colour, 9);
	os << "Total Humans: "
		<< sim.TOTALHUMANS << "     "
		<< "Total Zombies: "
		<< sim.TOTALZOMBIES << "    Turns: "
		<< sim.TURNCOUNT << std::endl;

	return os;
}
