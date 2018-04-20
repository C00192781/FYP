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
			//std::cout << m_pNodes[i]->getWaypoint().x << std::endl;
			m_pNodes[i]->setGoal(pDest->getWaypoint());

			float heuristic = CalculateHeuristic(m_pNodes[i], false);
			m_pNodes[i]->setHeuristic(heuristic);

			std::cout << "HEURISTIC: " << m_pNodes[i]->getHeuristic() << std::endl;

			/*		if (m_pNodes[i]->getObstacle() == true)
			{
			continue;
			}
			else
			{*/
			auto data = m_pNodes[i]->data();
			auto rhsData = m_pNodes[i]->rhsData();
			// set the weight to an infinite value to start off with
			data.second = std::numeric_limits<int>::max() - 100000;
			// set the rhs values of all nodes to an infinite value
			rhsData.second = std::numeric_limits<int>::max() - 100000;


			m_pNodes[i]->setMarked(false);

			m_pNodes[i]->setData(data);
			m_pNodes[i]->setRhsData(rhsData);
			//}

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

	/*if (node->data().first == std::to_string(13))
	{
		std::cout << "13" << std::endl;
	}*/
	// std::vector<Node *> & nodeQueue
	int min = 0;
	std::vector<int> predecessors;

	if (node != pStart)
	{
		//std::cout << "starting node" << pStart->data().first << std::endl;
		list<GraphArc>::const_iterator iter = node->arcList().begin();
		list<GraphArc>::const_iterator endIter = node->arcList().end();


		//int distance;
		// for each iteration though the nodes
		for (; iter != endIter; iter++)
		{

			/*if ((*iter).node()->getObstacle() == true)
			{
			continue;
			}
			else
			{*/
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
			//}
		}

		std::cout << "rhs value: " << min << std::endl;
		//Commented out SetKey as it's not listed in algorithm
		// Something is missing from the instructions though
		//float heuristic = CalculateHeuristic(m_pNodes[i], false);
		//m_pNodes[i]->setHeuristic(heuristic
		//node->setKey(CalculateKey(node, "LPA*"));
		node->setRhsData(pair<string, int>(node->rhsData().first, min));

		if (std::find(nodesToExamine.begin(), nodesToExamine.end(), stoi(node->data().first)) != nodesToExamine.end())
		{

		}
		else
		{
			nodesToExamine.push_back(stoi(node->data().first));

		}

		//node->setKey(CalculateKey(node));
	}

	//std::cout << "TTTTTTTTTTEEEEEEEEEEEMPPPPPP" << node->temp << std::endl;
	//node->setPrevious(m_pNodes[node->temp]);

	// Remove 'node' from the priority queue only if it is present.
	nodeQueue.erase(std::remove_if(nodeQueue.begin(), nodeQueue.end(), [node](auto nodeInVector) { return node == nodeInVector;  }), nodeQueue.end());

	//if (node->getObstacle() == false)
	//{
	if (node->data().second != node->rhsData().second)
	{
		std::cout << node->data().first << std::endl;
		node->setKey(CalculateKey(node, "LPA*"));
		node->setMarked(true);
		
		/*if (nodeQueue.size() > 0)
		{
			node->setPrevious(nodeQueue.front());
		}*/
		//std::cout << "TTTTTTTTTTEEEEEEEEEEEMPPPPPP" << node->temp << std::endl;
		//node->setPrevious(m_pNodes[node->temp]);

		//std::cout << "TTTTTTTTTTEEEEEEEEEEEMPPPPPP" << node->temp << std::endl;
		//node->setPrevious(m_pNodes[node->temp]);

		nodeQueue.push_back(node);
		std::sort(nodeQueue.begin(), nodeQueue.end(), pairCompare);


		std::cout << "node pushed " << std::endl;
	}

	//std::cout << "NODE BEING UPDATED: " << node->data().first << std::endl;
	//for (int i = 0; i < m_maxNodes; i++)
	//{
	//	std::cout << i << ": G-Value " << m_pNodes[i]->data().second << std::endl;
	//	std::cout << i << ": RHS-Value " << m_pNodes[i]->rhsData().second << std::endl;
	//	//std::cout << i << ": Heuristic " << m_pNodes[i]->getHeuristic() << std::endl;
	//	std::cout << endl;
	//}

	//}
}


