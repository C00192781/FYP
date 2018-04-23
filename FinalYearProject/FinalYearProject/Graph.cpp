#include "Graph.h"

struct {
	bool operator()(GraphNode * n1, GraphNode * n2)
	{
		float k1 = n1->getKey().x;
		float k2 = n1->getKey().y;

		float p1 = n2->getKey().x;
		float p2 = n2->getKey().y;

		std::vector<float> v1{ k1, k2 };
		std::vector<float> v2{ p1, p2 };

		return std::lexicographical_compare(v1.begin(), v1.end(),
			v2.begin(), v2.end());
	}
} pairCompare;

Graph::Graph()
{

}


// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
Graph::Graph(int size) : m_maxNodes(size)
{
	int i;
	m_pNodes = new GraphNode *[m_maxNodes];
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
Graph::~Graph()
{
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
bool Graph::addNode(std::pair<std::string, int> data, std::pair<std::string, int> rhsData, bool marked, int index, sf::Vector2f waypoint, sf::Vector2f key, float heuristic, sf::Vector2f goal) {
	bool nodeNotPresent = false;
	// find out if a node does not exist at that index.
	if (m_pNodes[index] == 0) {
		nodeNotPresent = true;
		// create a new node, put the data in it, and unmark it.
		m_pNodes[index] = new GraphNode;
		m_pNodes[index]->setData(data);
		m_pNodes[index]->setMarked(marked);
		m_pNodes[index]->setRhsData(rhsData);
		m_pNodes[index]->setWaypoint(waypoint);
		m_pNodes[index]->setKey(key);
		m_pNodes[index]->setGoal(goal);
		m_pNodes[index]->setHeuristic(heuristic);
	}

	return nodeNotPresent;
}
bool Graph::addNode(std::pair<std::string, int> data, std::pair<std::string, int> rhsData, bool marked, int index, sf::Vector2f waypoint)
{
	bool nodeNotPresent = false;
	// find out if a node does not exist at that index.
	if (m_pNodes[index] == 0) {
		nodeNotPresent = true;
		// create a new node, put the data in it, and unmark it.
		m_pNodes[index] = new GraphNode;
		m_pNodes[index]->setData(data);
		m_pNodes[index]->setMarked(marked);
		m_pNodes[index]->setRhsData(rhsData);
		m_pNodes[index]->setWaypoint(waypoint);
	}

	return nodeNotPresent;
}

bool Graph::addNode(GraphNode *node, int index) {
	bool nodeNotPresent = false;
	// find out if a node does not exist at that index.
	if (m_pNodes[index] == 0) {
		nodeNotPresent = true;
		std::cout << node->getWaypoint().x << std::endl;
		// create a new node, put the data in it, and unmark it.
		m_pNodes[index] = node;
		std::cout << m_pNodes[index]->getWaypoint().x << std::endl;
	}

	return nodeNotPresent;
}




// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
void Graph::removeNode(int index)
{
	// Only proceed if node does exist.
	if (m_pNodes[index] != 0) {
		// now find every arc that points to the node that
		// is being removed and remove it.        
		GraphArc* arc;
		arc = nullptr;

		//bool arcCheck = false;
		int weight;
		int counter = 0;
		// loop through every node
		for (int node = 0; node < m_maxNodes; node++) {
			// if the node is valid...
			if (m_pNodes[node] != 0) {
				// see if the node has an arc pointing to the current node.
				arc = m_pNodes[node]->getArc(m_pNodes[index]);
			}
			// if it has an arc pointing to the current node, then
			// remove the arc.
			if (arc != nullptr)
			{
				counter++;
				//std::cout << "counter" << counter << std::endl;
				//std::cout << arc->weight() << std::endl;
				GraphNode nodeObject = *(m_pNodes[index]);
				//std::cout << " " << nodeObject.data().first << std::endl;
				obstacleMap.insert(std::make_pair(index, nodeObject));
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
bool Graph::addArc(int from, int to, int weight)
{
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
void Graph::removeArc(int from, int to)
{
	// Make sure that the node exists before trying to remove
	// an arc from it.
	bool nodeExists = true;
	if (m_pNodes[from] == 0 || m_pNodes[to] == 0)
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
GraphArc * Graph::getArc(int from, int to)
{
	GraphArc* arc = 0;
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
void Graph::clearMarks() {
	for (int index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0)
		{
			m_pNodes[index]->setMarked(false);
		}
	}
}


void Graph::LPAStarInitialize(GraphNode * pStart, GraphNode * pDest)
{
	searchType = "LPA*";
	if (pStart != 0)
	{
		/////////// 3
		//////////// for all s ∈ S rhs(s) = g(s) = ∞;
		// setting the initial values of all of the nodes
		for (int i = 0; i < m_maxNodes; i++)
		{
			m_pNodes[i]->setGoal(pDest->getWaypoint());
			// heuristics calulcated using the maximum of the absolute differences between the x and y coordinates
			float heuristic = CalculateHeuristic(m_pNodes[i], false);
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
		initialKey.x = CalculateHeuristic(pStart, false);
		initialKey.y = 0;
		pStart->setKey(initialKey);
		// insert starting node into the queue
		nodeQueue.insert(nodeQueue.begin(), pStart);

		//set as being marked/visited
		pStart->setMarked(true);
	}
}


void Graph::UpdateVertex(GraphNode *node, GraphNode * pStart)
{
	int min = 0;
	std::vector<int> predecessors;

	if (node != pStart)
	{
		//std::cout << "starting node" << pStart->data().first << std::endl;
		list<GraphArc>::const_iterator iter = node->arcList().begin();
		list<GraphArc>::const_iterator endIter = node->arcList().end();

		// for each iteration though the nodes
		for (; iter != endIter; iter++)
		{
			int distance = (*iter).node()->data().second + iter->weight();

			predecessors.push_back(distance);

			if (predecessors.size() == 1)
			{
				min = predecessors.front();
			}
			if (predecessors.size() > 1)
			{
				min = *std::min_element(predecessors.begin(), predecessors.end()); 
			}
		}
		node->setRhsData(pair<string, int>(node->rhsData().first, min));


		if (std::find(nodesToExamine.begin(), nodesToExamine.end(), stoi(node->data().first)) != nodesToExamine.end())
		{

		}
		else
		{
			nodesToExamine.push_back(stoi(node->data().first));

		}
	}

	// Remove 'node' from the priority queue only if it is present.
	nodeQueue.erase(std::remove_if(nodeQueue.begin(), nodeQueue.end(), [node](auto nodeInVector) { return node == nodeInVector;  }), nodeQueue.end());

	// if g-value != rhs value 
	if (node->data().second != node->rhsData().second)
	{
		node->setKey(CalculateKey(node, "LPA*"));
		node->setMarked(true);
	

		nodeQueue.push_back(node);
		std::sort(nodeQueue.begin(), nodeQueue.end(), pairCompare);
	}
}


void Graph::ComputeShortestPath(GraphNode * pStart, GraphNode * pDest)
{
	// while (U.TopKey()<˙ CalculateKey(sgoal) OR rhs(sgoal) 6= g(sgoal))
	// u = U.Pop();
	// if (g(u) > rhs(u))
	// g(u) = rhs(u);
	// for all s ∈ succ(u) UpdateVertex(s);
	// else
	// g(u) = ∞;
	// for all s ∈ succ(u) ∪ {u} UpdateVertex(s);

	// clear and reset these to return correct path and correct statistics
	path.clear();
	cellExpansions = 0;
	pathLength = 0;

	if (nodeQueue.size() > 0)
	{
		std::sort(nodeQueue.begin(), nodeQueue.end(), pairCompare);
		pDest->setKey(CalculateKey(pDest, "LPA*"));
		
		while (keyComparer(nodeQueue.front(), pDest) == true || pDest->rhsData().second != pDest->data().second)
		{
			GraphNode * node = nodeQueue.front();
			nodeQueue.erase(std::remove(nodeQueue.begin(), nodeQueue.end(), nodeQueue.front()), nodeQueue.end());

			if (node->data().second > node->rhsData().second)
			{
				auto data = node->data();
				data.second = node->rhsData().second;
				node->setData(data);

				cellExpansions++;

				std::list<GraphArc>::const_iterator iter = node->arcList().begin();
				std::list<GraphArc>::const_iterator endIter = node->arcList().end();

				// for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					UpdateVertex((*iter).node(), pStart);
				}
			}
			else 
			{
				// g(u) = ∞;
				auto data = node->data();
				data.second = std::numeric_limits<int>::max() - 100000;
				node->setData(data);

				cellExpansions++;

				// for all s ∈ succ(u) ∪ {u} UpdateVertex(s);
				UpdateVertex(node, pStart);

				std::list<GraphArc>::const_iterator iter = node->arcList().begin();
				std::list<GraphArc>::const_iterator endIter = node->arcList().end();

				// for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					UpdateVertex((*iter).node(), pStart);
				}
			}
		}
	}

	for (int i = 0; i < nodesToExamine.size(); i++)
	{
		if (m_pNodes[nodesToExamine.at(i)] == nullptr)
		{
			nodesToExamine.erase(nodesToExamine.begin() + i);
		}
	}

	GraphNode* current = pDest;
	int max = pDest->data().second / 100;
	for (int y = 0; y < max; y++)
	{
		for (int i = 0; i < nodesToExamine.size(); i++)
		{
			list<GraphArc>::iterator iter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().begin();
			list<GraphArc>::iterator endIter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().end();

			for (; iter2 != endIter2; iter2++)
			{
				if (m_pNodes[nodesToExamine.at(i)]->data().second == (current->data().second - 100) && iter2->node() == current)
				{
					//std::cout << "Pushed: " << nodesToExamine.at(i) << std::endl;
					path.push_back(m_pNodes[nodesToExamine.at(i)]);
					//std::cout << m_pNodes[nodesToExamine.at(i)]->data().first << std::endl;
					current = m_pNodes[nodesToExamine.at(i)];
				}
			}
		}
	}	

	std::reverse(path.begin(), path.end());
	path.push_back(pDest);
	pathLength = path.size(); 
}



void Graph::SetObstacle(int node, bool obstacle, int start)
{	
	// obstacle = true means that we are removing a node/making it an obstacle
	if (obstacle == true)
	{
		if (searchType == "A*")
		{
			// check which nodes need to be updated after node is removed
			std::vector<int> nodesToUpdate;

			// iterate through arcs of the nodes connected to the obstacle node
			// we do this so we can set the weight of the node arcs going in to the obstacle node
			list<GraphArc>::iterator iter2 = m_pNodes[node]->arcList2().begin();
			list<GraphArc>::iterator endIter2 = m_pNodes[node]->arcList2().end();

			m_pNodes[node]->m_outArcList.clear();
			for (; iter2 != endIter2; iter2++)
			{
				// if the arc is attached to the obstacle node (could also point to other nodes),
				// set the weight
				m_pNodes[node]->m_outArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), (*iter2).weight()));

				nodesToUpdate.push_back(stoi((*iter2).node()->data().first));
			}

			// clear to make sure arcs not added to list multiple times
			m_pNodes[node]->m_inArcList.clear();

			for (int i = 0; i < nodesToUpdate.size(); i++)
			{
				list<GraphArc>::iterator connectedIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().begin();
				list<GraphArc>::iterator connectedEndIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().end();

				for (; connectedIter != connectedEndIter; connectedIter++)
				{
					if ((*connectedIter).node()->data().first == m_pNodes[node]->data().first)
					{
						m_pNodes[node]->m_inArcList.push_back(std::make_pair(stoi(m_pNodes[nodesToUpdate.at(i)]->data().first), (*connectedIter).weight()));

					}
				}
			}

			
			// remove this node and the connected arcs
			removeNode(node);

		}
		else if (searchType == "LPA*")
		{
			// check which nodes need to be updated after node is removed
			std::vector<int> nodesToUpdate;

			// iterate through arcs of the nodes connected to the obstacle node
			// we do this so we can set the weight of the node arcs going in to the obstacle node
			list<GraphArc>::iterator iter2 = m_pNodes[node]->arcList2().begin();
			list<GraphArc>::iterator endIter2 = m_pNodes[node]->arcList2().end();

			m_pNodes[node]->m_outArcList.clear();
			for (; iter2 != endIter2; iter2++)
			{
				// if the arc is attached to the obstacle node (could also point to other nodes),
				// set the weight
				m_pNodes[node]->m_outArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), (*iter2).weight()));

				nodesToUpdate.push_back(stoi((*iter2).node()->data().first));
			}

			// clear to make sure arcs not added to list multiple times
			m_pNodes[node]->m_inArcList.clear();

			for (int i = 0; i < nodesToUpdate.size(); i++)
			{
				list<GraphArc>::iterator connectedIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().begin();
				list<GraphArc>::iterator connectedEndIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().end();

				for (; connectedIter != connectedEndIter; connectedIter++)
				{
					if ((*connectedIter).node()->data().first == m_pNodes[node]->data().first)
					{
						m_pNodes[node]->m_inArcList.push_back(std::make_pair(stoi(m_pNodes[nodesToUpdate.at(i)]->data().first), (*connectedIter).weight()));

					}
				}
			}

			GraphNode * temp = m_pNodes[node];
			nodeQueue.erase(std::remove_if(nodeQueue.begin(), nodeQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), nodeQueue.end());

			// remove nodes and the connected arcs
			removeNode(node);

			// update the nodes that were connected to the removed node 
			for (int i = 0; i < nodesToUpdate.size(); i++)
			{
				UpdateVertex(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
			}
		}
		else if (searchType == "AD*")
		{
			// check which nodes need to be updated after node is removed
			std::vector<int> nodesToUpdate;

			// iterate through arcs of the nodes connected to the obstacle node
			// we do this so we can set the weight of the node arcs going in to the obstacle node
			list<GraphArc>::iterator iter2 = m_pNodes[node]->arcList2().begin();
			list<GraphArc>::iterator endIter2 = m_pNodes[node]->arcList2().end();

			m_pNodes[node]->m_outArcList.clear();
			for (; iter2 != endIter2; iter2++)
			{
				// if the arc is attached to the obstacle node (could also point to other nodes),
				// set the weight

				m_pNodes[node]->m_outArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), (*iter2).weight()));

				nodesToUpdate.push_back(stoi((*iter2).node()->data().first));
			}

			// clear to make sure arcs not added to list multiple times
			m_pNodes[node]->m_inArcList.clear();

			for (int i = 0; i < nodesToUpdate.size(); i++)
			{
				list<GraphArc>::iterator connectedIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().begin();
				list<GraphArc>::iterator connectedEndIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().end();

				for (; connectedIter != connectedEndIter; connectedIter++)
				{
					if ((*connectedIter).node()->data().first == m_pNodes[node]->data().first)
					{
						m_pNodes[node]->m_inArcList.push_back(std::make_pair(stoi(m_pNodes[nodesToUpdate.at(i)]->data().first), (*connectedIter).weight()));
					}
				}
			}

			GraphNode * temp = m_pNodes[node];
			openQueue.erase(std::remove_if(openQueue.begin(), openQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), openQueue.end());
			closedQueue.erase(std::remove_if(closedQueue.begin(), closedQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), closedQueue.end());
			inconsQueue.erase(std::remove_if(inconsQueue.begin(), inconsQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), inconsQueue.end());

			bool marked = m_pNodes[node]->marked();

			// remove nodes and the connected arcs
			removeNode(node);

			// update the nodes that were connected to the removed node 
			for (int i = 0; i < nodesToUpdate.size(); i++)
			{
				if (marked == true)
				ADStarUpdateState(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
			}
		}
		
	}
	else
	{
		if (searchType == "A*")
		{
			// iterate through obstacle map
			// should only have one item in it at a time
			std::map<int, GraphNode>::iterator it = obstacleMap.begin();
			std::map<int, GraphNode>::iterator end = obstacleMap.end();

			for (; it != end; it++)
			{
				if (stoi(it->second.data().first) == node)
				{
					// add back in the node
					addNode(it->second.data(), it->second.rhsData(), it->second.marked(), node, it->second.getWaypoint(), it->second.getKey(), it->second.getHeuristic(), it->second.getGoal());

					std::cout << m_pNodes[node]->arcList2().size() << std::endl;
					std::list<std::pair<int, int>>::iterator start = it->second.m_inArcList.begin();
					std::list<std::pair<int, int>>::iterator theend = it->second.m_inArcList.end();

					// add back in the arcs
					for (; start != theend; start++)
					{
						//std::cout << start->first << std::endl;
						addArc(start->first, node, start->second);
					}

					std::list<std::pair<int, int>>::iterator start2 = it->second.m_outArcList.begin();
					std::list<std::pair<int, int>>::iterator theend2 = it->second.m_outArcList.end();

					// add back in the arcs
					for (; start2 != theend2; start2++)
					{
						//std::cout << start2->first << std::endl;
						addArc(node, start2->first, start2->second);
					}
				}
			}

			std::map<int, GraphNode>::iterator removal;
			removal = obstacleMap.find(node);

			obstacleMap.erase(removal);
		}
		else if (searchType == "LPA*")
		{
			// iterate through obstacle map
			std::map<int, GraphNode>::iterator it = obstacleMap.begin();
			std::map<int, GraphNode>::iterator end = obstacleMap.end();

			for (; it != end; it++)
			{
				if (stoi(it->second.data().first) == node)
				{
					// add back in the node
					addNode(it->second.data(), it->second.rhsData(), it->second.marked(), node, it->second.getWaypoint(), it->second.getKey(), it->second.getHeuristic(), it->second.getGoal());

					//std::cout << m_pNodes[node]->arcList2().size() << std::endl;
					std::list<std::pair<int, int>>::iterator start = it->second.m_inArcList.begin();
					std::list<std::pair<int, int>>::iterator theend = it->second.m_inArcList.end();

					// add back in the arcs
					for (; start != theend; start++)
					{
						//std::cout << start->first << std::endl;
						addArc(start->first, node, start->second);
					}

					std::list<std::pair<int, int>>::iterator start2 = it->second.m_outArcList.begin();
					std::list<std::pair<int, int>>::iterator theend2 = it->second.m_outArcList.end();

					// add back in the arcs
					for (; start2 != theend2; start2++)
					{
						std::cout << start2->first << std::endl;
						addArc(node, start2->first, start2->second);
					}
				}
			}

			std::map<int, GraphNode>::iterator removal;
			removal = obstacleMap.find(node);

			obstacleMap.erase(removal);

			list<GraphArc>::iterator iter5 = m_pNodes[node]->arcList2().begin();
			list<GraphArc>::iterator endIter5 = m_pNodes[node]->arcList2().end();

			for (; iter5 != endIter5; iter5++)
			{
				//std::cout << (iter5->node()->data().first) << std::endl;
				UpdateVertex((*iter5).node(), m_pNodes[start]);
			}
		}

		else if (searchType == "AD*")
		{
			// iterate through obstacle map
			std::map<int, GraphNode>::iterator it = obstacleMap.begin();
			std::map<int, GraphNode>::iterator end = obstacleMap.end();

			for (; it != end; it++)
			{
				if (stoi(it->second.data().first) == node)
				{
					// add the node node back into the graph
					addNode(it->second.data(), it->second.rhsData(), it->second.marked(), node, it->second.getWaypoint(), it->second.getKey(), it->second.getHeuristic(), it->second.getGoal());

					//std::cout << m_pNodes[node]->arcList2().size() << std::endl;
					std::list<std::pair<int, int>>::iterator start = it->second.m_inArcList.begin();
					std::list<std::pair<int, int>>::iterator theend = it->second.m_inArcList.end();

					// add back in the arcs
					for (; start != theend; start++)
					{
						addArc(start->first, node, start->second);
					}

					std::list<std::pair<int, int>>::iterator start2 = it->second.m_outArcList.begin();
					std::list<std::pair<int, int>>::iterator theend2 = it->second.m_outArcList.end();

					// add back in the arcs
					for (; start2 != theend2; start2++)
					{
						//std::cout << start2->first << std::endl;
						addArc(node, start2->first, start2->second);
					}
				}
			}

			std::map<int, GraphNode>::iterator removal;
			removal = obstacleMap.find(node);

			obstacleMap.erase(removal);

			list<GraphArc>::iterator iter5 = m_pNodes[node]->arcList2().begin();
			list<GraphArc>::iterator endIter5 = m_pNodes[node]->arcList2().end();

			for (; iter5 != endIter5; iter5++)
			{
				//std::cout << (iter5->node()->data().first) << std::endl;
				ADStarUpdateState((*iter5).node(), m_pNodes[start]);
			}
		}
	}
}



