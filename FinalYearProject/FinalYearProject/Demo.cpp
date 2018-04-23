#include "Demo.h"

Demo::Demo()
{

}

Demo::~Demo()
{
}

void Demo::Initialize()
{
	window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Project");
	isRunning = true;

	font = new sf::Font();
	if (!font->loadFromFile("calibri.ttf"))
	{
		std::cout << "Font failed to Load" << std::endl;
	}

	graphSize = 27;
	graph = new Graph(graphSize);
	logger = new Logging();
	SetUpNodes();
	SetUpArcs();
	VisualGraphSetUp();
	BackgroundGrid();
	//logger->clearCSVFile();
//	clearCSVFile();
	logger->Initialize();

	int startX = graph->nodeArray()[start]->getWaypoint().x;
	int startY = graph->nodeArray()[start]->getWaypoint().y;

	unit = new Unit(startX, startY, 4, sf::Color::White);

	timer = 0;
	startMessage = false;
	searchInitialized = false;
	obstacleCondition = false;
	compute = true;
	addOrRemove = " ";

	start = 0;
	goal = 16;
	obstacle = 0;

	inflation = 2.5;
	wait = false;
	deliberation = false;

	cost = 0;
	publishedCost = 0;

	searchType = 5;

	adStarSearchComplete = false;

}

void Demo::SetUpNodes()
{
	int i = 0;
	float positionX = 0;
	float positionY = 0;
	myfile.open("nodes.txt");

	while (myfile >> nodeLabel >> positionX >> positionY) 
	{
		// giving each node a string and an integer we'll change later
		sf::Vector2f waypoint = { positionX, positionY };
		graph->addNode(pair<string, int>(nodeLabel, 0), pair<string, int>(nodeLabel, 0), false, i, waypoint);
		i++;
	}
	myfile.close();
}

void Demo::SetUpArcs()
{
	myfile.open("arcs.txt");

	int from, to, weight;
	while (myfile >> from >> to >> weight) 
	{
		graph->addArc(from, to, weight);
	}
	myfile.close();
}



void Demo::VisualGraphSetUp()
{
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
		sf::Text text = sf::Text(textNodeNum, *font);
		text.setPosition(sf::Vector2f((xPos), (yPos)));
		text.setFillColor(sf::Color::Black);

		texts.push_back(text);

		sf::RectangleShape node;
		node.setPosition(sf::Vector2f(xPos, yPos));
		node.setSize(sf::Vector2f(49, 49));
		node.setFillColor(sf::Color::Green);
		/*node.setRadius(20);
		node.setFillColor(sf::Color::Red);
		node.setOutlineColor(sf::Color::Blue);*/
		//node.setOutlineThickness(4);
		//node.setPosition((xPos), (yPos));

		nodes.push_back(node);

		index++;
	}
	nodesFile.close();

	//std::reverse(nodes.begin(), nodes.end());
	//std::reverse(texts.begin(), texts.end());
}

void Demo::BackgroundGrid()
{
	for (int x = 0; x < 21; x++)
	{
		for (int y = 0; y < 14; y++)
		{
			sf::RectangleShape rectangle;
			rectangle.setPosition(sf::Vector2f(x * 50, y * 50));
			rectangle.setSize(sf::Vector2f(49, 49));
			rectangle.setFillColor(sf::Color::Blue);
			grid.push_back(rectangle);
		}
	}
}

void Demo::Render()
{
	window->clear();
	for (int index = 0; index < grid.size(); index++)
	{
		window->draw(grid.at(index));
	}
	for (int index = 0; index < graphSize; index++)
	{
		window->draw(nodes.at(index));
		window->draw(texts.at(index));
	}
	unit->Draw(window);
	

	window->display();
}

