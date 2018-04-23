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

class GraphArc;
class GraphNode;

// used by the UCS search algorithm (Uniform Cost Search)
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


// used by A* search algorithm
class NodeSearchCostComparer2
{
public:
	bool operator() (GraphNode * n1, GraphNode * n2)
	{
		int p1 = n1->getEstimate() + n1->data().second;
		int p2 = n2->getEstimate() + n2->data().second;
		return p1 > p2;
	}
};


class Priority
{
public:
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
	}
};

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

	std::vector<int> nodesToExamine;

	GraphNode* tempStart;
	GraphNode* tempGoal;

	int cellExpansions;
	int pathLength;
	float inflationIteration; 

	// notes inflation at start of each call of ComputeOrImprovePath for logging data
	void setInflationIteration(float inflation);

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

	std::vector<GraphNode*> nodeQueue;
	std::vector<GraphNode*> path;

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
	void MoveAgent();

	std::map<int, GraphNode> obstacleMap;

	bool flag = false;
	float ADStarInflation = 0;

	std::string searchType = " ";

	void UCS(GraphNode* pStart, GraphNode* pDest, std::vector<GraphNode *>& path);
	void InitializeAStar(GraphNode* pStart, std::priority_queue <GraphNode*, vector<GraphNode*>, NodeSearchCostComparer2> *queue);
	void AStar(GraphNode* pStart, GraphNode* pDest);

	int ADStarFindStart();

	std::vector<GraphNode*> getPath();
	
	int getCellExpansions();
	int getPathLength();
	float getInflationIteration(); 

};

#endif