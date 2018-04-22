#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <utility>  
#include <map> 

#include "Graph.h"
#include "Unit.h"
#include "SFML\Graphics.hpp"

//#include <stdio.h>
//#include <unistd.h>


using namespace std;
using std::pair;

#define screenWidth 1080
#define screenHeight 720

class Demo
{
public:
	Demo();
	~Demo();

	void Initialize();
	void SetUpNodes();
	void SetUpArcs();
	void VisualGraphSetUp();
	void BackgroundGrid();

	void Render();
	void Update();

	void LPAStar();
	void ADStar();
	void AStar();

	void clearCSVFile();

	bool IsRunning()
	{
		return isRunning;
	};
	

private:

	bool isRunning;

	sf::RenderWindow *window;
	sf::Clock clock;
	int graphSize;

	Graph *graph;

	std::string nodeLabel;
	ifstream myfile;
	ifstream nodesFile;

	std::vector<sf::RectangleShape> nodes;
	std::vector<sf::Text> texts;

	sf::Font* font;

	std::vector<GraphNode*> path;

	Unit *unit;

	std::vector<sf::RectangleShape> grid;



	int timer = 0;
	bool startMessage = false;
	bool searchInitialized = false;
	bool obstacleCondition = false;
	bool compute = true;
	std::string addOrRemove = " ";

	int start = 0;
	int goal = 16;
	int obstacle = 0;

	float inflation = 2.5;
	bool wait = false;
	bool deliberation = false;

	int cost = 0;
	int publishedCost = 0;

	int searchType = 2;



};