void Demo::Update()
{
	bool check = false;
	while (window->isOpen())
	{
		//if (timer < 3100)
		timer++;


		//// draw nodes
		//for (int index = 0; index < graphSize; index++)
		//{
		//	window->draw(nodes.at(index));
		//	window->draw(texts.at(index));
		//}

		//window.draw(edges);


		// MAIN
		if (startMessage == false)
		{
			if (timer > 200)
			{
				cout << "Choose Search Algorithm" << endl;
				cout << "For LPA* - type 0" << endl;
				cout << "For AD* - type 1" << endl;
				cout << "For A* - type 2" << endl;
				cin >> searchType;

				if (searchType == 1)
				{
					cout << "Anytime Dynamic A*" << endl;
					std::cout << "Input Starting node + Goal node " << std::endl;
					std::cout << std::endl;
					startMessage = true;
				}

				else if (searchType == 0)
				{
					cout << "Lifelong Planning A*" << endl;
					std::cout << "Input Starting node + Goal node " << std::endl;
					std::cout << std::endl;
					startMessage = true;
				}
				else if (searchType == 2)
				{
					cout << "A*" << endl;
					std::cout << "Input Starting node + Goal node " << std::endl;
					std::cout << std::endl;
					startMessage = true;
				}
			}
		}

		unit->Move();

		if (searchType == 0)
		{
			LPAStar();
			unit->setSearchType("LPA*");
		}
		else if (searchType == 1)
		{
			ADStar();
			unit->setSearchType("AD*");
		}
		else if (searchType == 2)
		{
			AStar();
			unit->setSearchType("A*");
		}

		Render();
	}
}