float Graph::CalculateHeuristic(GraphNode * node, bool obstacle)
{
	float dx, dy = 0;

	dx = abs(node->getWaypoint().x - node->getGoal().x);
	dy = abs(node->getWaypoint().y - node->getGoal().y);

	float result = std::max(dx, dy);
	std::cout << "heuristic result: " << result << " " << node->data().first << std::endl;

	return result;
}

sf::Vector2f Graph::CalculateKey(GraphNode * node, std::string searchType)
{
	sf::Vector2f key;
	float gs, rhs;
	float k1, k2;

	gs = node->data().second;
	rhs = node->rhsData().second;

	if (searchType == "LPA*")
	{
		k1 = std::min(gs, rhs) + node->getHeuristic();
		k2 = std::min(gs, rhs);
		key = sf::Vector2f{ k1, k2 };
	}

	if (searchType == "AD*")
	{
		if (gs > rhs)
		{
			k1 = rhs + getInflation() * node->getHeuristic();
			k2 = rhs;
		}
		else
		{
			k1 = gs + node->getHeuristic();
			k2 = gs;
		}

		key = sf::Vector2f{ k1, k2 };
	}


	return key;
}


bool Graph::keyComparer(GraphNode* n1, GraphNode* n2)
{
	
	if (searchType == "AD*")
	{
		n1->setKey(CalculateKey(n1, "AD*"));
		n2->setKey(CalculateKey(n2, "AD*"));
	}
	

	float k1 = n1->getKey().x;
	float k2 = n1->getKey().y;

	float p1 = n2->getKey().x;
	float p2 = n2->getKey().y;

	std::vector<float> v1{ k1, k2 };
	std::vector<float> v2{ p1, p2 };

	bool check = false;

	if (k1 < p1)
	{
		check = true;
	}
	else if (p1 < k1)
	{
		check = false;
	}
	else if (k2 < p2)
	{
		check = true;
	}
	else if (p2 < k2)
	{
		check = true;
	}

	return check;
}



