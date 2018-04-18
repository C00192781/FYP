#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>
#include <sstream> 
#include "SFML\Graphics.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

#include "GraphArc.h"
#include "GraphNode.h"

using namespace std;

//template <class NodeType, class ArcType> class GraphArc;
//template <class NodeType, class ArcType> class GraphNode;
class GraphArc;
class GraphNode;

//template<class NodeType, class ArcType>

class NodeSearchCostComparer
{
public:
	// compares distance
	bool operator() (GraphNode * n1, GraphNode * n2)
	{
		pair<string, int> p1 = n1->data();
		pair<string, int> p2 = n2->data();
		return p1.second > p2.second;
	}
};


class Priority
{
public:
	//typedef GraphNode<NodeType, ArcType> Node;
	// used to compare f values

	//float CalculateHeuristic(Node * node)
	//{
	//	float dx = abs(node->getWaypoint().x - node->getGoal().x);
	//	float dy = abs(node->getWaypoint().y - node->getGoal().y);

	//	float result = 100.0f * (dx + dy);

	//	return result;
	//}

	bool operator() (GraphNode * n1, GraphNode * n2)
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
//		// a way that they form a heap.
//		// std::begin() returns an iterator to the beginning of the given container c 
//		std::make_heap(std::begin(c), std::end(c), comp);
//	}
//
//	void begin()
//	{
//		return std::begin(c);
//	}
//	void end()
//	{
//		return std::end(c);
//	}
//};



// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
class Graph {
private:
	// ----------------------------------------------------------------
	//  Description:    A container of all the nodes in the graph.
	// ----------------------------------------------------------------

	//std::vector<Node *> m_nodes;

	// An array of all the nodes in the graph.
	GraphNode** m_pNodes;

	// the maximum number of nodes in the graph.
	int m_maxNodes;


public:
	// Constructor and destructor functions
	Graph();
	Graph(int size);
	~Graph();

	// Accessors
	GraphNode** nodeArray() const {
		return m_pNodes;
	}

	std::vector<sf::Vector2f> waypoints;
	void addWaypoint(sf::Vector2f waypoint) { waypoints.push_back(waypoint); }

	// Public member functions.
	bool addNode(std::pair<std::string, int> data, std::pair<std::string, int> rhsData, bool marked, int index, sf::Vector2f waypoint);
	bool addNode(std::pair<std::string, int> data, std::pair<std::string, int> rhsData, bool marked, int index, sf::Vector2f waypoint, sf::Vector2f key, float heuristic, sf::Vector2f goal);
	bool addNode(GraphNode *node, int index);
	void removeNode(int index);
	bool addArc(int from, int to, int weight);
	void removeArc(int from, int to);
	GraphArc* getArc(int from, int to);
	void clearMarks();

	std::vector<GraphNode *> nodeQueue;

	// std::vector<GraphNode *>& path
	void LPAStarInitialize(GraphNode* pStart, GraphNode* pDest);
	void UpdateVertex(GraphNode *node, GraphNode * pStart);
	void ComputeShortestPath(GraphNode * pStart, GraphNode * pDest);
	void SetObstacle(int node, bool obstacle, int start);
	
	float CalculateHeuristic(GraphNode * node, bool obstacle);
	sf::Vector2f CalculateKey(GraphNode *node, std::string searchType);
	bool keyComparer(GraphNode* n1, GraphNode* n2);

	std::vector<GraphNode *> openQueue;
	std::vector<GraphNode *> closedQueue;
	std::vector<GraphNode *> inconsQueue;

	void ADStarInitialize(GraphNode* pStart, GraphNode* pDest,
		std::vector<GraphNode *>& path, float inflation);
	void ADStarUpdateState(GraphNode* node, GraphNode * pDest);
	int ComputeOrImprovePath(GraphNode * pStart, GraphNode * pDest);
	void setInflation(float inflation);
	float getInflation();
	void MoveStates();
	void InflationHandler();

	std::map<int, GraphNode> obstacleMap;

	bool flag = false;
	float ADStarInflation = 0;

	std::string searchType = " ";






	std::vector<GraphNode*> path;
};

#endif