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

//#include <iostream>
//#include <queue>
//
//class Node
//{
//public:
//	void setData(std::pair<std::string, int> data)
//	{
//		m_pair = data;
//	}
//	std::pair<std::string, int> data() const
//	{
//		return m_pair;
//	}
//private:
//	std::pair<std::string, int> m_pair;
//};
//
//
//class NodeSearchCostComparer
//{
//public:
//	bool operator()(Node * n1, Node * n2)
//	{
//		std::pair<std::string, int> p1 = n1->data();
//		std::pair<std::string, int> p2 = n2->data();
//		return p1.second > p2.second;
//	}
//};
//
//
//template <typename Data, typename Container, typename Predicate>
//class MyPriorityQueue : public std::priority_queue<Data, Container, Predicate>
//{
//public:
//	// std::priority_queue has two useful members:
//	// 1. c is the underlying container of a priority_queue
//	// 2. comp is the comparison predicate
//	void reorder()
//	{
//		// std::make_heap rearranges the elements in the range [first,last] in such 
//		//  a way that they form a heap.
//		// std::begin() returns an iterator to the beginning of the given container c 
//		std::make_heap(std::begin(c), std::end(c), comp);
//	}
//};
//
//
//
//int main()
//{
//	Node *first = new Node();
//	// Each node stores a city name and search path cost
//	first->setData(std::pair<std::string, int>("Aldor", 60));
//	Node *second = new Node();
//	second->setData(std::pair<std::string, int>("Caldor", 40));
//
//	MyPriorityQueue<Node *, std::vector<Node *>, NodeSearchCostComparer> pq;
//	pq.push(first);
//	pq.push(second);
//
//	Node *temp = pq.top();
//	std::cout << temp->data().second << std::endl;
//
//	// Dynamically change the priority of an element within the queue.
//	first->setData(std::pair<std::string, int>("Aldor", 30));
//	// pq still reports Caldor as top element, even though Aldor has a lower path cost.
//	std::cout << pq.top()->data().second << std::endl;
//	// Next, the pop operation results in heap corruption (clearly not desired!)
//	//pq.pop();
//
//	// SOLUTION below:
//	// Comment out pq.pop() above.
//	// Solution is to periodically force the priority queue to reorder its elements 
//	pq.reorder();
//	// pq correctly reports 'Aldor' as top element.
//	std::cout << pq.top()->data().second << std::endl;
//	// You can now do pq.pop() safely without heap corruption
//	system("PAUSE");
//}
//
//