void Graph::ADStarInitialize(GraphNode * pStart, GraphNode * pDest, std::vector<GraphNode*>& path, float inflation)
{
	searchType = "AD*";
	setInflation(inflation);
	// g(sstart) = rhs(sstart) = ∞; g(sgoal) = ∞;
	// setting the initial values of all of the nodes
	for (int i = 0; i < m_maxNodes; i++)
	{
		//std::cout << m_pNodes[i]->getWaypoint().x << std::endl;
		m_pNodes[i]->setGoal(pStart->getWaypoint());

		float heuristic = CalculateHeuristic(m_pNodes[i], false);
		m_pNodes[i]->setHeuristic(heuristic);

		//std::cout << "HEURISTIC: " << heuristic << std::endl;

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
	//std::cout << "clearing queues" << std::endl;

	openQueue.clear();
	closedQueue.clear();
	inconsQueue.clear();

	//std::cout << "queues cleared" << std::endl;

	pDest->setRhsData(pair<string, int>(pDest->rhsData().first, 0));
	sf::Vector2f initialKey;

	initialKey = CalculateKey(pDest, "AD*");
	pDest->setKey(initialKey);
	//set as being marked/visited
	pDest->setMarked(true);
	
	openQueue.push_back(pDest);
	//std::cout << "Anytime Dynamic A* initialized" << std::endl;
}






void Graph::ADStarUpdateState(GraphNode * node, GraphNode * pDest)
{
	int min = 0;
	std::vector<int> successors;

	if (node->marked() == false)
	{
		auto data = node->data();
		data.second = std::numeric_limits<int>::max() - 100000;
		node->setData(data);
	}

	if (node != pDest)
	{
		list<GraphArc>::const_iterator iter = node->arcList().begin();
		list<GraphArc>::const_iterator endIter = node->arcList().end();

		// for each iteration though the nodes
		for (; iter != endIter; iter++)
		{
			int distance = iter->weight() + (*iter).node()->data().second;

			successors.push_back(distance);

			if (successors.size() == 1)
			{
				min = successors.front();
			}
			if (successors.size() > 1)
			{
				min = *std::min_element(successors.begin(), successors.end());
			}
		}

		node->setRhsData(pair<string, int>(node->rhsData().first, min));

		if (std::find(nodesToExamine.begin(), nodesToExamine.end(), stoi(node->data().first)) != nodesToExamine.end())
		{

		}
		else
		{
			nodesToExamine.push_back(stoi(node->data().first));

		}
	}

	// if (s ∈ OPEN) remove s from OPEN;
	// Remove 'node' from the priority queue only if it is present.
	if (openQueue.size() > 0)
	openQueue.erase(std::remove_if(openQueue.begin(), openQueue.end(), [node](auto nodeInVector) { return node == nodeInVector;  }), openQueue.end());


	// 09. if (g(s) != rhs(s))

	if (node->data().second != node->rhsData().second)
	{
		// if s6∈ CLOSED

		if (std::find(closedQueue.begin(), closedQueue.end(), node) != closedQueue.end())
		{
			// insert s into INCONS;
			inconsQueue.push_back(node);
		}
		else 
		{
			// insert s into OPEN with key(s);
			std::cout << node->data().first << std::endl;
			node->setKey(CalculateKey(node, "AD*"));
			node->setMarked(true);
			openQueue.push_back(node);
			std::sort(openQueue.begin(), openQueue.end(), pairCompare);
		}
	}
}



int Graph::ComputeOrImprovePath(GraphNode * pStart, GraphNode * pDest)
{
	tempStart = pStart;
	path.clear();
	cellExpansions = 0;
	pathLength = 0;

	setInflationIteration(ADStarInflation); 

	if (openQueue.size() > 0)
	{
		std::sort(openQueue.begin(), openQueue.end(), pairCompare);
		while (keyComparer(openQueue.front(), pStart) == true || pStart->rhsData().second != pStart->data().second)
		{
			// remove state s with the minimum key from OPEN;
			GraphNode * node = openQueue.front();
			openQueue.erase(std::remove(openQueue.begin(), openQueue.end(), openQueue.front()), openQueue.end());

			// if (g(s) > rhs(s))
			if (node->data().second > node->rhsData().second)
			{
				auto data = node->data();
				data.second = node->rhsData().second;
				// g(s) = rhs(s);
				node->setData(data);

				cellExpansions++;

				// CLOSED = CLOSED ∪ {s};
				closedQueue.push_back(node);

				// for all s' ∈ Pred(s) UpdateState(s');
				list<GraphArc>::const_iterator iter = node->arcList().begin();
				list<GraphArc>::const_iterator endIter = node->arcList().end();

				// for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					ADStarUpdateState((*iter).node(), pDest);
				}
			}
			else 
			{
				// g(s) = ∞;
				auto data = node->data();
				data.second = std::numeric_limits<int>::max() - 100000;
				node->setData(data);

				cellExpansions++;

				// for all s' ∈ Pred(s) ∪ { s } UpdateState(s');
				list<GraphArc>::const_iterator iter = node->arcList().begin();
				list<GraphArc>::const_iterator endIter = node->arcList().end();

				ADStarUpdateState(node, pDest);

				//for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					ADStarUpdateState((*iter).node(), pDest);
				}
			}
		}
	}

		std::cout << "Path Cost: " << pStart->data().second << std::endl;


		for (int i = 0; i < nodesToExamine.size(); i++)
		{
			if (m_pNodes[nodesToExamine.at(i)] == nullptr)
			{
				nodesToExamine.erase(nodesToExamine.begin() + i);
			}
		}

		GraphNode* current = pStart;
		int max = pStart->data().second / 100;
		for (int y = 0; y < max; y++)
		{
			for (int i = 0; i < nodesToExamine.size(); i++)
			{
				list<GraphArc>::iterator iter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().begin();
				list<GraphArc>::iterator endIter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().end();

				for (; iter2 != endIter2; iter2++)
				{
					if (m_pNodes[nodesToExamine.at(i)]->data().second == (current->data().second - 100) && iter2->node() == current)
					{
						//std::cout << "Pushed: " << nodesToExamine.at(i) << std::endl;
						path.push_back(m_pNodes[nodesToExamine.at(i)]);
						//std::cout << m_pNodes[nodesToExamine.at(i)]->data().first << std::endl;
						current = m_pNodes[nodesToExamine.at(i)];
					}
				}
			}
		}
		path.push_back(pDest);
		pathLength = path.size();
		return pDest->data().second;
}



