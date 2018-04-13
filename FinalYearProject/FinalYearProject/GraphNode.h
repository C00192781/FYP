#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include <utility> 
#include <string>
#include "SFML\Graphics.hpp"
#include "GraphArc.h"

// Forward references
class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
class GraphNode {
private:
	// -------------------------------------------------------
	// Description: data inside the node
	// -------------------------------------------------------
	std::pair<std::string, int> m_data;

	// -------------------------------------------------------
	// Description: list of arcs that the node has.
	// -------------------------------------------------------
	std::list<GraphArc> m_arcList;

	// -------------------------------------------------------
	// Description: This remembers if the node is marked.
	// -------------------------------------------------------
	bool m_marked;

	// -------------------------------------------------------
	// Description: pointer to previous node
	// -------------------------------------------------------
	GraphNode *m_previous;

	float estimateDistance;


	std::pair<std::string, int> rhs_data;

	sf::Vector2f m_waypoint;
	sf::Vector2f m_goal;
	sf::Vector2f m_key;

	float m_heuristic;
	bool m_obstacle; 

	// path distance


public:
	GraphNode();
	~GraphNode();
	// Constructor function
	//GraphNode(GraphNode * previous = 0) : m_previous(previous) {}

	// holds from, weight values of incoming arcs (deleted) 
	// used for when we set obstacles
	std::list<std::pair<int, int>> m_inArcList;
	std::list<std::pair<int, int>> m_outArcList;


	// Accessor functions
	std::list<GraphArc> const & arcList() const;

	std::list<GraphArc> & arcList2();

	bool marked() const;


	std::pair<std::string, int> const & data() const;


	std::pair<std::string, int> const & rhsData() const;


	// Manipulator functions
	// setters 
	void setData(std::pair<std::string, int> data);


	void setRhsData(std::pair<std::string, int> rhsData);


	void setMarked(bool mark);


	// Estimate 
	float getEstimate();

	void setEstimate(float newEstimate);


	// previous
	GraphNode* getPrevious();

	void setPrevious(GraphNode *prev);

	void setWaypoint(sf::Vector2f waypoint);

	sf::Vector2f getWaypoint();

	void setGoal(sf::Vector2f goal);

	sf::Vector2f getGoal();

	void setKey(sf::Vector2f key);

	sf::Vector2f getKey();

	void setHeuristic(float heuristic);

	float getHeuristic();

	void setObstacle(bool obstacle);

	bool getObstacle();

    GraphArc* getArc(GraphNode* pNode );
    void addArc(GraphNode* pNode, int pWeight );
    void removeArc(GraphNode* pNode );
};

#endif