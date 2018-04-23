#pragma once

#ifndef DEMO_H
#define DEMO_H

#include <iostream>
#include <fstream>
#include <string>
#include <utility>  
#include <map> 

#include "Graph.h"
#include "Unit.h"
#include "Logging.h"
#include "SFML\Graphics.hpp"
#include <Windows.h>

using namespace std;
using std::pair;

#define screenWidth 1080
#define screenHeight 720

// This is the class that utilizes most of the other classes in the project

class Demo
{
public:
	Demo();
	~Demo();

	// Set up the graph and its visual representation
	void Initialize();
	void SetUpNodes();
	void SetUpArcs();
	void VisualGraphSetUp();
	void BackgroundGrid();

	void Render();
	void Update();

	// algorithms ran within these functions
	// Essentially each algorithms Main Loop
	void LPAStar();
	void ADStar();
	void AStar();

	bool IsRunning()
	{
		return isRunning;
	};
	

private:

	bool isRunning;

	sf::RenderWindow *window;
	// used for measuring time for data logging
	sf::Clock clock;
	int graphSize;

	Graph *graph;
	std::string nodeLabel;
	ifstream myfile;
	ifstream nodesFile;

	// used for visual representation
	std::vector<sf::RectangleShape> nodes;
	std::vector<sf::Text> texts;
	std::vector<GraphNode*> path;
	std::vector<sf::RectangleShape> grid;
	
	sf::Font* font;

	Unit *unit;
	Logging* logger; 

	std::string addOrRemove = " ";
	
	// timer used primarily to control node selection
	int timer;
	bool startMessage = false;
	bool searchInitialized = false;
	bool obstacleCondition = false;
	bool compute = true;

	int start;
	int goal;
	int obstacle;

	int cost;
	int publishedCost;

	float inflation;
	bool wait;
	bool deliberation;

	// determines which search to run
	int searchType;

	bool adStarSearchComplete = false;


	// used to keep track of number of obstacles on graph
	int obstacleCounter;


	// used to check if user has entered start cell and goal cell that exist in the graph
	bool successfulInput; 

};
#endif