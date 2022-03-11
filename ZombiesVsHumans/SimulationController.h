#pragma once

#include "Organism.h"
#include "Human.h"
#include "Zombie.h"

#include <iostream>
#include <string>
#include <vector>

class SimulationController
{

public:

	friend std::ostream& operator<<(std::ostream& os, const SimulationController& sim);

	//To begin we have the regular default constructor which uses the default 20x20 dimensions
	SimulationController();

	/// <summary>
	/// Constructor for the simulation controller
	/// </summary>
	/// <param name="width">Map Height</param>
	/// <param name="height">Map Width</param>
	/// <param name="humans">Number of Humans</param>
	/// <param name="zombies">Number of Zombies</param>
	SimulationController(int width, int height, int humans, int zombies);

	~SimulationController();

	void RunThroughBoard();

	std::string GetEndMessage() { return ENDINGMESSAGE; };

	//Checks if an extintion or human victory has occured
	bool CheckEndGameConditions();

private:

	void PopulateBoard();

	//Gets a count of all the humans and zombies on the map
	void TallyPopulation();

	//Returns true if given tile is free
	bool CheckTileIsFree(int x,int y);

	//Default starting values
	int BOARDWIDTH = 20;
	int BOARDHEIGHT = 20;
	int TOTALHUMANS = 10;
	int TOTALZOMBIES = 100;
	int TURNCOUNT = 0;

	const char HUMAN = 'H';
	const char ZOMBIE = 'Z';

	std::string ENDINGMESSAGE = "";

	std::vector<std::vector<Organism*>> m_GameBoard;
};