void Graph::ComputeShortestPath(GraphNode * pStart, GraphNode * pDest)
{
	//{09} while (U.TopKey()<˙ CalculateKey(sgoal) OR rhs(sgoal) 6= g(sgoal))
	//{10} u = U.Pop();
	//{11} if (g(u) > rhs(u))
	//{12} g(u) = rhs(u);
	//{13} for all s ∈ succ(u) UpdateVertex(s);
	//{14} else
	//{15} g(u) = ∞;
	//{16} for all s ∈ succ(u) ∪ {u} UpdateVertex(s);


	//**************************
	//m_pNodes[5]->setObstacle(true);

	/*removeNode(5);*/

	path.clear();

	if (nodeQueue.size() > 0)
	{
		std::sort(nodeQueue.begin(), nodeQueue.end(), pairCompare);
		// flag == false
		//CalculateKey(pDest, "LPA*");
		pDest->setKey(CalculateKey(pDest, "LPA*"));
		//std::cout << "GOAL KEY X: " << pDest->getKey().x << std::endl;
		//std::cout << "GOAL KEY Y: " << pDest->getKey().y << std::endl;
		
		while (keyComparer(nodeQueue.front(), pDest) == true || pDest->rhsData().second != pDest->data().second)
		{

		/*	for (int i = 0; i < m_maxNodes; i++)
			{
				std::cout << m_pNodes[i]->getKey().x;
			}*/


			/*if (nodeQueue.front() == pDest)
			{
				flag = true;
			}*/

			/*if (keyComparer(nodeQueue.front(), pDest) == true)
			{
			std::cout << "compare " << std::endl;
			}*/
			//std::cout << nodeQueue.size() << std::endl;
			GraphNode * node = nodeQueue.front();
			nodeQueue.erase(std::remove(nodeQueue.begin(), nodeQueue.end(), nodeQueue.front()), nodeQueue.end());

			//if (nodeQueue.size() > 0)
			//{
			//	std::sort(nodeQueue.begin(), nodeQueue.end(), pairCompare);
			//	node->setPrevious(nodeQueue.front());
			//	//path.push_back(node);
			//}

			//node->setPrevious(nodeQueue.front());
			//path.push_back(node);

			//while (pStart != node)
			//{
			//	node = node->getPrevious();
			//	path.push_back(node);
			//}



			std::cout << "Node in priority queue*************************************************: " << node->data().first << std::endl;
	/*		if (nodeQueue.size() > 0)
			{
				std::cout << "behind" << nodeQueue.front()->data().first << std::endl;
			}*/
			std::cout << "node popped" << std::endl;
			if (node->data().second > node->rhsData().second)
			{
				std::cout << node->data().first << " G-Value: " << node->data().second << std::endl;
				std::cout << node->data().first << " RHS-Value: " << node->rhsData().second << std::endl;
				auto data = node->data();
				data.second = node->rhsData().second;
				node->setData(data);
				//node->setMarked(false);

				std::cout << node->data().first << " G > RHS" << std::endl;
				std::cout << node->data().first << " G-Value: " << node->data().second << std::endl;
				std::cout << node->data().first << " RHS-Value: " << node->rhsData().second << std::endl;
				std::cout << endl;


				//Node node = *nodeQueue->top();

				std::list<GraphArc>::const_iterator iter = node->arcList().begin();
				std::list<GraphArc>::const_iterator endIter = node->arcList().end();

				// for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					//if ((*iter).node()->getObstacle() == false)
					//{
					//(*iter).node()->setPrevious(node);
					//(*iter).node()->temp = stoi(node->data().first);
					UpdateVertex((*iter).node(), pStart);
					//}
				}
			}
			else //RP {14} else
			{
				std::cout << node->data().first << " G-Value: " << node->data().second << std::endl;
				std::cout << node->data().first << " RHS-Value: " << node->rhsData().second << std::endl;

				// RP
				//{15} g(u) = ∞;
				auto data = node->data();
				data.second = std::numeric_limits<int>::max() - 100000;
				node->setData(data);

				std::cout << node->data().first << " RHS >= G" << std::endl;
				std::cout << node->data().first << " G-Value: " << node->data().second << std::endl;
				std::cout << node->data().first << " RHS-Value: " << node->rhsData().second << std::endl;
				std::cout << endl;

				// RP
				//{16} for all s ∈ succ(u) ∪ {u} UpdateVertex(s);
				//if (node->getObstacle() == false)
				//{
				UpdateVertex(node, pStart);
				//}

				std::list<GraphArc>::const_iterator iter = node->arcList().begin();
				std::list<GraphArc>::const_iterator endIter = node->arcList().end();

				// for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					//if ((*iter).node()->getObstacle() == false)
					//{
					//(*iter).node()->setPrevious(node);
					//(*iter).node()->temp = stoi(node->data().first);
					UpdateVertex((*iter).node(), pStart);
					//}
				}
			}
			//pDest->setKey(CalculateKey(pDest, "LPA*"));


			for (int i = 0; i < m_maxNodes; i++)
			{
				if (m_pNodes[i] != nullptr)
				{
					std::cout << i << ": G-Value " << m_pNodes[i]->data().second << std::endl;
					std::cout << i << ": RHS-Value " << m_pNodes[i]->rhsData().second << std::endl;
					std::cout << i << ": Marked " << m_pNodes[i]->marked() << std::endl;
					std::cout << endl;
				}
			}

		}

		std::cout << std::endl;
		if (nodeQueue.size() > 0)
		{
			std::cout << nodeQueue.front()->data().first << std::endl;
		}
		std::cout << "Path Cost: " << pDest->data().second << std::endl;
		std::cout << "Path Size: " << path.size() << std::endl;
		//**********************
		//nodeQueue.clear();
		//nodeQueue.push_back(pStart);
		/*std::cout << std::endl;
		sf::Vector2f key = sf::Vector2f{ (float)std::numeric_limits<int>::max() - 100000 , (float)std::numeric_limits<int>::max() - 100000 };
		pDest->setKey(key);*/
		/*for (int i = 0; i < m_maxNodes; i++)
		{
			if (i != 16)
			{
				sf::Vector2f key = sf::Vector2f{ (float)std::numeric_limits<int>::max() - 100000 , (float)std::numeric_limits<int>::max() - 100000 };
				m_pNodes[i]->setKey(key);
			}
		}*/


	/*	for (int i = 0; i < m_maxNodes; i++)
		{
			if (m_pNodes[i]->getPrevious() != nullptr)
			std::cout << "Node: " << i << " " << m_pNodes[i]->getPrevious()->data().first << std::endl;
		}*/
		//std::cout << "Previous: " << pDest->getPrevious()->data().first << std::endl;
		//std::cout << "Previous: " << m_pNodes[19]->getPrevious()->data().first << std::endl;
		//std::cout << "Previous: " << pDest->getPrevious()->getPrevious()->getPrevious()->data().first << std::endl;
		//std::cout << "Previous: " << pDest->getPrevious()->getPrevious()->getPrevious()->getPrevious()->data().first << std::endl;
	}


	//std::cout << pDest->getHeuristic() << std::endl;



	for (int i = 0; i < nodesToExamine.size(); i++)
	{
		if (m_pNodes[nodesToExamine.at(i)] == nullptr)
		{

			nodesToExamine.erase(nodesToExamine.begin() + i);
			//obstacleMap.erase(removal);
		}
	}

	std::cout << "nodes to examine: " << nodesToExamine.size() << std::endl;


	GraphNode* current = pDest;
	int max = pDest->data().second / 100;
	for (int y = 0; y < max; y++)
	{
		for (int i = 0; i < nodesToExamine.size(); i++)
		{



			/*if (m_pNodes[nodesToExamine.at(i)] != pDest)
			{*/
			/*if (current == pStart)
			{
				break;
			}
*/
			list<GraphArc>::iterator iter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().begin();
			list<GraphArc>::iterator endIter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().end();


			for (; iter2 != endIter2; iter2++)
			{
				if (m_pNodes[nodesToExamine.at(i)]->data().second == (current->data().second - 100) && iter2->node() == current)
				{
					std::cout << "Pushed: " << nodesToExamine.at(i) << std::endl;
					path.push_back(m_pNodes[nodesToExamine.at(i)]);
					std::cout << m_pNodes[nodesToExamine.at(i)]->data().first << std::endl;
					current = m_pNodes[nodesToExamine.at(i)];
				}
			}
			/*}*/
		}
	}
	

	std::cout << path.size() << std::endl;
}



