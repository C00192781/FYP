#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include "SFML\Graphics.hpp"

// Forward references
template <typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphNode {
private:
	// typedef the classes to make our lives easier.
	typedef GraphArc<NodeType, ArcType> Arc;
	typedef GraphNode<NodeType, ArcType> Node;
	// -------------------------------------------------------
	// Description: data inside the node
	// -------------------------------------------------------
	NodeType m_data;

	// -------------------------------------------------------
	// Description: list of arcs that the node has.
	// -------------------------------------------------------
	list<Arc> m_arcList;

	// -------------------------------------------------------
	// Description: This remembers if the node is marked.
	// -------------------------------------------------------
	bool m_marked;

	// -------------------------------------------------------
	// Description: pointer to previous node
	// -------------------------------------------------------
	Node *m_previous;


	float estimateDistance;

	///// NEW
	NodeType rhs_data;

	sf::Vector2f m_waypoint;


	sf::Vector2f m_goal;

	sf::Vector2f m_key;

	float m_heuristic;

	bool m_obstacle; 

	// holds from, weight values of incoming arcs (deleted) 
	// used for when we set obstacles
	std::list<std::pair<int, Node>> m_inArcList;


	// path distance


public:
	// Constructor function
	GraphNode(Node * previous = 0) : m_previous(previous) {}

	// Accessor functions
	list<Arc> const & arcList() const {
		return m_arcList;
	}

	list<Arc> & arcList2() {
		return m_arcList;
	}

	bool marked() const {
		return m_marked;
	}


	NodeType const & data() const {
		return m_data;
	}


	NodeType const & rhsData() const {
		return rhs_data;
	}


	// Manipulator functions
	// setters 
	void setData(NodeType data) {
		m_data = data;
	}


	void setRhsData(NodeType rhsData) {
		rhs_data = rhsData;
	}


	void setMarked(bool mark) {
		m_marked = mark;
	}


	// Estimate 
	float getEstimate()
	{
		return estimateDistance;
	}

	void setEstimate(float newEstimate)
	{
		estimateDistance = newEstimate;
	}


	// previous
	GraphNode* getPrevious()
	{
		return m_previous;
	}

	void setPrevious(GraphNode *prev)
	{
		m_previous = prev;
	}

	void setWaypoint(sf::Vector2f waypoint)
	{
		m_waypoint = waypoint;
	}

	sf::Vector2f getWaypoint()
	{
		return m_waypoint;
	}

	void setGoal(sf::Vector2f goal)
	{
		m_goal = goal;
	}

	sf::Vector2f getGoal()
	{
		return m_goal;
	}

	void setKey(sf::Vector2f key)
	{
		m_key = key;
	}

	sf::Vector2f getKey()
	{
		return m_key;
	}

	void setHeuristic(float heuristic)
	{
		m_heuristic = heuristic;
	}

	float getHeuristic()
	{
		return m_heuristic;
	}


	void setObstacle(bool obstacle)
	{
		m_obstacle = obstacle;
	}

	bool getObstacle()
	{
		return m_obstacle;
	}


	/*void setRhsData(int value) {
		rhs_data.second = value;
	}

	int getRhsData()
	{
		return rhs_data.second; 
	}*/


	//Node * previous() const {
	//	return m_previous;
	//}


	//void setStarData(NodeType )

	/*int getDistance()
	{
		return m_distance;
	}

	void setDistance(int distance)
	{
		m_distance = distance;
	}*/


    Arc* getArc( Node* pNode );    
    void addArc( Node* pNode, ArcType pWeight );
    void removeArc( Node* pNode );


};

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    This finds the arc in the current node that
//                  points to the node in the parameter.
//  Arguments:      The node that the arc connects to.
//  Return Value:   A pointer to the arc, or 0 if an arc doesn't
//                  exist from this to the specified input node.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
GraphArc<NodeType, ArcType>* GraphNode<NodeType, ArcType>::getArc( Node* node ) {

     auto iter = m_arcList.begin();
     auto endIter = m_arcList.end();
     Arc* arc = 0;
     
     // find the arc that matches the node
     for( ; iter != endIter && nullptr == arc; ++iter ) {         
          if ( (*iter).node() == node) {
               arc = &( (*iter) );
          }
     }

     // returns nullptr if not found
     return arc;
}


// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    This adds an arc from the current node pointing
//                  to the first parameter, with the second parameter 
//                  as the weight.
//  Arguments:      First argument is the node to connect the arc to.
//                  Second argument is the weight of the arc.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc( Node* node, ArcType weight ) {
   // Create a new arc.
   Arc a;
   a.setNode(node);
   a.setWeight(weight);   
   // Add it to the arc list.
   m_arcList.push_back( a );
}


// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::removeArc( Node* node ) {
     auto iter = m_arcList.begin();
     auto endIter = m_arcList.end();

     int size = m_arcList.size();
     // find the arc that matches the node
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == node) {
             m_arcList.remove( (*iter) );
          }                           
     }
}

#include "GraphArc.h"

#endif
