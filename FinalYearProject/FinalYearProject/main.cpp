#include <iostream>
#include <fstream>
#include <string>
#include <utility>  
#include <map> 

#include "Graph.h"
#include "SFML\Graphics.hpp"


using namespace std;
using std::pair;

#define screenWidth 1080
#define screenHeight 720

typedef GraphArc<std::string, int> Arc;
typedef GraphNode<pair<std::string, int>, int> Node;

int main(int argc, char *argv[]) {

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Project");

	int graphSize = 16;
	Graph< pair<string, int>, int > graph(graphSize);
	
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
		graph.addNode(pair<string,int>(nodeLabel, 0), i, waypoint);
		
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
	sf::CircleShape nodes[16];
	sf::Color circleColour(200, 200, 200);

	sf::Text text[16];

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
		text[index].setPosition(sf::Vector2f(xPos + 7, yPos));
		text[index].setFillColor(sf::Color::White);

		nodes[index].setRadius(20);
		nodes[index].setFillColor(sf::Color::Red);
		nodes[index].setOutlineColor(sf::Color::Blue);
		nodes[index].setOutlineThickness(4);
		nodes[index].setPosition(xPos, yPos);

		index++;
	}
	nodesFile.close();

	
	//set up a path
	std::vector<Node* > path;


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
	
	while (window.isOpen())
	{
		if (timer < 3100)
		timer++;

		// draw nodes
		for (int index = 0; index < 16; index++)
		{
			window.draw(nodes[index]);
			window.draw(text[index]);
		}
		//window.draw(edges);
		
		int start = 0;
		int goal = 24;

		if (timer > 500 && startMessage == false)
		{
			cout << "Lifelong Planning A*" << endl;
			std::cout << "Input Starting node + Goal node " << std::endl;
			std::cout << std::endl;
			startMessage = true;
		}
	
		if (timer > 2000 && searchInitialized == false)
		{
			cout << "Starting point: " << endl;
			cin >> start;
			cout << "Goal: " << endl;
			cin >> goal;
			graph.LPAStarInitialize(graph.nodeArray()[start], graph.nodeArray()[goal], path);
			searchInitialized = true;
		}

		if (searchInitialized == true)
		{
			//graph.LPAStar(graph.nodeArray()[start], graph.nodeArray()[goal], path);
			graph.ComputeShortestPath(graph.nodeArray()[start], graph.nodeArray()[goal]);
		}

		window.display();
	}
	
	system("PAUSE");
}