void Graph::setInflation(float inflation)
{
	ADStarInflation = inflation;
}


float Graph::getInflation()
{
	return ADStarInflation;
}

void Graph::MoveStates()
{
	const int size = inconsQueue.size();
	//std::cout << "SIZE START OF LOOP: " << size << std::endl;
	
	while (inconsQueue.size() > 0)
	{
		std::cout << "SIZE: " << inconsQueue.size() << std::endl;
		GraphNode * node = inconsQueue.front();
		openQueue.push_back(node);
		inconsQueue.erase(std::remove(inconsQueue.begin(), inconsQueue.end(), inconsQueue.front()), inconsQueue.end());
	}

	const int openQueueSize = openQueue.size();
	if (openQueue.size() > 0)
	{
		for (int i = 0; i < openQueue.size(); i++)
		{
			openQueue.at(i)->setKey(CalculateKey(openQueue.at(i), "AD*"));
		}
	}
	closedQueue.clear();	
}

void Graph::InflationHandler()
{

}

void Graph::UCS(GraphNode * pStart, GraphNode * pDest, std::vector<GraphNode*>& path)
{
	std::priority_queue <GraphNode *, vector<GraphNode *>, NodeSearchCostComparer> queue;

	if (pStart != 0)
	{
		// insert starting node into the queue
		queue.push(pStart);

		// setting the initial values of all of the nodes
		for (int i = 0; i < m_maxNodes; i++)
		{
			if (m_pNodes[i] != nullptr)
			{
				auto data = m_pNodes[i]->data();
				// set the weight to an infinite value to start off with
				data.second = std::numeric_limits<int>::max() - 100000;
				m_pNodes[i]->setData(data);
			}
		}

		//set the starting node weight to 0
		pStart->setData(pair<string, int>(pStart->data().first, 0));
		//set is being marked/visited
		//pStart->setMarked(true);
	}



	// while the node queue size is not 0 and we haven't reached our Goal Node yet
	while (queue.size() != 0 && queue.top() != pDest)
	{
		//set up iterators
		list<GraphArc>::const_iterator iter = queue.top()->arcList().begin();
		list<GraphArc>::const_iterator endIter = queue.top()->arcList().end();

		// for each iteration though the nodes
		for (; iter != endIter; iter++)
		{
			// if the current node is not the highest priority node - THEN WE KNOW TO START ADDING UP DISTANCE
			if ((*iter).node() != queue.top())
			{
				// EACH TIME WE ITERATE THROUGH THE NODES WE ADD THE DISTANCE
				// distance = the distance of the current node + the distance added up so far 
				int distance = queue.top()->data().second + iter->weight();


				/////// FOR FINDING SHORTEST PATH
				// if the distance is less than the weight of the current node, i.e. we've found a shorter path
				if (distance < (*iter).node()->data().second)
				{
					// set the current node's values - same name, the shorter/new distance
					(*iter).node()->setData(pair<string, int>((*iter).node()->data().first, distance));
					// set the previous node as being the node with the shortest path
					(*iter).node()->setPrevious((queue.top()));
				}


				///////// FOR MARKING
				// if the node has not been marked
				if ((*iter).node()->marked() == false)
				{
					//(*iter).node()->setPrevious((nodeQueue.top()));
					// mark it as being true
					(*iter).node()->setMarked(true);
					// push the current node to the queue
					queue.push((*iter).node());
				}
			}
		}
		//////// REMOVE NODE
		// remove the node from the front of the queue
		queue.pop();
	}
}


