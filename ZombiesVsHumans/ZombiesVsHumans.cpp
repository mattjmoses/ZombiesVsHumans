// ZombiesVsHumans.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SimulationController.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <windows.h>

int main()
{
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);

	//Sizing the console
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 500, 430, true);
	int boardWidth = 20;
	int boardHeight = 20;
	int numHumans = 20;
	int numZombies = 10;

	SimulationController simControl = SimulationController(boardWidth, boardHeight, numHumans, numZombies);

	while (simControl.CheckEndGameConditions())
	{
		simControl.RunThroughBoard();
		std::cout << simControl;
		std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
	}
	std::cout << std::endl;
	std::cout << simControl.GetEndMessage();
	std::cout << std::endl;
}