void Graph::SetObstacle(int node, bool obstacle, int start)
{	

	//float heuristic = CalculateHeuristic(m_pNodes[node], true);
	//m_pNodes[node]->setHeuristic(heuristic);

	if (obstacle == true)
	{
		if (searchType == "LPA*")
		{
			// !!!!!
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
				//if ((*iter2).node()->data().first == m_pNodes[node]->data().first)
				//{
				////(*iter2).setWeight(weight);
				//	m_pNodes[node]->m_inArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), *(*iter2).node()));
				//}
				m_pNodes[node]->m_outArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), (*iter2).weight()));
				//m_outArcList

				nodesToUpdate.push_back(stoi((*iter2).node()->data().first));
			}

			// !!!!
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

			std::cout << "m_inArcList size : " << m_pNodes[node]->m_inArcList.size() << std::endl;
			std::cout << "m_outArcList size : " << m_pNodes[node]->m_outArcList.size() << std::endl;

			GraphNode * temp = m_pNodes[node];
			nodeQueue.erase(std::remove_if(nodeQueue.begin(), nodeQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), nodeQueue.end());

			// !!!!!
			// remove nodes and the connected arcs
			removeNode(node);

			//std::map<int, GraphNode>::iterator it = obstacleMap.begin();
			////std::cout << "obstacle map test" << it->second.m_inArcList.size() << std::endl;

			// !!!! 
			// update the nodes that were connected to the removed node 
			for (int i = 0; i < nodesToUpdate.size(); i++)
			{

				UpdateVertex(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
				/*sf::Vector2f key = sf::Vector2f{ (float)(std::numeric_limits<int>::max() - 100000) , (float)(std::numeric_limits<int>::max() - 100000) };
				m_pNodes[nodesToUpdate.at(i)]->setKey(key);*/
			}
		}
		else if (searchType == "AD*")
		{
			// !!!!!
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
				//if ((*iter2).node()->data().first == m_pNodes[node]->data().first)
				//{
				////(*iter2).setWeight(weight);
				//	m_pNodes[node]->m_inArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), *(*iter2).node()));
				//}
				m_pNodes[node]->m_outArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), (*iter2).weight()));
				//m_outArcList

				nodesToUpdate.push_back(stoi((*iter2).node()->data().first));
			}

			// !!!!
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

			std::cout << "m_inArcList size : " << m_pNodes[node]->m_inArcList.size() << std::endl;
			std::cout << "m_outArcList size : " << m_pNodes[node]->m_outArcList.size() << std::endl;

			GraphNode * temp = m_pNodes[node];
			//nodeQueue.erase(std::remove_if(nodeQueue.begin(), nodeQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), nodeQueue.end());
			openQueue.erase(std::remove_if(openQueue.begin(), openQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), openQueue.end());
			closedQueue.erase(std::remove_if(closedQueue.begin(), closedQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), closedQueue.end());
			inconsQueue.erase(std::remove_if(inconsQueue.begin(), inconsQueue.end(), [temp](auto nodeInVector) { return temp == nodeInVector;  }), inconsQueue.end());

			bool marked = m_pNodes[node]->marked();

			// !!!!!
			// remove nodes and the connected arcs
			removeNode(node);

			//std::map<int, GraphNode>::iterator it = obstacleMap.begin();
			////std::cout << "obstacle map test" << it->second.m_inArcList.size() << std::endl;

			// !!!! 
			// update the nodes that were connected to the removed node 
			for (int i = 0; i < nodesToUpdate.size(); i++)
			{
				if (marked == true)
				ADStarUpdateState(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
				/*sf::Vector2f key = sf::Vector2f{ (float)(std::numeric_limits<int>::max() - 100000) , (float)(std::numeric_limits<int>::max() - 100000) };
				m_pNodes[nodesToUpdate.at(i)]->setKey(key);*/
			}
		}
		
	}
	else
	{
		if (searchType == "LPA*")
		{
			// !!!!
			// iterate through obstacle map
			// should only have one item in it at a time
			std::map<int, GraphNode>::iterator it = obstacleMap.begin();
			std::map<int, GraphNode>::iterator end = obstacleMap.end();

			// !!!
			// Assuming done one at a time
			/*for (; it != end; it++)

			{*/
			//GraphNode nodeObject = *(&(it->second));

			for (; it != end; it++)
			{
				if (stoi(it->second.data().first) == node)
				{
					std::cout << it->second.getWaypoint().x << " " << it->second.getWaypoint().y << std::endl;
					std::cout << stoi(it->second.data().first) << std::endl;

					// !!!!
					// add back in the node
					addNode(it->second.data(), it->second.rhsData(), it->second.marked(), node, it->second.getWaypoint(), it->second.getKey(), it->second.getHeuristic(), it->second.getGoal());
					//GraphNode nodeObject = *(m_pNodes[index]);


					//GraphNode nodeObject = *(&(it->second));
					//addNode(&nodeObject, stoi(it->second.data().first));
					std::cout << m_pNodes[node]->arcList2().size() << std::endl;
					std::list<std::pair<int, int>>::iterator start = it->second.m_inArcList.begin();
					std::list<std::pair<int, int>>::iterator theend = it->second.m_inArcList.end();

					// add back in the arcs
					for (; start != theend; start++)
					{
						std::cout << start->first << std::endl;
						addArc(start->first, node, start->second);
						// added because I couldn't fully copy node 
						//addArc(node, start->first, start->second);
					}


					std::list<std::pair<int, int>>::iterator start2 = it->second.m_outArcList.begin();
					std::list<std::pair<int, int>>::iterator theend2 = it->second.m_outArcList.end();

					// add back in the arcs
					for (; start2 != theend2; start2++)
					{
						std::cout << start2->first << std::endl;
						addArc(node, start2->first, start2->second);
						// added because I couldn't fully copy node 
						//addArc(node, start->first, start->second);
					}
					std::cout << "test" << std::endl;

				}
			}

			// !!!!
			// clear the obstacle map as we've gotten the necessary data now
			// ensures only one item is in it at any one time
			//obstacleMap.clear();

			//openQueue.erase(std::remove_if(openQueue.begin(), openQueue.end(), [node](auto nodeInVector) { return node == nodeInVector;  }), openQueue.end());
			//obstacleMap.erase(std::remove_if(obstacleMap.begin(), obstacleMap.end(), [node](auto nodeInVector) { return node == nodeInVector;  }), openQueue.end());

			std::map<int, GraphNode>::iterator removal;
			removal = obstacleMap.find(node);

			obstacleMap.erase(removal);



			std::cout << m_pNodes[node]->getWaypoint().x << " " << m_pNodes[node]->getWaypoint().y << std::endl;
			std::cout << m_pNodes[node]->arcList2().size() << std::endl;

			// *********************************************************
			std::cout << "Outgoing arcs: " << m_pNodes[node]->arcList2().size() << std::endl;
			list<GraphArc>::iterator iter5 = m_pNodes[node]->arcList2().begin();
			list<GraphArc>::iterator endIter5 = m_pNodes[node]->arcList2().end();

			for (; iter5 != endIter5; iter5++)
			{
				std::cout << (iter5->node()->data().first) << std::endl;
				UpdateVertex((*iter5).node(), m_pNodes[start]);
				//UpdateVertex(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
			}

			std::cout << "Outgoing arcs: " << m_pNodes[node]->arcList2().size() << std::endl;

			//std::cout << "obstacle map test" << it->second.m_inArcList.size() << std::endl;

		}

		else if (searchType == "AD*")
		{
			// !!!!
			// iterate through obstacle map
			// should only have one item in it at a time
			std::map<int, GraphNode>::iterator it = obstacleMap.begin();
			std::map<int, GraphNode>::iterator end = obstacleMap.end();

			// !!!
			// Assuming done one at a time
			/*for (; it != end; it++)

			{*/
			//GraphNode nodeObject = *(&(it->second));


			for (; it != end; it++)
			{
				if (stoi(it->second.data().first) == node)
				{
					std::cout << it->second.getWaypoint().x << " " << it->second.getWaypoint().y << std::endl;
					std::cout << stoi(it->second.data().first) << std::endl;

					// !!!!
					// add back in the node
					addNode(it->second.data(), it->second.rhsData(), it->second.marked(), node, it->second.getWaypoint(), it->second.getKey(), it->second.getHeuristic(), it->second.getGoal());
					//GraphNode nodeObject = *(m_pNodes[index]);


					//GraphNode nodeObject = *(&(it->second));
					//addNode(&nodeObject, stoi(it->second.data().first));
					std::cout << m_pNodes[node]->arcList2().size() << std::endl;
					std::list<std::pair<int, int>>::iterator start = it->second.m_inArcList.begin();
					std::list<std::pair<int, int>>::iterator theend = it->second.m_inArcList.end();

					// add back in the arcs
					for (; start != theend; start++)
					{
						std::cout << start->first << std::endl;
						addArc(start->first, node, start->second);
						// added because I couldn't fully copy node 
						//addArc(node, start->first, start->second);
					}


					std::list<std::pair<int, int>>::iterator start2 = it->second.m_outArcList.begin();
					std::list<std::pair<int, int>>::iterator theend2 = it->second.m_outArcList.end();

					// add back in the arcs
					for (; start2 != theend2; start2++)
					{
						std::cout << start2->first << std::endl;
						addArc(node, start2->first, start2->second);
						// added because I couldn't fully copy node 
						//addArc(node, start->first, start->second);
					}
					std::cout << "test" << std::endl;

				}
			}

			// !!!!
			// clear the obstacle map as we've gotten the necessary data now
			// ensures only one item is in it at any one time
			//obstacleMap.clear();

			std::map<int, GraphNode>::iterator removal;
			removal = obstacleMap.find(node);

			obstacleMap.erase(removal);


			std::cout << m_pNodes[node]->getWaypoint().x << " " << m_pNodes[node]->getWaypoint().y << std::endl;
			std::cout << m_pNodes[node]->arcList2().size() << std::endl;

			// *********************************************************
			std::cout << "Outgoing arcs: " << m_pNodes[node]->arcList2().size() << std::endl;
			list<GraphArc>::iterator iter5 = m_pNodes[node]->arcList2().begin();
			list<GraphArc>::iterator endIter5 = m_pNodes[node]->arcList2().end();

			for (; iter5 != endIter5; iter5++)
			{
				std::cout << (iter5->node()->data().first) << std::endl;
				ADStarUpdateState((*iter5).node(), m_pNodes[start]);
				//UpdateVertex(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
			}

			std::cout << "Outgoing arcs: " << m_pNodes[node]->arcList2().size() << std::endl;

			//std::cout << "obstacle map test" << it->second.m_inArcList.size() << std::endl;
		}
	}

	//flag = false;
}



