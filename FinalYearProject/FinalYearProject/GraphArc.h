#ifndef GRAPHARC_H
#define GRAPHARC_H

#include "GraphNode.h"
#include <iostream>

class GraphNode;

// -------------------------------------------------------
// Name:        GraphArc
// Description: This is the arc class. The arc class
//              points to a graph node, and contains a 
//              weight.
// -------------------------------------------------------
class GraphArc {
private:

	// -------------------------------------------------------
	// Description: pointer to the node that the arc points to
	// -------------------------------------------------------
	GraphNode* m_pNode;

	// -------------------------------------------------------
	// Description: Weight of the arc
	// -------------------------------------------------------
	int m_weight;

	friend bool operator==(const GraphArc& lhs, const GraphArc& rhs)
	{
		return (lhs.m_pNode == rhs.m_pNode && lhs.m_weight == rhs.m_weight);
	}

public:
	GraphArc();
	~GraphArc();

	// Accessor functions
	GraphNode* node() const;

	int weight() const;

	// Manipulator functions
	void setNode(GraphNode* pNode);

	void setWeight(int weight);

};

#endif
