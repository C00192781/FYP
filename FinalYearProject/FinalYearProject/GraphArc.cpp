#include "GraphArc.h"

GraphArc::GraphArc()
{
}

GraphArc::~GraphArc()
{
}

GraphNode * GraphArc::node() const
{
	return m_pNode;
}

int GraphArc::weight() const
{
	return m_weight;
}

void GraphArc::setNode(GraphNode * pNode)
{
	m_pNode = pNode;
}

void GraphArc::setWeight(int weight)
{
	m_weight = weight;
}