float Graph::CalculateHeuristic(GraphNode * node, bool obstacle)
{
	float dx, dy = 0;
	/*if (obstacle == false)
	{*/
		dx = abs(node->getWaypoint().x - node->getGoal().x);
		dy = abs(node->getWaypoint().y - node->getGoal().y);

		std::cout << "DX: " << dx << " DY: " << dy << std::endl;
	//}
	//else
	//{
	//	//dx = abs(node->getWaypoint().x + std::numeric_limits<int>::max() - 50000);
	//	//dy = abs(node->getWaypoint().y + std::numeric_limits<int>::max() - 50000);
	//	
	//	//dx = abs(std::numeric_limits<int>::max() - 50000);
	//	//dy = abs(std::numeric_limits<int>::max() - 50000);
	//}


	//dx = abs(node->getWaypoint().x - node->getGoal().x);
	//dy = abs(node->getWaypoint().y - node->getGoal().y);

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
		//std::cout << gs << " " << rhs << " " << std::endl;
		//std::cout << "Heuristic at time of key calculation " << node->getHeuristic() <<  std::endl;
		k1 = std::min(gs, rhs) + node->getHeuristic();
		k2 = std::min(gs, rhs);
		//std::cout << k1 << " " << k2 << " " << std::endl;
		key = sf::Vector2f{ k1, k2 };
	}

	if (searchType == "AD*")
	{
		if (gs > rhs)
		{
			//std::cout << "gs: " << gs << std::endl;
			//std::cout << "rhs: " << rhs << std::endl;
			//std::cout << "heur: " << node->getHeuristic() << std::endl;
			//std::cout << "inflation: " << getInflation() << std::endl;
			k1 = rhs + getInflation() * node->getHeuristic();
			k2 = rhs;
		}
		else
		{
			k1 = gs + node->getHeuristic();
			k2 = gs;
		}

	/*	k1 = std::min(gs, rhs) + getInflation() * node->getHeuristic();
		k2 = std::min(gs, rhs);*/

		key = sf::Vector2f{ k1, k2 };

		//std::cout << "KEY CALCULATION" << std::endl;
		std::cout << k1 << std::endl;
		std::cout << k2 << std::endl;
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

	//std::cout << "**************************************************Node being compared: " << n1->data().first << std::endl;
	//std::cout << "Goal Key X: " << k1 << std::endl;
	//std::cout << "Goal Key Y: " << k2 << std::endl;
	//std::cout << "Key At Front X : " << p1 << std::endl;
	//std::cout << "Key at Front Y: " << p2 << std::endl;
	//std::cout << "Key G-Value: " << n1->data().second << std::endl;
	//std::cout << "Key RHS-Value: " << n1->rhsData().second << std::endl;
	//std::cout << "Key Heurtistic: " << n1->getHeuristic() << std::endl;
	//std::cout << "NodeQueue Size: " << nodeQueue.size() << std::endl;

	std::vector<float> v1{ k1, k2 };
	std::vector<float> v2{ p1, p2 };

	bool check = false;
	/*if (k1 == p1)
	{
		return k2 < p2;
	}
	else
	{
		return (k1 < p1);
	}*/

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

	
	if (check == false)
	{
		std::cout << "This condition is no longer true" << std::endl;
	}

	return check;

	///*bool check = std::lexicographical_compare(v1.begin(), v1.end(),
	//	v2.begin(), v2.end());*/
	//std::cout << "check" << check << std::endl;

	//return check;
}