void Demo::LPAStar()
{
	if (timer > 1000 && searchInitialized == false)
	{
		cout << "Starting point: " << endl;
		cin >> start;
		cout << "Goal: " << endl;
		cin >> goal;
		// Initialize LPA*
		graph->LPAStarInitialize(graph->nodeArray()[start], graph->nodeArray()[goal]);
		clock.restart();
		searchInitialized = true;
	}

	if (searchInitialized == true)
	{
		if (wait == false)
		{
			//graph.SetObstacle(14, true, start);
			clock.restart();
			graph->ComputeShortestPath(graph->nodeArray()[start], graph->nodeArray()[goal]);
			unit->SetPath(graph->getPath(), graph->nodeArray()[start]->getWaypoint().x, graph->nodeArray()[start]->getWaypoint().y);
			sf::Time elapsed = clock.getElapsedTime();
			float sec = elapsed.asMilliseconds();

			logger->LogLineToCSVFile("LPA*", start, goal, sec, graph->getPathLength(), graph->getCellExpansions(), NULL);

			compute = false;
			wait = true;
		}
		if (wait == true)
		{
			if (timer >= 100 && sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
			{
				std::cout << "true" << std::endl;
				//std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
				//sf::Vector2f mousePos = sf::Vector2f{ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) };
				for (int i = 0; i < graphSize; i++)
				{
					/*if (nodes[i].getGlobalBounds().contains(mousePos))
					{
					std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
					}*/
					if (nodes.at(i).getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
					{
						if (graph->nodeArray()[i] != nullptr)
						{
							std::cout << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;
							//graph->SetObstacle(stoi(texts.at(i).getString().toAnsiString()), true, start);
							nodes.at(i).setFillColor(sf::Color::Red);
							/*clock.restart();*/
							graph->SetObstacle(i, true, start);
							wait = false;
						}
						else
						{
							std::cout << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;
							//graph->SetObstacle(stoi(texts.at(i).getString().toAnsiString()), false, start);
							nodes.at(i).setFillColor(sf::Color::Green);
							//clock.restart();
							graph->SetObstacle(i, false, start);
							wait = false;
						}
					}
				}
				timer = 0;
			}
			//std::cout << "Would you like to add an obstacle or remove one? A/R or Add/Remove" << std::endl;
			//cin >> addOrRemove;
			//std::cout << "Type in the Node that you want to be changed" << std::endl;
			//cin >> obstacle;

			//if (addOrRemove == "ADD" || addOrRemove == "add" || addOrRemove == "Add" || addOrRemove == "A" || addOrRemove == "a")
			//{
			//	graph->SetObstacle(obstacle, true, start);
			//}
			//if (addOrRemove == "REMOVE" || addOrRemove == "remove" || addOrRemove == "Remove" || addOrRemove == "R" || addOrRemove == "r")
			//{
			//	graph->SetObstacle(obstacle, false, start);
			//}
			////graph.UpdateVertex(graph.nodeArray()[stoi(obstacleQuestion)], graph.nodeArray()[start]);
			//obstacleCondition = true;
			//compute = true;
		}
	}
}

void Demo::ADStar()
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
		graph->ADStarInitialize(graph->nodeArray()[start], graph->nodeArray()[goal], path, inflation);
		clock.restart();
		cost = graph->ComputeOrImprovePath(graph->nodeArray()[start], graph->nodeArray()[goal]);
		sf::Time elapsed = clock.getElapsedTime();
		float sec = elapsed.asMilliseconds();
		logger->LogLineToCSVFile("AD*", start, goal, sec, graph->getPathLength(), graph->getCellExpansions(), graph->getInflationIteration());
		unit->SetPath(graph->getPath(), graph->nodeArray()[start]->getWaypoint().x, graph->nodeArray()[start]->getWaypoint().y);

		searchInitialized = true;
	}


	if (searchInitialized == true)
	{
		/*sf::Time elapsed = clock.getElapsedTime();
		float sec = elapsed.asMilliseconds();*/
		/*if (deliberation == false)
		{ */
		//if (sec >= 1 && )
		bool edgeCosts = false;
		//std::cout << "Seconds passed: " << sec << std::endl;
		/*	if (sec >= 1)
		{*/

		/*cost = graph.ComputeOrImprovePath(graph.nodeArray()[start], graph.nodeArray()[goal]);
		publishedCost = cost;*/

		if (graph->getInflation() <= 1)
		{
			wait = true;

			if (adStarSearchComplete == false)
			{
				/*sf::Time elapsed = clock.getElapsedTime();
				float sec = elapsed.asMilliseconds();

				std::cout << "SEC: " << sec << std::endl;

				logger->LogLineToCSVFile("AD*", start, goal, sec, graph->getPathLength(), graph->getCellExpansions(), graph->getInflation());*/
			}

			adStarSearchComplete = true;

			if (timer >= 100 && sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
			{
				std::cout << "true" << std::endl;
				//std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
				//sf::Vector2f mousePos = sf::Vector2f{ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) };
				for (int i = 0; i < graphSize; i++)
				{
					/*if (nodes[i].getGlobalBounds().contains(mousePos))
					{
					std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
					}*/
					if (nodes.at(i).getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
					{
						if (graph->nodeArray()[i] != nullptr)
						{

							std::cout << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;
							//graph->SetObstacle(stoi(texts.at(i).getString().toAnsiString()), true, start);
							nodes.at(i).setFillColor(sf::Color::Red);
							adStarSearchComplete = false;
							graph->SetObstacle(i, true, start);
							edgeCosts = true;
							wait = false;
						}
						else
						{
							std::cout << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;
							//graph->SetObstacle(stoi(texts.at(i).getString().toAnsiString()), false, start);
							nodes.at(i).setFillColor(sf::Color::Green);
							adStarSearchComplete = false;
							graph->SetObstacle(i, false, start);
							edgeCosts = true;
							wait = false;
						}
					}
				}
				timer = 0;
			}
		}

		if (wait == false)
		{

			if (edgeCosts == true)
			{
				inflation = graph->getInflation();
				inflation++;
				graph->setInflation(inflation);
			}
			else if (graph->getInflation() > 1)
			{
				inflation = graph->getInflation();
				inflation--;
				if (inflation < 1)
				{
					inflation = 1;
				}
				graph->setInflation(inflation);
			}
			edgeCosts = false;


			graph->MoveStates();
			

			//std::cout << "SEC: " << sec << std::endl;
			clock.restart();
			cost = graph->ComputeOrImprovePath(graph->nodeArray()[start], graph->nodeArray()[goal]);
			sf::Time elapsed = clock.getElapsedTime();
			float sec = elapsed.asMilliseconds();
			logger->LogLineToCSVFile("AD*", start, goal, sec, graph->getPathLength(), graph->getCellExpansions(), graph->getInflationIteration());
			std::cout << graph->getInflation() << std::endl;
			std::cout << std::endl;
			unit->SetPath(graph->getPath(), graph->nodeArray()[start]->getWaypoint().x, graph->nodeArray()[start]->getWaypoint().y);


			publishedCost = cost;

		}



		/*		if (wait == false)
		{
		if (edgeCosts == true)
		{
		inflation = graph.getInflation();
		inflation += 1;
		graph.setInflation(inflation);
		edgeCosts = false;
		}
		else if (graph.getInflation() > 1)
		{
		inflation = graph.getInflation();
		inflation--;
		graph.setInflation(inflation);
		}*/

		/*	graph.MoveStates();
		cost = graph.ComputeOrImprovePath(graph.nodeArray()[start], graph.nodeArray()[goal]);
		publishedCost = cost;*/

		//if (graph.getInflation() <= 1)
		//{
		//	wait = true;
		//	//clock.restart();
		//}
		//}
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
		//}

	
	}
}

void Demo::AStar()
{
	if (timer > 1000 && searchInitialized == false)
	{
		cout << "Starting point: " << endl;
		cin >> start;
		cout << "Goal: " << endl;
		cin >> goal;
		// Initialize A*
		//clock.restart();
		//graph->AStar(graph->nodeArray()[start], graph->nodeArray()[goal]);
		//unit->SetPath(graph->getPath(), graph->nodeArray()[start]->getWaypoint().x, graph->nodeArray()[start]->getWaypoint().y);
		//sf::Time elapsed = clock.getElapsedTime();
		//float sec = elapsed.asMilliseconds();

		//clock.restart();
		searchInitialized = true;
	}

	if (searchInitialized == true)
	{
		if (wait == false)
		{
			//graph.SetObstacle(14, true, start);
			clock.restart();
			graph->AStar(graph->nodeArray()[start], graph->nodeArray()[goal]);
			sf::Time elapsed = clock.getElapsedTime();
			float sec = elapsed.asMilliseconds();
			unit->SetPath(graph->getPath(), graph->nodeArray()[start]->getWaypoint().x, graph->nodeArray()[start]->getWaypoint().y);

			logger->LogLineToCSVFile("A*", start, goal, sec, graph->getPathLength(), graph->getCellExpansions(), NULL);

			compute = false;
			wait = true;
		}
		if (wait == true)
		{
			if (timer >= 100 && sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
			{
				std::cout << "true" << std::endl;
				//std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
				//sf::Vector2f mousePos = sf::Vector2f{ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) };
				for (int i = 0; i < graphSize; i++)
				{
					/*if (nodes[i].getGlobalBounds().contains(mousePos))
					{
					std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
					}*/
					if (nodes.at(i).getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
					{
						if (graph->nodeArray()[i] != nullptr)
						{

							std::cout << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;
							//graph->SetObstacle(stoi(texts.at(i).getString().toAnsiString()), true, start);
							nodes.at(i).setFillColor(sf::Color::Red);
							graph->SetObstacle(i, true, start);
							wait = false;
						}
						else
						{
							std::cout << sf::Mouse::getPosition(*window).x << " " << sf::Mouse::getPosition(*window).y << std::endl;
							//graph->SetObstacle(stoi(texts.at(i).getString().toAnsiString()), false, start);
							nodes.at(i).setFillColor(sf::Color::Green);
							graph->SetObstacle(i, false, start);
							wait = false;
						}
					}
				}
				timer = 0;
			}
			//std::cout << "Would you like to add an obstacle or remove one? A/R or Add/Remove" << std::endl;
			//cin >> addOrRemove;
			//std::cout << "Type in the Node that you want to be changed" << std::endl;
			//cin >> obstacle;

			//if (addOrRemove == "ADD" || addOrRemove == "add" || addOrRemove == "Add" || addOrRemove == "A" || addOrRemove == "a")
			//{
			//	graph->SetObstacle(obstacle, true, start);
			//}
			//if (addOrRemove == "REMOVE" || addOrRemove == "remove" || addOrRemove == "Remove" || addOrRemove == "R" || addOrRemove == "r")
			//{
			//	graph->SetObstacle(obstacle, false, start);
			//}
			////graph.UpdateVertex(graph.nodeArray()[stoi(obstacleQuestion)], graph.nodeArray()[start]);
			//obstacleCondition = true;
			//compute = true;
		}
	}
}


