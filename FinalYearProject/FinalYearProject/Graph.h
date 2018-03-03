#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <sstream> 
#include "SFML\Graphics.hpp"

using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

template<class NodeType, class ArcType>
class NodeSearchCostComparer
{
public:
	typedef GraphNode<NodeType, ArcType> Node;
	// compares distance
	bool operator() (Node * n1, Node * n2)
	{
		pair<string, int> p1 = n1->data();
		pair<string, int> p2 = n2->data();
		return p1.second > p2.second;
	}
};



template<class NodeType, class ArcType>
class Priority
{
public:
	typedef GraphNode<NodeType, ArcType> Node;
	// used to compare f values

	float CalculateHeuristic(Node * node)
	{
		float dx = abs(node->getWaypoint().x - node->getGoal().x);
		float dy = abs(node->getWaypoint().y - node->getGoal().y);

		float result = 200.0f * (dx + dy);

		return result;
	}

	bool operator() (Node * n1, Node * n2)
	{
		float k1 = std::min(n1->data().second, n1->rhsData().second) + CalculateHeuristic(n1);
		float k2 = std::min(n1->data().second, n1->rhsData().second);

		/*std::cout << "Calculations" << CalculateHeuristic(n1) << std::endl;*/
		float p1 = std::min(n1->data().second, n1->rhsData().second) + CalculateHeuristic(n2);
		float p2 = std::min(n1->data().second, n1->rhsData().second);

		/*sf::Vector2f k = sf::Vector2f{ k1, k2 };
		sf::Vector2f p = sf::Vector2f{ p1, p2 };*/
		bool check;

		//if ((k1 < p1) || ((k1 == p1) && (k2 <= p2)))
		//{
		//	std::cout << "true!!!" << std::endl;
		//}
		
		return ((k1 < p1) || ((k1 == p1) && (k2 <= p2)));
	}
};

template <typename Data, typename Container, typename Predicate>
class MyPriorityQueue : public std::priority_queue<Data, Container, Predicate>
{
public:
	// std::priority_queue has two useful members:
	// 1. c is the underlying container of a priority_queue
	// 2. comp is the comparison predicate
	void reorder()
	{
		// std::make_heap rearranges the elements in the range [first,last] in such 
		// a way that they form a heap.
		// std::begin() returns an iterator to the beginning of the given container c 
		std::make_heap(std::begin(c), std::end(c), comp);
	}
};



// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph {
private:

	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;

	// ----------------------------------------------------------------
	//  Description:    A container of all the nodes in the graph.
	// ----------------------------------------------------------------

	//std::vector<Node *> m_nodes;

	// An array of all the nodes in the graph.
	Node** m_pNodes;

	// the maximum number of nodes in the graph.
	int m_maxNodes;


