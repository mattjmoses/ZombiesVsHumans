#pragma once

#include <chrono>
#include <random>
#include <vector>

class Organism
{
public:

	//The For figuring out where elements are on the map
	enum class direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DEFAULT
	};

	//Using an x and y struct to represent positions on the map
	struct position
	{
		int x = 0;
		int y = 0;
	};

	//Constructor for our organism. Zombies and humans both make use of this
	Organism(std::vector<std::vector<Organism*>>& board) : GAMEBOARD(board) {};

	virtual ~Organism();

	virtual position TakeTurn() = 0;

	virtual char GetIcon() = 0;

	virtual void IncreaseMoveCount() { m_MoveCount++; };

	virtual int GetMoveCount() { return m_MoveCount; };

	virtual void ResetMoveCount() { m_MoveCount = 0; }
	
	virtual std::vector<std::pair<position,char>> GetSurroundingTiles(int range);

	virtual void SetTurnTaken(bool taken) { m_TurnTaken = taken; };

	virtual bool GetTurnTaken() { return m_TurnTaken; };

	//Checking if an organism is alive or dead. If a human is dead it becomes a zombie. Vice versa for a human
	virtual bool GetIsAlive() { return m_IsAlive; };

	virtual void SetIsAlive(bool alive) { m_IsAlive = alive; };

	//Returns a random free tile next to the organism
	virtual position GetFreeAdjacentTile();

	virtual std::vector<position> GetPossibleMoves() = 0;

	//So uh what we'll do is record the character's position when they're spawned then we can just update it as we go.
	virtual void SetPosition(int x, int y) { m_CurrentPosition = position{ x,y }; };

	virtual position GetPosition() { return m_CurrentPosition; };

	//Keeping track of how many moves were made. Both zombies and humans need this
	int m_MoveCount = 0;

	char m_Icon = 'O';

	bool m_TurnTaken = false;

	bool m_IsAlive = true;

	position m_CurrentPosition;

	//Organisms will also need to be able to see their surroundings. SO they will need a reference to the map.
	std::vector<std::vector<Organism*>>& GAMEBOARD;
};