void Graph::ADStarInitialize(GraphNode * pStart, GraphNode * pDest, std::vector<GraphNode*>& path, float inflation)
{
	searchType = "AD*";
	setInflation(inflation);
	/*if (pStart != 0)
	{*/
		///// g(sstart) = rhs(sstart) = ∞; g(sgoal) = ∞;
		// setting the initial values of all of the nodes
		for (int i = 0; i < m_maxNodes; i++)
		{
			//std::cout << m_pNodes[i]->getWaypoint().x << std::endl;
			m_pNodes[i]->setGoal(pStart->getWaypoint());

			float heuristic = CalculateHeuristic(m_pNodes[i], false);
			m_pNodes[i]->setHeuristic(heuristic);

			std::cout << "HEURISTIC: " << heuristic << std::endl;

			/*		if (m_pNodes[i]->getObstacle() == true)
			{
			continue;
			}
			else
			{*/
			auto data = m_pNodes[i]->data();
			auto rhsData = m_pNodes[i]->rhsData();
			// set the weight to an infinite value to start off with
			data.second = std::numeric_limits<int>::max() - 100000;
			// set the rhs values of all nodes to an infinite value
			rhsData.second = std::numeric_limits<int>::max() - 100000;


			m_pNodes[i]->setMarked(false);

			m_pNodes[i]->setData(data);
			m_pNodes[i]->setRhsData(rhsData);
			//}

		}
		std::cout << "clearing queues" << std::endl;

		openQueue.clear();
		closedQueue.clear();
		inconsQueue.clear();

		std::cout << "queues cleared" << std::endl;

		pDest->setRhsData(pair<string, int>(pDest->rhsData().first, 0));
		sf::Vector2f initialKey;
	/*	initialKey.x = CalculateHeuristic(pDest, false);
		initialKey.y = 0;*/
		initialKey = CalculateKey(pDest, "AD*");
		pDest->setKey(initialKey);
		//set as being marked/visited
		pDest->setMarked(true);
		// insert starting node into the queue
		//openQueue.insert(openQueue.begin(), pDest);
		openQueue.push_back(pDest);
	/*}*/
	std::cout << "Anytime Dynamic A* initialized" << std::endl;

	// will call ComputeOrImprovePath here
	//ComputeOrImprovePath(pStart, pDest);
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
		//std::cout << "starting node" << pStart->data().first << std::endl;
		list<GraphArc>::const_iterator iter = node->arcList().begin();
		list<GraphArc>::const_iterator endIter = node->arcList().end();


		//int distance;
		// for each iteration though the nodes
		for (; iter != endIter; iter++)
		{

			/*if ((*iter).node()->getObstacle() == true)
			{
			continue;
			}
			else
			{*/
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
			//}
		}

		std::cout << "rhs value: " << min << std::endl;

		node->setRhsData(pair<string, int>(node->rhsData().first, min));




		if (std::find(nodesToExamine.begin(), nodesToExamine.end(), stoi(node->data().first)) != nodesToExamine.end())
		{

		}
		else
		{
			nodesToExamine.push_back(stoi(node->data().first));

		}

		//node->setKey(CalculateKey(node));
	}

	// 08. if (s ∈ OPEN) remove s from OPEN;
	// Remove 'node' from the priority queue only if it is present.
	if (openQueue.size() > 0)
	openQueue.erase(std::remove_if(openQueue.begin(), openQueue.end(), [node](auto nodeInVector) { return node == nodeInVector;  }), openQueue.end());


	// 09. if (g(s) 6= rhs(s))

	//if (node->getObstacle() == false)
	//{
	if (node->data().second != node->rhsData().second)
	{
		//10. if s6∈ CLOSED

		//std::vector<Node*>::iterator it;

		//it = std::find(closedQueue.begin(), closedQueue.end(), node);
		//closedQueue.push_back(node);

	
	//	closedQueue.push_back(node);

		if (std::find(closedQueue.begin(), closedQueue.end(), node) != closedQueue.end())
		{
			// 13. insert s into INCONS;
			std::cout << node->data().first << "node pushed into incons queue" << std::endl;
			//node->setMarked(false);
			inconsQueue.push_back(node);

			//std::sort(inconsQueue.begin(), inconsQueue.end(), pairCompare);
		}
		else //12. else
		{
			/*if (closedQueue.size() > 0)
			{
				std::cout << "closed" << closedQueue.front()->data().first << std::endl;
			}
			if (openQueue.size() > 0)
			{
				std::cout << "open" << openQueue.front()->data().first << std::endl;
			}*/
			//11. insert s into OPEN with key(s);
			std::cout << node->data().first << std::endl;
			node->setKey(CalculateKey(node, "AD*"));
			node->setMarked(true);
			openQueue.push_back(node);
			std::sort(openQueue.begin(), openQueue.end(), pairCompare);
			//std::cout << "node pushed into closed queue: " << node->data().first << std::endl;
		}
	}
	//std::cout << node->data().first << std::endl;
	//node->setKey(CalculateKey(node));
	//node->setMarked(true);
	//openQueue.push_back(node);
	//std::sort(openQueue.begin(), openQueue.end(), pairCompare<NodeType, ArcType>);

	//std::cout << "node pushed " << std::endl;
	//}
	//}
}



