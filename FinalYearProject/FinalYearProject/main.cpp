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



int startX, startY = 0;
int goalX, goalY = 0; 
int mazeWidth, mazeHeight = 0;         

struct cell
{
	int gValue;
	int hValue;
	int rhs;
	int iteration;
	int key;
};


cell **maze = NULL;
cell *mazeStart;
cell *mazeGoal;

void lpastarinitialize()
{
	// initially set to infinity
	mazeStart->gValue = std::numeric_limits<int>::max() - 100000;
	mazeStart->rhs = 0;
	mazeGoal->gValue = std::numeric_limits<int>::max() - 100000;
	mazeGoal->rhs = std::numeric_limits<int>::max() - 100000;
	// h value will be calculated during maze generation 
}


void createmaze()
{
	int x, y;
	maze = (cell **)calloc(mazeWidth, sizeof(cell *));
	for (x = 0; x < mazeWidth; x++)
	{
		maze[x] = (cell *)calloc(mazeHeight, sizeof(cell));
	}

	for (y = 0; y < mazeHeight; y++)
	{
		for (x = 0; x < mazeWidth; x++)
		{
		
		}
	}
	cout << "Maze created and populated with cells" << endl;
}



int main(int argc, char *argv[]) {

	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Project");

	cout << "Type A* or LPA*" << endl;
	std::string projectInitiation; 
	cin >> projectInitiation;

	if (projectInitiation == "A*")
	{
		Graph< pair<string, int>, int > graph(30);

		std::string nodeLabel;

		/*map<string, int> map;*/

		int i = 0;
		ifstream myfile;
		myfile.open("nodes.txt");

		while (myfile >> nodeLabel) {
			// giving each node a string and an integer we'll change later

			graph.addNode(pair<string, int>(nodeLabel, 0), i);
			i++;
		}
		myfile.close();

		myfile.open("arcs.txt");

		int from, to, weight;
		while (myfile >> from >> to >> weight) {
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

		graph.aStar(graph.nodeArray()[start], graph.nodeArray()[goal], path);

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


		if (pCurrent == graph.nodeArray()[start])
		{
			/*path.push_back(pCurrent->data().first);
			cout << path.front() << endl;*/
		}

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
	}
	else if (projectInitiation == "LPA*")
	{
		cout << "Enter Starting Node X Value" << endl;
		cin >> startX;
		cout << "Enter Starting Node Y Value" << endl;
		cin >> startY;
		cout << "Enter Goal Node X Value" << endl;
		cin >> goalX;
		cout << "Enter Goal Node Y Value" << endl;
		cin >> goalY;
		cout << "Specificy Maze Width" << endl;
		cin >> mazeWidth;
		cout << "Specify Maze Height" << endl;
		cin >> mazeHeight;
		createmaze();
	}
	
	system("PAUSE");
	
}