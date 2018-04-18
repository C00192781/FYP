#include <iostream>
#include <fstream>
#include <string>
#include <utility>  
#include <map> 

#include "Graph.h"
#include "SFML\Graphics.hpp"

//#include <stdio.h>
//#include <unistd.h>


using namespace std;
using std::pair;

#define screenWidth 1080
#define screenHeight 720

//typedef GraphArc<std::string, int> Arc;
//typedef GraphNode<pair<std::string, int>, int> Node;

int main(int argc, char *argv[]) {

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Project");
	sf::Clock clock;
	const int graphSize = 51;
	Graph graph(graphSize);
	
	std::string nodeLabel;

	/*map<string, int> map;*/

	int i = 0;
	ifstream myfile;
	float positionX = 0;
	float positionY = 0;
	myfile.open ("nodes.txt");

	while ( myfile >> nodeLabel >> positionX >> positionY) {
	// giving each node a string and an integer we'll change later
		sf::Vector2f waypoint = { positionX, positionY };
		graph.addNode(pair<string,int>(nodeLabel, 0), pair<string, int>(nodeLabel, 0), false, i, waypoint);
		i++;
	}
	myfile.close();

	myfile.open("arcs.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
	}
    myfile.close();


	/// FOR DRAWING NODES + EDGES
	sf::CircleShape nodes[graphSize];
	sf::Color circleColour(200, 200, 200);


	
	sf::Text text[graphSize];

	sf::Font* font = new sf::Font();
	if (!font->loadFromFile("calibri.ttf"))
	{
		std::cout << "Font failed to Load" << std::endl;
	}

	ifstream nodesFile;

	int index = 0;
	int nodeNum = 0;

	// positions we'll draw nodes at
	int xPos;
	int yPos;

	// open text file extract positions for different nodes
	nodesFile.open("drawNodes.txt");
	while (nodesFile >> nodeNum >> xPos >> yPos)
	{
		std::string textNodeNum = std::to_string(nodeNum);
		text[index] = sf::Text(textNodeNum, *font);
		text[index].setPosition(sf::Vector2f((xPos * 0.9) + 7, (yPos * 0.9)));
		text[index].setFillColor(sf::Color::White);

		nodes[index].setRadius(20);
		nodes[index].setFillColor(sf::Color::Red);
		nodes[index].setOutlineColor(sf::Color::Blue);
		nodes[index].setOutlineThickness(4);
		nodes[index].setPosition((xPos * 0.9), (yPos * 0.9));

		index++;
	}
	nodesFile.close();

	
	//set up a path
	std::vector<GraphNode* > path;


	//// set up a pointer to the Goal Node
	//Node *pCurrent = graph.nodeArray()[goal];
	//// used to output path + path values
	//while (pCurrent != graph.nodeArray()[start])
	//{
	//	std::cout << "Node: " << pCurrent->data().first << std::endl; 
	//	std::cout << "Distance: " << pCurrent->data().second << std::endl;

	//	std::cout << "Estimate: " << pCurrent->getEstimate() << std::endl;
	//	pCurrent = pCurrent->getPrevious();
	//}


	//if(pCurrent == graph.nodeArray()[start])
	//{
	//	/*path.push_back(pCurrent->data().first);
	//	cout << path.front() << endl;*/
	//}


	//for (int index = path.size() - 1; index > -1; index--)
	//{
	//	std::cout << path.at(i)->data().first << path.at(i)->data().second;
	//	if (path.at(i) == graph.nodeArray()[goal])
	//	{
	//		std::cout << std::endl;
	//	}
	//}

	sf::VertexArray edges(sf::LinesStrip, 72);
	sf::VertexArray edge(sf::LinesStrip, 2);
	

	int edgeIndex;
	ifstream edgesFile;
	string edgeString;

	int posX, posY;
	int timer = 0;
	bool startMessage = false;
	bool searchInitialized = false;
	bool obstacleCondition = false;
	bool compute = true;
	std::string addOrRemove = " ";

	int start = 0;
	int goal = 16;
	int obstacle = 0;

	float inflation = 3;
	bool wait = false;
	bool deliberation = false;

	int cost = 0;
	int publishedCost = 0;

	int searchType = 2;
	
	while (window.isOpen())
	{
		//if (timer < 3100)
			timer++;

		// draw nodes
		for (int index = 0; index < graphSize; index++)
		{
			window.draw(nodes[index]);
			window.draw(text[index]);
		}
		//window.draw(edges);


		// MAIN
		if (startMessage == false)
		{
			if (timer > 200)
			{
				cout << "Choose Search Algorithm" << endl;
				cout << "For LPA* - type 0" << endl;
				cout << "For AD* - type 1" << endl;
				cin >> searchType;

				if (searchType == 1)
				{
					cout << "Anytime Dynamic A*" << endl;
					std::cout << "Input Starting node + Goal node " << std::endl;
					std::cout << std::endl;
					startMessage = true;
				}

				if (searchType == 0)
				{
					cout << "Lifelong Planning A*" << endl;
					std::cout << "Input Starting node + Goal node " << std::endl;
					std::cout << std::endl;
					startMessage = true;
				}
			}		
		}


		if (searchType == 0)
		{
			if (timer > 1000 && searchInitialized == false)
			{
				cout << "Starting point: " << endl;
				cin >> start;
				cout << "Goal: " << endl;
				cin >> goal;
				// Initialize LPA*
				graph.LPAStarInitialize(graph.nodeArray()[start], graph.nodeArray()[goal]);
				searchInitialized = true;
			}

			if (searchInitialized == true)
			{
				if (compute == true)
				{
					//graph.SetObstacle(14, true, start);
					graph.ComputeShortestPath(graph.nodeArray()[start], graph.nodeArray()[goal]);
					compute = true;
					obstacleCondition = false;
				}
				if (obstacleCondition == false)
				{
					std::cout << "Would you like to add an obstacle or remove one? A/R or Add/Remove" << std::endl;
					cin >> addOrRemove;
					std::cout << "Type in the Node that you want to be changed" << std::endl;
					cin >> obstacle;

					if (addOrRemove == "ADD" || addOrRemove == "add" || addOrRemove == "Add" || addOrRemove == "A" || addOrRemove == "a")
					{
						graph.SetObstacle(obstacle, true, start);
					}
					if (addOrRemove == "REMOVE" || addOrRemove == "remove" || addOrRemove == "Remove" || addOrRemove == "R" || addOrRemove == "r")
					{
						graph.SetObstacle(obstacle, false, start);
					}
					//graph.UpdateVertex(graph.nodeArray()[stoi(obstacleQuestion)], graph.nodeArray()[start]);
					obstacleCondition = true;
					compute = true;
				}
			}
		}



		if (searchType == 1)
		{
			if (timer > 1000 && searchInitialized == false)
			{
				cout << "Starting point: " << endl;
				cin >> start;
				cout << "Goal: " << endl;
				cin >> goal;
				cout << "Inflation: " << endl;
				// Initialize AD*
				std::cout << "Initializing AD*" << std::endl;
				graph.ADStarInitialize(graph.nodeArray()[start], graph.nodeArray()[goal], path, inflation);
				cost = graph.ComputeOrImprovePath(graph.nodeArray()[start], graph.nodeArray()[goal]);
				publishedCost = cost;
				clock.restart();
				searchInitialized = true;
			}


			if (searchInitialized == true)
			{
				sf::Time elapsed = clock.getElapsedTime();
				float sec = elapsed.asSeconds();
				/*if (deliberation == false)
				{ */
				//if (sec >= 1 && )
				bool edgeCosts = false;
				//std::cout << "Seconds passed: " << sec << std::endl;
			/*	if (sec >= 1)
				{*/
				if (timer >= 2500 && sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
				{
					//std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
					//sf::Vector2f mousePos = sf::Vector2f{ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) };
					for (int i = 0; i < graphSize; i++)
					{
						/*if (nodes[i].getGlobalBounds().contains(mousePos))
						{
						std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;

						}*/
						if (nodes[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
						{
							if (graph.nodeArray()[i] != nullptr)
							{
								std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
								graph.SetObstacle(i, true, start);
								edgeCosts = true;
								wait = false;
							}
							else
							{
								std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
								graph.SetObstacle(i, false, start);
								edgeCosts = true;
								wait = false;
							}
						}
					}
					timer = 0;
				}
				if (wait == false)
				{
					if (edgeCosts == true)
					{
						inflation = graph.getInflation();
						inflation+= 5;
						graph.setInflation(inflation);
						edgeCosts = false;
					}
					else if (graph.getInflation() > 1)
					{
						inflation = graph.getInflation();
						inflation--;
						graph.setInflation(inflation);
					}

					graph.MoveStates();
					cost = graph.ComputeOrImprovePath(graph.nodeArray()[start], graph.nodeArray()[goal]);

					publishedCost = cost;

					if (graph.getInflation() <= 1)
					{
						wait = true;
						//clock.restart();
					}
				}
				//if (compute == true)
				//{
				//	//graph.SetObstacle(14, true, start);
				//	graph.setInflation(inflation);
				//	graph.ComputeOrImprovePath(graph.nodeArray()[start], graph.nodeArray()[goal]);
				//	compute = true;
				//	obstacleCondition = false;
				//}
				//if (obstacleCondition == false)
				//{
				//	std::cout << "Would you like to add an obstacle or remove one? A/R or Add/Remove" << std::endl;
				//	cin >> addOrRemove;
				//	std::cout << "Type in the Node that you want to be changed" << std::endl;
				//	cin >> obstacle;

				//	if (addOrRemove == "ADD" || addOrRemove == "add" || addOrRemove == "Add" || addOrRemove == "A" || addOrRemove == "a")
				//	{
				//		graph.SetObstacle(obstacle, true, start);
				//	}
				//	if (addOrRemove == "REMOVE" || addOrRemove == "remove" || addOrRemove == "Remove" || addOrRemove == "R" || addOrRemove == "r")
				//	{
				//		graph.SetObstacle(obstacle, false, start);
				//	}
				//	//graph.UpdateVertex(graph.nodeArray()[stoi(obstacleQuestion)], graph.nodeArray()[start]);
				//	inflation--;
				//	obstacleCondition = true;
				//	compute = true;
				//}
			/*}*/
			}
		}
	

		
		//// MAIN
		//if (startMessage == false)
		//{
		//	cout << "Lifelong Planning A*" << endl;
		//	std::cout << "Input Starting node + Goal node " << std::endl;
		//	std::cout << std::endl;
		//	startMessage = true;
		//}

		//if (timer > 1000 && searchInitialized == false)
		//{
		//	cout << "Starting point: " << endl;
		//	cin >> start;
		//	cout << "Goal: " << endl;
		//	cin >> goal;
		//	// Initialize LPA*
		//	graph.LPAStarInitialize(graph.nodeArray()[start], graph.nodeArray()[goal]);
		//	searchInitialized = true;
		//}

		//if (searchInitialized == true)
		//{
		//	if (compute == true)
		//	{
		//		//graph.SetObstacle(14, true, start);
		//		graph.ComputeShortestPath(graph.nodeArray()[start], graph.nodeArray()[goal]);
		//		compute = true;
		//		obstacleCondition = false;
		//	}
		//	if (obstacleCondition == false)
		//	{
		//		std::cout << "Would you like to add an obstacle or remove one? A/R or Add/Remove" << std::endl;
		//		cin >> addOrRemove;
		//		std::cout << "Type in the Node that you want to be changed" << std::endl;
		//		cin >> obstacle;

		//		if (addOrRemove == "ADD" || addOrRemove == "add" || addOrRemove == "Add" || addOrRemove == "A" || addOrRemove == "a")
		//		{
		//			graph.SetObstacle(obstacle, true, start);
		//		}
		//		if (addOrRemove == "REMOVE" || addOrRemove == "remove" || addOrRemove == "Remove" || addOrRemove == "R" || addOrRemove == "r")
		//		{
		//			graph.SetObstacle(obstacle, false, start);
		//		}
		//		//graph.UpdateVertex(graph.nodeArray()[stoi(obstacleQuestion)], graph.nodeArray()[start]);
		//		obstacleCondition = true;
		//		compute = true;
		//	}
		//}
		window.display();
	}

	system("PAUSE");
}


void LPAStar()
{

}

void ADStar()
{
	
}