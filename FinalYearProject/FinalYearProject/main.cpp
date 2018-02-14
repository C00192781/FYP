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
// *************************************************
typedef GraphNode<pair<std::string, int>, int> Node;

int main(int argc, char *argv[]) {

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Project");

	Graph< pair<string, int>, int > graph(30);
	
	std::string nodeLabel;

	/*map<string, int> map;*/

	int i = 0;
	ifstream myfile;
	myfile.open ("nodes.txt");

	while ( myfile >> nodeLabel ) {
	// giving each node a string and an integer we'll change later

		graph.addNode(pair<string,int>(nodeLabel, 0), i);
		i++;
	}
	myfile.close();

	myfile.open("arcs.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
	}
    myfile.close();

	
	//set up a path
	std::vector<Node* > path;



	int start = 0;
	int goal = 24;

	cout << "Provide starting point" << endl;
	cin >> start;
	cout << "Provide goal" << endl;
	cin >> goal;

	graph.LPAStar(graph.nodeArray()[start], graph.nodeArray()[goal], path);
	
	//// set up a pointer to the Goal Node
	Node *pCurrent = graph.nodeArray()[goal];
	// used to output path + path values
	while (pCurrent != graph.nodeArray()[start])
	{
		std::cout << "Node: " << pCurrent->data().first << std::endl; 
		std::cout << "Distance: " << pCurrent->data().second << std::endl;

		std::cout << "Estimate: " << pCurrent->getEstimate() << std::endl;
		pCurrent = pCurrent->getPrevious();
	}


	if(pCurrent == graph.nodeArray()[start])
	{
		/*path.push_back(pCurrent->data().first);
		cout << path.front() << endl;*/
	}


	//for (int index = path.size() - 1; index > -1; index--)
	//{
	//	std::cout << path.at(i)->data().first << path.at(i)->data().second;
	//	if (path.at(i) == graph.nodeArray()[goal])
	//	{
	//		std::cout << std::endl;
	//	}
	//}


	/// FOR DRAWING NODES + EDGES
	sf::CircleShape nodes[30];
	sf::Color circleColour(200, 200, 200);

	sf::VertexArray edges(sf::LinesStrip, 72);
	sf::VertexArray edge(sf::LinesStrip, 2);

	
	ifstream nodesFile;
	
	int index = 0;
	string nodeString;

	// positions we'll draw nodes at
	int xPos;
	int yPos;

	// open text file extract positions for different nodes
	nodesFile.open("drawNodes.txt");
	while (nodesFile >> xPos >> yPos >> nodeString)
	{
		nodes[index].setRadius(20);
		nodes[index].setFillColor(sf::Color::Red);
		nodes[index].setOutlineColor(sf::Color::Blue);
		nodes[index].setOutlineThickness(4);
		nodes[index].setPosition(xPos, yPos);

		index++;
	}


	nodesFile.close();

	int edgeIndex;
	ifstream edgesFile;
	string edgeString;

	int posX, posY;
	
	while (window.isOpen())
	{

		sf::Event event;

		while (window.pollEvent(event))
		{
			edgeIndex = 0;
			edgesFile.open("drawEdges.txt");
			while (edgesFile >> posX >> posY)
			{
				edges[edgeIndex].position = sf::Vector2f(posX, posY);
				edges[edgeIndex].color = sf::Color::Green;

				edgeIndex++;	
			}
			
			// draw nodes
			for (int index = 0; index <= 29; index++)
			{
				window.draw(nodes[index]);
			}

			window.draw(edges);
			window.display();
		}
	}
	

	system("PAUSE");
	
}
