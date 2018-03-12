﻿#ifndef GRAPH_H
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

	//float CalculateHeuristic(Node * node)
	//{
	//	float dx = abs(node->getWaypoint().x - node->getGoal().x);
	//	float dy = abs(node->getWaypoint().y - node->getGoal().y);

	//	float result = 100.0f * (dx + dy);

	//	return result;
	//}

	bool operator() (Node * n1, Node * n2)
	{
		float k1 = n1->getKey().x;
		float k2 = n1->getKey().y;

		float p1 = n2->getKey().x;
		float p2 = n2->getKey().y;

		while (k1 != k2)
		{
			if (p1 == p2 || p1 < k1)
			{
				return false;
			}
			else if (k1 < p1)
			{
				return true;
			}
			k1++; 
			p1++;
		}
		return (p1 != p2);

		//return (k1 < p1);
		//return ( (k1 < p1) || ( (k1 == p1) && (k2 <= p2) ));
		
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

	void begin() 
	{
		return std::begin(c);
	}
	void end() 
	{
		return std::end(c);
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


	typedef GraphNode<NodeType, ArcType> Node;

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
	sf::Vector2f CalculateKey(Node *node);
	void UpdateVertex(Node *node, Node *pStart, MyPriorityQueue<Node *, std::vector<Node *>, Priority<NodeType, ArcType>> *nodeQueue);
	void ComputeShortestPath(MyPriorityQueue<Node *, std::vector<Node *>, Priority<NodeType, ArcType>> *nodeQueue, Node *pStart, Node *pDest);
	float CalculateHeuristic(Node * node);
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
float Graph<NodeType, ArcType>::CalculateHeuristic(Node * node)
{
	float dx = abs(node->getWaypoint().x - node->getGoal().x);
	float dy = abs(node->getWaypoint().y - node->getGoal().y);

	float result = 100.0f * (dx + dy);

	return result;
}




template<class NodeType, class ArcType>
sf::Vector2f Graph<NodeType, ArcType>::CalculateKey(Node * node)
{
	float gs = node->data().second;
	float rhs = node->rhsData().second;


	float k1 = std::min(gs, rhs) + node->getHeuristic();
	float k2 = std::min(gs, rhs);

	sf::Vector2f key = sf::Vector2f{ k1, k2 };

	return key;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::LPAStar(Node* pStart, Node* pDest, std::vector<Node *>& path)
{
	MyPriorityQueue<Node *, std::vector<Node *>, Priority<NodeType, ArcType>> nodeQueue;

	if (pStart != 0)
	{
		/////////// 3
		//////////// for all s ∈ S rhs(s) = g(s) = ∞;
		// setting the initial values of all of the nodes
		for (int i = 0; i < m_maxNodes; i++)
		{
			//std::cout << m_pNodes[i]->getWaypoint().x << std::endl;
			m_pNodes[i]->setGoal(pDest->getWaypoint());
			std::cout << m_pNodes[i]->getGoal().x << " " << m_pNodes[i]->getGoal().y << std::endl;

			float heuristic = CalculateHeuristic(m_pNodes[i]);
			m_pNodes[i]->setHeuristic(heuristic);

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

		pStart->setRhsData(pair<string, int>(pStart->rhsData().first, 0));
		sf::Vector2f initialKey;
		initialKey.x = CalculateHeuristic(pStart);
		initialKey.y = 0;
		pStart->setKey(initialKey);
		// insert starting node into the queue
		nodeQueue.push(pStart);

		//set as being marked/visited
		pStart->setMarked(true);
	}

	ComputeShortestPath(&nodeQueue, pStart, pDest);
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

	//nodeQueue.top()->data().first != pDest->data().first
	// **************
	while (nodeQueue->top() != pDest || pDest->data().second != pDest->rhsData().second)
	{

		if (pDest->data().second != pDest->rhsData().second)
		{
			std::cout << "second parameter true" << std::endl;
		}

		std::cout << "check: " << nodeQueue->top()->data().first << " " << nodeQueue->size() << std::endl;

		// ***********
		//nodeQueue.top()->setMarked(false);
		//Node node = *nodeQueue->top();
		////if (nodeQueue->size() != 0)
		////{
		//	nodeQueue->pop();
		/*}*/



		if (nodeQueue->top()->data().second > nodeQueue->top()->rhsData().second)
		{
			/*auto data = node.data();
			data.second = node.rhsData().second;
			node.setData(data);*/


			auto data = nodeQueue->top()->data();
			data.second = nodeQueue->top()->rhsData().second;
			nodeQueue->top()->setData(data);


			nodeQueue->top()->setMarked(false);
			Node node = *nodeQueue->top();
			//if (nodeQueue->size() != 0)
			//{
			nodeQueue->pop();
			std::cout << "node popped" << std::endl;
			/*}*/


			list<Arc>::const_iterator iter = node.arcList().begin();
			list<Arc>::const_iterator endIter = node.arcList().end();

			// for each iteration though the nodes
			for (; iter != endIter; iter++)
			{
				UpdateVertex((*iter).node(), pStart, nodeQueue);
			}
		}
		else
		{
			/*	auto data = node.data();
			data.second = std::numeric_limits<int>::max() - 100000;
			node.setData(data);*/

			auto data = nodeQueue->top()->data();
			data.second = nodeQueue->top()->rhsData().second;
			nodeQueue->top()->setData(data);


			nodeQueue->top()->setMarked(false);
			Node node = *nodeQueue->top();
			//if (nodeQueue->size() != 0)
			//{
			nodeQueue->pop();
			std::cout << "node popped" << std::endl;
			/*}*/

			for (int i = 0; i < m_maxNodes; i++)
			{
				// *********
				if (node.data().first == m_pNodes[i]->data().first)
				{
					UpdateVertex(m_pNodes[i], pStart, nodeQueue);
				}
			}

			list<Arc>::const_iterator iter = node.arcList().begin();
			list<Arc>::const_iterator endIter = node.arcList().end();

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
	int min = 0;
	std::vector<int> predecessors;

	if (node != pStart)
	{
		//std::cout << "starting node" << pStart->data().first << std::endl;
		list<Arc>::const_iterator iter = node->arcList().begin();
		list<Arc>::const_iterator endIter = node->arcList().end();


		//int distance;
		// for each iteration though the nodes
		for (; iter != endIter; iter++)
		{
			// ***************
			int distance = node->data().second + iter->weight();
			//std::cout << "DISTANCE" << distance << std::endl;


			predecessors.push_back(distance);

			//std::cout << "predecessor " << predecessors.size() << std::endl;
			if (predecessors.size() == 1)
			{
				min = predecessors.front();
			}
			if (predecessors.size() > 1)
			{
				min = std::min(min, predecessors.back());
			}
		}

		std::cout << "min value: " << min << std::endl;

		node->setRhsData(pair<string, int>(node->rhsData().first, min));
	}
	// ********
	Node nodeCopy = *node;
	
	std::cout << "Current node being updated: " << node->data().first << std::endl;

	// node is marked if it is in the queue
	if (node->marked() == true)
	{
		//********************
		node->setData(pair<string, int>(node->data().first, std::numeric_limits<int>::max()));
		node->setRhsData(pair<string, int>(node->rhsData().first, std::numeric_limits<int>::max()));

		std::cout << node->data().first << std::endl;

		nodeQueue->reorder();

		nodeQueue->pop();

		//std::cout << node->data().first << std::endl;
		//std::cout << nodeQueue->top()->data().first << std::endl;
	
		node->setData(nodeCopy.data());
		node->setRhsData(nodeCopy.rhsData());
		node->setMarked(false);
	}

	if (node->data().second != node->rhsData().second)
	{
		node->setKey(CalculateKey(node));
		node->setMarked(true);
		nodeQueue->push(node);

		std::cout << "node pushed " << std::endl;
		std::cout << "node with top priority: " << nodeQueue->size() << std::endl;
	}
}

#include "GraphNode.h"
#include "GraphArc.h"


#endif