int Graph::ComputeOrImprovePath(GraphNode * pStart, GraphNode * pDest)
{
	std::cout << "INFLATION: " << ADStarInflation << std::endl;
	//setInflation(inflation);
	if (openQueue.size() > 0)
	{
		std::sort(openQueue.begin(), openQueue.end(), pairCompare);
		//flag == false
		//pStart->setKey(CalculateKey(pStart, "AD*"));
		/*	while (keyComparer(nodeQueue.front(), pStart) == true*/
			//while (keyComparer(nodeQueue.front(), pDest) == true
		while (keyComparer(openQueue.front(), pStart) == true || pStart->rhsData().second != pStart->data().second)
		{
			/*if (openQueue.front() == pStart)
			{
				flag = true;
			}*/

			// 15. remove state s with the minimum key from OPEN;
			GraphNode * node = openQueue.front();
			openQueue.erase(std::remove(openQueue.begin(), openQueue.end(), openQueue.front()), openQueue.end());

			//std::cout << openQueue.size() << std::endl;
			std::cout << "node in open queue: " << node->data().first << std::endl;
			std::cout << "node popped" << std::endl;

			// 16. if (g(s) > rhs(s))

			if (node->data().second > node->rhsData().second)
			{

				auto data = node->data();
				data.second = node->rhsData().second;
				// 17. g(s) = rhs(s);
				node->setData(data);

				//node->setMarked(false);

				//Node node = *nodeQueue->top();


				/*node->setKey(CalculateKey(node, "AD*"));
				node->setMarked(true);*/
				// 18. CLOSED = CLOSED ∪ {s};
				closedQueue.push_back(node);

				// 19. for all s' ∈ Pred(s) UpdateState(s');
				list<GraphArc>::const_iterator iter = node->arcList().begin();
				list<GraphArc>::const_iterator endIter = node->arcList().end();

				// for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					//if ((*iter).node()->getObstacle() == false)
					//{
					ADStarUpdateState((*iter).node(), pDest);
					//UpdateVertex((*iter).node(), pStart);
					//}
				}
			}
			else //20. else
			{
				//21. g(s) = ∞;
				auto data = node->data();
				data.second = std::numeric_limits<int>::max() - 100000;
				node->setData(data);

				// 22. for all s' ∈ Pred(s) ∪ { s } UpdateState(s');

				list<GraphArc>::const_iterator iter = node->arcList().begin();
				list<GraphArc>::const_iterator endIter = node->arcList().end();



				ADStarUpdateState(node, pDest);
				//for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					ADStarUpdateState((*iter).node(), pDest);
				}
			}
			//pStart->setKey(CalculateKey(pStart, "AD*"));
		}


		for (int i = 0; i < m_maxNodes; i++)
		{
			if (m_pNodes[i] == nullptr)
			{
				std::cout << "Node " << i << " returns nullptr" << std::endl;
				std::cout << endl;
			}
			if (m_pNodes[i] != nullptr)
			{
				std::cout << i << ": G-Value " << m_pNodes[i]->data().second << std::endl;
				std::cout << i << ": RHS-Value " << m_pNodes[i]->rhsData().second << std::endl;
				std::cout << i << ": Marked " << m_pNodes[i]->marked() << std::endl;
				std::cout << i << ": key " << m_pNodes[i]->getKey().x << " " << m_pNodes[i]->getKey().y << std::endl;
				std::cout << i << ": heur " << m_pNodes[i]->getHeuristic() << std::endl;
				std::cout << endl;
			}
		}
	}

		std::cout << std::endl;
		std::cout << "ADSTAR COST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		std::cout << "Path Cost: " << pStart->data().second << std::endl;
		std::cout << "INCONS QUEUE SIZE: " << inconsQueue.size() << std::endl;
		std::cout << "CLOSED QUEUE SIZE: " << closedQueue.size() << std::endl;
		std::cout << "OPEN QUEUE SIZE: " << openQueue.size() << std::endl;
		std::cout << "INFLATION AT END: " << getInflation() << std::endl;


		for (int i = 0; i < nodesToExamine.size(); i++)
		{
			if (m_pNodes[nodesToExamine.at(i)] == nullptr)
			{

				nodesToExamine.erase(nodesToExamine.begin() + i);
				//obstacleMap.erase(removal);
			}
		}

		std::cout << "nodes to examine: " << nodesToExamine.size() << std::endl;


		GraphNode* current = pStart;
		int max = pStart->data().second / 100;
		for (int y = 0; y < max; y++)
		{
			for (int i = 0; i < nodesToExamine.size(); i++)
			{



				/*if (m_pNodes[nodesToExamine.at(i)] != pDest)
				{*/
				/*if (current == pStart)
				{
				break;
				}
				*/
				list<GraphArc>::iterator iter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().begin();
				list<GraphArc>::iterator endIter2 = m_pNodes[nodesToExamine.at(i)]->arcList2().end();


				for (; iter2 != endIter2; iter2++)
				{
					if (m_pNodes[nodesToExamine.at(i)]->data().second == (current->data().second - 100) && iter2->node() == current)
					{
						std::cout << "Pushed: " << nodesToExamine.at(i) << std::endl;
						path.push_back(m_pNodes[nodesToExamine.at(i)]);
						std::cout << m_pNodes[nodesToExamine.at(i)]->data().first << std::endl;
						current = m_pNodes[nodesToExamine.at(i)];
					}
				}
				/*}*/
			}
		}


		std::cout << path.size() << std::endl;


		//**********************
		//nodeQueue.clear();
		//nodeQueue.push_back(pStart);
		std::cout << std::endl;
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
	std::cout << "SIZE START OF LOOP: " << size << std::endl;
	
	/*for (int i = 0; i < inconsQueue.size(); i++)
	{
		if (inconsQueue.at(i) == nullptr)
		{
			inconsQueue.erase(inconsQueue.begin() + i);
		}
	}*/
	
	while (inconsQueue.size() > 0)
	{
		/*for (int i = 0; i < size -1; i++)
		{*/
			std::cout << "SIZE: " << inconsQueue.size() << std::endl;
			GraphNode * node = inconsQueue.front();
			openQueue.push_back(node);
			inconsQueue.erase(std::remove(inconsQueue.begin(), inconsQueue.end(), inconsQueue.front()), inconsQueue.end());
		/*}*/
	}
	//std::sort(openQueue.begin(), openQueue.end(), pairCompare);
	
	/*std::map<int, GraphNode>::iterator removal;
	removal = obstacleMap.find(node);

	obstacleMap.erase(removal);*/
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








