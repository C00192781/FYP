#include "GraphNode.h"

#include "GraphNode.h"

using namespace std;

GraphNode::GraphNode()
{

}

GraphNode::~GraphNode()
{
}

std::list<GraphArc> const & GraphNode::arcList() const
{
	return m_arcList;
}

std::list<GraphArc>& GraphNode::arcList2()
{
	return m_arcList;
}

bool GraphNode::marked() const
{
	return m_marked;
}

std::pair<std::string, int> const & GraphNode::data() const
{
	return m_data;
}

std::pair<std::string, int> const & GraphNode::rhsData() const
{
	return rhs_data;
}

void GraphNode::setData(std::pair<std::string, int> data)
{
	m_data = data;
}

void GraphNode::setRhsData(std::pair<std::string, int> rhsData)
{
	rhs_data = rhsData;
}

void GraphNode::setMarked(bool mark)
{
	m_marked = mark;
}

float GraphNode::getEstimate()
{
	return estimateDistance;
}

void GraphNode::setEstimate(float newEstimate)
{
	estimateDistance = newEstimate;
}

GraphNode * GraphNode::getPrevious()
{
	return m_previous;
}

void GraphNode::setPrevious(GraphNode * prev)
{
	m_previous = prev;
}

void GraphNode::setWaypoint(sf::Vector2f waypoint)
{
	m_waypoint = waypoint;
}

sf::Vector2f GraphNode::getWaypoint()
{
	return m_waypoint;
}

void GraphNode::setGoal(sf::Vector2f goal)
{
	m_goal = goal;
}

sf::Vector2f GraphNode::getGoal()
{
	return m_goal;
}

void GraphNode::setKey(sf::Vector2f key)
{
	m_key = key;
}

sf::Vector2f GraphNode::getKey()
{
	return m_key;
}

void GraphNode::setHeuristic(float heuristic)
{
	m_heuristic = heuristic;
}

float GraphNode::getHeuristic()
{
	return m_heuristic;
}

void GraphNode::setObstacle(bool obstacle)
{
	m_obstacle = obstacle;
}

bool GraphNode::getObstacle()
{
	return m_obstacle;
}



// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    This finds the arc in the current node that
//                  points to the node in the parameter.
//  Arguments:      The node that the arc connects to.
//  Return Value:   A pointer to the arc, or 0 if an arc doesn't
//                  exist from this to the specified input node.
// ----------------------------------------------------------------
GraphArc* GraphNode::getArc(GraphNode* node) {

	auto iter = m_arcList.begin();
	auto endIter = m_arcList.end();
	GraphArc* arc = 0;

	// find the arc that matches the node
	for (; iter != endIter && nullptr == arc; ++iter) {
		if ((*iter).node() == node) {
			arc = &((*iter));
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
void GraphNode::addArc(GraphNode * pNode, int pWeight)
{
	// Create a new arc.
	GraphArc a;
	a.setNode(pNode);
	a.setWeight(pWeight);
	// Add it to the arc list.
	m_arcList.push_back(a);
}



void GraphNode::removeArc(GraphNode * pNode)
{
	std::list<GraphArc>::const_iterator iter = m_arcList.begin();
	std::list<GraphArc>::const_iterator endIter = m_arcList.end();

	int size = m_arcList.size();
	// find the arc that matches the node
	for (; iter != endIter && m_arcList.size() == size;
		++iter) {
		if ((*iter).node() == pNode) {
			m_arcList.remove(*iter);
			break;
		}
	}
}