// Initialize or reset nodes for AStar search
void Graph::InitializeAStar(GraphNode * pStart, std::priority_queue<GraphNode*, vector<GraphNode*>, NodeSearchCostComparer2> *queue)
{
	searchType = "A*";
	if (pStart != 0)
	{
		// insert starting node into the queue
		queue->push(pStart);

		// setting the initial values of all of the nodes
		for (int i = 0; i < m_maxNodes; i++)
		{
			// ESTIMATE, i.e. g(n)
			if (m_pNodes[i] != nullptr)
			{
				m_pNodes[i]->setEstimate(m_pNodes[i]->data().second*0.9);
				auto data = m_pNodes[i]->data();
				// set the weight to an infinite value to start off with
				data.second = std::numeric_limits<int>::max() - 100000;
				m_pNodes[i]->setData(data);
				m_pNodes[i]->setMarked(false);
			}
		}

		//set the starting node weight to 0
		pStart->setData(pair<string, int>(pStart->data().first, 0));
		//set as being marked/visited
		pStart->setMarked(true);

	}
}


// std::vector<GraphNode*>& path
void Graph::AStar(GraphNode * pStart, GraphNode * pDest)
{
	for (int i = 0; i < m_maxNodes; i++)
	{
		if (m_pNodes[i] != nullptr)
		{
			m_pNodes[i]->setPrevious(nullptr);
			m_pNodes[i]->setData(pair<string, int>(m_pNodes[i]->data().first, std::numeric_limits<int>::max() - 100000));
			m_pNodes[i]->setEstimate(m_pNodes[i]->data().second);
			m_pNodes[i]->setMarked(false);
		}
	}
	
	path.clear();
	cellExpansions = 0;
	pathLength = 0;

	//std::vector<GraphNode*> starPath;
	UCS(pDest, pStart, path);
	std::priority_queue <GraphNode*, vector<GraphNode*>, NodeSearchCostComparer2> queue;

	InitializeAStar(pStart, &queue);

	// while the node queue size is not 0 and we haven't reached our Goal Node yet
	while (queue.size() != 0 && queue.top() != pDest)
	{
		//set up iterators
		list<GraphArc>::const_iterator iter = queue.top()->arcList().begin();
		list<GraphArc>::const_iterator endIter = queue.top()->arcList().end();

		// for each iteration though the nodes
		for (; iter != endIter; iter++)
		{
			// if the current node is not the highest priority node - THEN WE KNOW TO START ADDING UP DISTANCE
			if ((*iter).node() != queue.top())
			{
				int distance = queue.top()->data().second + iter->weight();
				///// FOR FINDING SHORTEST PATH
				////if the distance is less than the weight of the current node, i.e. we've found a shorter path
				if (distance < (*iter).node()->data().second)
				{
					/// set the current node's values - same name, the shorter/new distance
					(*iter).node()->setData(pair<string, int>((*iter).node()->data().first, distance));
					///set the previous node as being the node with the shortest path
					(*iter).node()->setPrevious((queue.top()));
				}


				///////// FOR MARKING
				// if the node has not been marked
				if ((*iter).node()->marked() == false)
				{
					//(*iter).node()->setPrevious((nodeQueue.top()));
					// mark it as being true
					(*iter).node()->setMarked(true);
					cellExpansions++;
					// push the current node to the queue
					queue.push((*iter).node());
				}


				if ((*iter).node() == pDest)
				{
					if (distance <= (*iter).node()->data().second)
					{
						// set the current node's values - same name, the shorter/new distance
						(*iter).node()->setData(pair<string, int>((*iter).node()->data().first, distance));
						// set the previous node as being the node with the shortest path
						(*iter).node()->setPrevious((queue.top()));

						GraphNode* node = (*iter).node();
						path.push_back(node);

						// Retrieve pointer to the previous node 
						while (pStart != node)
						{
							// cycles through nodes from the goal
							// to the node before the goal
							// adding them to the back of the path vector.
							// So, the top of the path vector is the first node
							// that you need to travel to

							node = node->getPrevious();
							// Push this to the path vector
							path.push_back(node);
						}
					}
				}
			}
		}
		//////// REMOVE NODE
		// remove the node from the front of the queue
		queue.pop();
	}
	//std::cout << pDest->data().second << std::endl;
	//std::cout << path.size() << std::endl;
	std::reverse(path.begin(), path.end());
	pathLength = path.size();
}

int Graph::ADStarFindStart()
{
	int min = 0;

	//std::cout << "starting node" << pStart->data().first << std::endl;
	list<GraphArc>::const_iterator iter = tempStart->arcList().begin();
	list<GraphArc>::const_iterator endIter = tempStart->arcList().end();

	GraphNode * newStart = new GraphNode();

	int distance = std::numeric_limits<int>::max() - 100000;;

	//int distance;
	// for each iteration though the nodes
	for (; iter != endIter; iter++)
	{
		int distTest = (*iter).node()->data().second + iter->weight();
		if (distTest <= distance)
		{
			distance = distTest;
			newStart = (*iter).node();
		}
	}

	return stoi(newStart->data().first);
}

void Graph::setInflationIteration(float inflation)
{
	inflationIteration = inflation;
}

std::vector<GraphNode*> Graph::getPath()
{
	return path;
}

/// <summary>
/// The following 3 getters are used to enable data to be retrieved and logged
/// </summary>
/// <returns></returns>

int Graph::getCellExpansions()
{
	return cellExpansions;
}

int Graph::getPathLength()
{
	return pathLength;
}

float Graph::getInflationIteration()
{
	return inflationIteration;
}