public:
	// Constructor and destructor functions
	Graph(int size);
	~Graph();

	// Accessors
	Node** nodeArray() const {
		return m_pNodes;
	}

	std::vector<sf::Vector2f> waypoints;
	void addWaypoint(sf::Vector2f waypoint) { waypoints.push_back(waypoint); }


	// Public member functions.
	bool addNode(NodeType data, int index, sf::Vector2f waypoint);
	void removeNode(int index);
	bool addArc(int from, int to, ArcType weight);
	void removeArc(int from, int to);
	Arc* getArc(int from, int to);
	void clearMarks();

	void LPAStar(Node* pStart, Node* pDest,
		std::vector<Node *>& path);
	void CalculateKey(Node node, Node *pStart, Node *pDest);
	void UpdateVertex(Node *node, Node *pStart, MyPriorityQueue<Node *, std::vector<Node *>, Priority<NodeType, ArcType>> *nodeQueue);
	void ComputeShortestPath(MyPriorityQueue<Node *, std::vector<Node *>, Priority<NodeType, ArcType>> *nodeQueue, Node *pStart, Node *pDest);

	/*void UCS*/
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph(int size) : m_maxNodes(size) {
	int i;
	m_pNodes = new Node *[m_maxNodes];
	// go through every index and clear it to null (0)
	for (i = 0; i < m_maxNodes; i++) {
		m_pNodes[i] = 0;
	}


}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
	for (int index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			delete m_pNodes[index];
		}
	}
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode(NodeType data, int index, sf::Vector2f waypoint) {
	bool nodeNotPresent = false;
	// find out if a node does not exist at that index.
	if (m_pNodes[index] == 0) {
		nodeNotPresent = true;
		// create a new node, put the data in it, and unmark it.
		m_pNodes[index] = new Node;
		m_pNodes[index]->setData(data);
		m_pNodes[index]->setMarked(false);
		// ************* is this acceptable???
		m_pNodes[index]->setRhsData(data);
		m_pNodes[index]->setWaypoint(waypoint);
	}

	return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode(int index) {
	// Only proceed if node does exist.
	if (m_pNodes[index] != 0) {
		// now find every arc that points to the node that
		// is being removed and remove it.        
		Arc* arc;

		// loop through every node
		for (int node = 0; node < m_maxNodes; node++) {
			// if the node is valid...
			if (m_pNodes[node] != 0) {
				// see if the node has an arc pointing to the current node.
				arc = m_pNodes[node]->getArc(m_pNodes[index]);
			}
			// if it has an arc pointing to the current node, then
			// remove the arc.
			if (arc != 0) {
				removeArc(node, index);
			}
		}


		// now that every arc pointing to the current node has been removed,
		// the node can be deleted.
		delete m_pNodes[index];
		m_pNodes[index] = 0;
	}
}

// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc(int from, int to, ArcType weight) {
	bool proceed = true;
	// make sure both nodes exist.
	if (m_pNodes[from] == 0 || m_pNodes[to] == 0)
	{
		proceed = false;
	}

	// if an arc already exists we should not proceed
	if (m_pNodes[from]->getArc(m_pNodes[to]) != 0) {
		proceed = false;
	}

	if (proceed == true) {
		// add the arc to the "from" node.
		m_pNodes[from]->addArc(m_pNodes[to], weight);
	}

	return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc(int from, int to) {
	// Make sure that the node exists before trying to remove
	// an arc from it.
	bool nodeExists = true;
	if (m_pNodes[from] == 0 || m_pNodes[O] == 0)
	{
		nodeExists = false;
	}

	if (nodeExists == true)
	{
		// remove the arc.
		m_pNodes[from]->removeArc(m_pNodes[to]);
	}
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc(int from, int to) {
	Arc* arc = 0;
	// make sure the to and from nodes exist
	if (m_pNodes[from] != 0 && m_pNodes[to] != 0) {
		arc = m_pNodes[from]->getArc(m_pNodes[to]);
	}

	return arc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
	for (int index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0)
		{
			m_pNodes[index]->setMarked(false);
		}
	}
}




template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::CalculateKey(Node node, Node *pStart, Node *pDest)
{

	// f(n) = h(n) + g(n)
	// total cost = node weight + 


	// h(n) = estimated cost to goal from n 
	// g(n) = cost so far to reach n 
	// f(n) = estimated total cost of the path through n to goal


	// g(n) = cost so far to reach n 
	// int distance = nodeQueue.top()->data().second + iter->weight();

	// n [min(g(s), rhs(s)) + h(s); min(g(s), rhs(s))];
	float gs = node.data().second;
	float rhs = node.rhsData().second;
	//float hs = 

	//float hs = std::max(pStart->)



	//float hs = 

	/////////**********************************************************
	//"Distance: "

	//	return sf::Vector2f(5, 5);
	//cout << node.data().second << endl;
	//return sf::Vector2f(min(node->data(), node->rhsData() + ), )
	//return sf::Vector2f();

	// return

}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::LPAStar(Node* pStart, Node* pDest, std::vector<Node *>& path)
{
	/////// INITIALIZE

	Priority<NodeType, ArcType> priority;

	MyPriorityQueue<Node *, std::vector<Node *>, Priority<NodeType, ArcType>> nodeQueue;
	string g = pDest->data().first;
	int goal = stoi(g);

	//nodeQueue.goalSetter(goal);

	//stringstream(pDest->data().first);

	//string s = "12345";

	//// object from the class stringstream
	//stringstream geek(s);

	//// The object has the value 12345 and stream
	//// it to the integer x
	//int x = 0;
	//geek >> x;



	//std::cout << pDest->data().first << std::endl;
	// std::priority_queue<Data, Container, Predicate>






	if (pStart != 0)
	{
		/////////// 3
		//////////// for all s ∈ S rhs(s) = g(s) = ∞;
		// setting the initial values of all of the nodes
		for (int i = 0; i < m_maxNodes; i++)
		{
			// estimate commented out
			//m_pNodes[i]->setEstimate(std::numeric_limits<int>::max() - 100000);

			//std::cout << m_pNodes[i]->getWaypoint().x << std::endl;
			m_pNodes[i]->setGoal(pDest->getWaypoint());
			std::cout << m_pNodes[i]->getGoal().x << std::endl;
			auto data = m_pNodes[i]->data();
			auto rhsData = m_pNodes[i]->rhsData();
			// set the weight to an infinite value to start off with
			data.second = std::numeric_limits<int>::max() - 100000;
			// set the rhs values of all nodes to an infinite value
			rhsData.second = std::numeric_limits<int>::max() - 100000;


			m_pNodes[i]->setMarked(false);

			m_pNodes[i]->setData(data);
			m_pNodes[i]->setRhsData(rhsData);

		}

		/////////////// 5 ???
		///////////////  U.Insert(sstart, [h(sstart); 0]);
		// insert starting node into the queue
		nodeQueue.push(pStart);
		//set as being marked/visited
		pStart->setMarked(true);
		pStart->setRhsData(pair<string, int>(pStart->rhsData().first, 0));
	}

	//set as being marked/visited
	pDest->setMarked(true);
	pDest->setRhsData(pair<string, int>(pDest->rhsData().first, 0));
	nodeQueue.push(pDest);
	//nodeQueue.reorder();
	ComputeShortestPath(&nodeQueue, pStart, pDest);


	//Node *node = nodeQueue.top();


	//// test
	//UpdateVertex(node, pStart, &nodeQueue);

}


template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::ComputeShortestPath(MyPriorityQueue<Node*, std::vector<Node*>, Priority<NodeType, ArcType>> *nodeQueue, Node * pStart, Node * pDest)
{
	//{09} while (U.TopKey()<˙ CalculateKey(sgoal) OR rhs(sgoal) 6= g(sgoal))
	//{10} u = U.Pop();
	//{11} if (g(u) > rhs(u))
	//{12} g(u) = rhs(u);
	//{13} for all s ∈ succ(u) UpdateVertex(s);
	//{14} else
	//{15} g(u) = ∞;
	//{16} for all s ∈ succ(u) ∪ {u} UpdateVertex(s);

	/*while (nodeQueue->top(0 < ))*/

	while (pDest->data().second != pDest->rhsData().second)
	{
		//nodeQueue->reorder();
		Node * node = nodeQueue->top();
		nodeQueue->pop();
		std::cout << node->data().first << "Test" << std::endl;
		if (node->data().second > node->rhsData().second)
		{
			auto data = node->data();
			data.second = node->rhsData().second;
			node->setData(data);

			list<Arc>::const_iterator iter = node->arcList().begin();
			list<Arc>::const_iterator endIter = node->arcList().end();

			// for each iteration though the nodes
			for (; iter != endIter; iter++)
			{
				UpdateVertex((*iter).node(), pStart, nodeQueue);
			}
		}
		else
		{
			auto data = node->data();
			data.second = std::numeric_limits<int>::max() - 100000;
			node->setData(data);

			UpdateVertex(node, pStart, nodeQueue);

			list<Arc>::const_iterator iter = node->arcList().begin();
			list<Arc>::const_iterator endIter = node->arcList().end();

			// for each iteration though the nodes
			for (; iter != endIter; iter++)
			{
				UpdateVertex((*iter).node(), pStart, nodeQueue);
			}
		}

	}
}

template<class NodeType, class ArcType>
inline void Graph<NodeType, ArcType>::UpdateVertex(Node *node, Node * pStart, MyPriorityQueue<Node *, std::vector<Node *>, Priority<NodeType, ArcType>> *nodeQueue)
{
	std::cout << "start node data" << pStart->data().second << std::endl;
	std::cout << "node data" << node->data().second << std::endl;

	int min;
	std::vector<int> predecessors;
	/*if (node->data().first != pStart->data().first)
	{*/
	list<Arc>::const_iterator iter = node->arcList().begin();
	list<Arc>::const_iterator endIter = node->arcList().end();

	//int distance;
	// for each iteration though the nodes
	for (; iter != endIter; iter++)
	{
		// if the current node is not the highest priority node - THEN WE KNOW TO START ADDING UP DISTANCE
		/*if ((*iter).node() != nodeQueue.top())
		{*/

		int distance = node->data().second + iter->weight();
		predecessors.push_back(distance);

		std::cout << predecessors.size() << "predecessor " << std::endl;
		if (predecessors.size() == 1)
		{
			min = predecessors.back();
		}
		if (predecessors.size() > 1)
		{
			min = std::min(predecessors.back(), predecessors.back() - 1);
		}
	}


	/*}*/


	std::cout << "min " << min << std::endl;

}

//void Initialize()
//{
//	std::vector<Node*> path;
//	std::priority_queue < Node *, vector<Node *>, Priority<NodeType, ArcType>> nodeQueue;
//}



#include "GraphNode.h"
#include "GraphArc.h"


#endif













//template<class NodeType, class ArcType>
//class SearchCostComparer2
//{
//public:
//	typedef GraphNode<NodeType, ArcType> Node;
//	// used to compare f values
//	bool operator() (Node * n1, Node * n2)
//	{
//		// f(n) = h(n) + g(n)
//		int p1 = n1->getEstimate() + n1->data().second;
//		int p2 = n2->getEstimate() + n2->data().second;
//		return p1 > p2;
//	}
//};




//template<class NodeType, class ArcType>
//class Priority
//{
//public:
//	typedef GraphNode<NodeType, ArcType> Node;
//	// used to compare f values
//	bool operator() (Node * n1, Node * n2)
//	{
//		
//		// f(n) = h(n) + g(n)
//		int p1 = n1->
//		int p2 = n2->getEstimate() + n2->data().second;
//		return p1 > p2;
//	}
//};