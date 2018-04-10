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
bool Graph::addNode(std::pair<std::string, int> data, std::pair<std::string, int> rhsData, bool marked, int index, sf::Vector2f waypoint) {
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
		// create a new node, put the data in it, and unmark it.
		//m_pNodes[index] = &node;
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
				std::cout << "counter" << counter << std::endl;
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

		node->setRhsData(pair<string, int>(node->rhsData().first, min));
		//node->setKey(CalculateKey(node));
	}

	// Remove 'node' from the priority queue only if it is present.
	nodeQueue.erase(std::remove_if(nodeQueue.begin(), nodeQueue.end(), [node](auto nodeInVector) { return node == nodeInVector;  }), nodeQueue.end());

	//if (node->getObstacle() == false)
	//{
	if (node->data().second != node->rhsData().second)
	{
		std::cout << node->data().first << std::endl;
		node->setKey(CalculateKey(node, "LPA*"));
		node->setMarked(true);
		nodeQueue.push_back(node);
		std::sort(nodeQueue.begin(), nodeQueue.end(), pairCompare);

		std::cout << "node pushed " << std::endl;
	}
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

	if (nodeQueue.size() > 0)
	{
		std::sort(nodeQueue.begin(), nodeQueue.end(), pairCompare);

		while (flag == false || pDest->rhsData().second != pDest->data().second)
		{
			if (nodeQueue.front() == pDest)
			{
				flag = true;
			}

			/*if (keyComparer(nodeQueue.front(), pDest) == true)
			{
			std::cout << "compare " << std::endl;
			}*/

			GraphNode * node = nodeQueue.front();
			nodeQueue.erase(std::remove(nodeQueue.begin(), nodeQueue.end(), nodeQueue.front()), nodeQueue.end());

			std::cout << "node in priority queue: " << node->data().first << std::endl;
			std::cout << "node popped" << std::endl;
			if (node->data().second > node->rhsData().second)
			{

				auto data = node->data();
				data.second = node->rhsData().second;
				node->setData(data);
				node->setMarked(false);


				//Node node = *nodeQueue->top();

				std::list<GraphArc>::const_iterator iter = node->arcList().begin();
				std::list<GraphArc>::const_iterator endIter = node->arcList().end();

				// for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					//if ((*iter).node()->getObstacle() == false)
					//{
					UpdateVertex((*iter).node(), pStart);
					//}
				}
			}
			else //RP {14} else
			{
				// RP
				//{15} g(u) = ∞;
				auto data = node->data();
				data.second = std::numeric_limits<int>::max() - 100000;
				node->setData(data);

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
					UpdateVertex((*iter).node(), pStart);
					//}
				}
			}

		}

		std::cout << std::endl;
		std::cout << "Path Cost: " << pDest->data().second << std::endl;

		//**********************
		//nodeQueue.clear();
		//nodeQueue.push_back(pStart);
		std::cout << std::endl;
	}
}



void Graph::SetObstacle(int node, bool obstacle, int start)
{
	//if (obstacle == true)
	//{
	//	m_pNodes[node]->setObstacle(true);
	//}
	//else
	//{
	//	m_pNodes[node]->setObstacle(false);
	//}




	//list<Arc>::iterator iter = m_pNodes[node]->arcList2().begin();
	//list<Arc>::iterator endIter = m_pNodes[node]->arcList2().end();

	//int weight;

	//if (obstacle == true)
	//{
	//	weight = std::numeric_limits<int>::max() - 50000;
	//}
	//else
	//{
	//	weight = 100;
	//}

	//// for node being set as an obstacle
	//// setting weight of arcs protuding out to other nodes
	//for (; iter != endIter; iter++)
	//{
	//	(*iter).setWeight(weight);

	//	// iterate through arcs of the nodes connected to the obstacle node
	//	// we do this so we can set the weight of the node arcs going in to the obstacle node
	//	list<Arc>::iterator iter2 = (*iter).node()->arcList2().begin();
	//	list<Arc>::iterator endIter2 = (*iter).node()->arcList2().end();
	//	
	//	for (; iter2 != endIter2; iter2++)
	//	{
	//		// if the arc is attached to the obstacle node (could also point to other nodes),
	//		// set the weight
	//		if ((*iter2).node()->data().first == m_pNodes[node]->data().first)
	//		{
	//			(*iter2).setWeight(weight);
	//		}
	//	}
	//}

	// Now, we update the vertexes at the end of the arcs protruding from the obstacle node

	//list<GraphArc>::iterator iter3 = m_pNodes[node]->arcList2().begin();
	//list<GraphArc>::iterator endIter3 = m_pNodes[node]->arcList2().end();
	////UpdateVertex(m_pNodes[node], m_pNodes[start]);
	//for (; iter3 != endIter3; iter3++)
	//{
	//	std::cout << "Nodes at end of obstacle node's arc: " << (*iter3).node()->data().first << std::endl;
	//	UpdateVertex((*iter3).node(), m_pNodes[start]);
	//}

	//float heuristic = CalculateHeuristic(m_pNodes[node], true);
	//m_pNodes[node]->setHeuristic(heuristic);

	if (obstacle == true)
	{

		std::vector<int> nodesToUpdate;

		// iterate through arcs of the nodes connected to the obstacle node
		// we do this so we can set the weight of the node arcs going in to the obstacle node
		list<GraphArc>::iterator iter2 = m_pNodes[node]->arcList2().begin();
		list<GraphArc>::iterator endIter2 = m_pNodes[node]->arcList2().end();

		for (; iter2 != endIter2; iter2++)
		{
			// if the arc is attached to the obstacle node (could also point to other nodes),
			// set the weight
			//if ((*iter2).node()->data().first == m_pNodes[node]->data().first)
			//{
			////(*iter2).setWeight(weight);
			//	m_pNodes[node]->m_inArcList.push_back(std::make_pair(stoi((*iter2).node()->data().first), *(*iter2).node()));
			//}



			nodesToUpdate.push_back(stoi((*iter2).node()->data().first));
		}

		m_pNodes[node]->m_inArcList.clear();

		for (int i = 0; i < nodesToUpdate.size(); i++)
		{
			list<GraphArc>::iterator connectedIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().begin();
			list<GraphArc>::iterator connectedEndIter = m_pNodes[nodesToUpdate.at(i)]->arcList2().end();

			for (; connectedIter != connectedEndIter; connectedIter++)
			{
				if ((*connectedIter).node()->data().first == m_pNodes[node]->data().first)
				{
					//std::cout << (*connectedIter).node()->data().first << std::endl;
					//std::cout << "bogus dude!" << std::endl;
					m_pNodes[node]->m_inArcList.push_back(std::make_pair(stoi(m_pNodes[nodesToUpdate.at(i)]->data().first), (*connectedIter).weight()));

				}
			}
		}

		std::cout << "m_inArcList size : " << m_pNodes[node]->m_inArcList.size() << std::endl;

		removeNode(node);

		//std::map<int, GraphNode>::iterator it = obstacleMap.begin();
		////std::cout << "obstacle map test" << it->second.m_inArcList.size() << std::endl;

		for (int i = 0; i < nodesToUpdate.size(); i++)
		{
			UpdateVertex(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
			//UpdateVertex(node)
		}
	}
	else
	{

		std::map<int, GraphNode>::iterator it = obstacleMap.begin();
		std::map<int, GraphNode>::iterator end = obstacleMap.end();


		/*for (; it != end; it++)
		{*/
			if (stoi(it->second.data().first) == node)
			{
				addNode(it->second.data(), it->second.rhsData(), it->second.marked(), node, it->second.getWaypoint());
				//addNode(&(it->second), stoi(it->second.data().first));

				std::list<std::pair<int, int>>::iterator start = it->second.m_inArcList.begin();
				std::list<std::pair<int, int>>::iterator theend = it->second.m_inArcList.end();

				for (; start != theend; start++)
				{
					addArc(start->first, node, start->second);
					addArc(node, start->first, start->second);
				}
				std::cout << "test" << std::endl;

	/*			for (int i = 0; i < it->second.m_inArcList.size(); i++)
				{
					addArc(it->second.m_inArcList.back().second.data, node, weight);
				}
				addArc(4, 5, 100);
				addArc(6, 5, 100);*/

			}
		/*}
*/
		list<GraphArc>::iterator iter5 = m_pNodes[node]->arcList2().begin();
		list<GraphArc>::iterator endIter5 = m_pNodes[node]->arcList2().end();

		std::cout << "Outgoing arcs: " << m_pNodes[node]->arcList2().size() << std::endl;

		for (; iter5 != endIter5; iter5++)
		{
			std::cout << (iter5->node()->data().first) << std::endl;
			UpdateVertex((*iter5).node(), m_pNodes[start]);
			//UpdateVertex(m_pNodes[nodesToUpdate.at(i)], m_pNodes[start]);
		}



		//std::cout << "obstacle map test" << it->second.m_inArcList.size() << std::endl;
		
	}


	/*list<GraphArc>::iterator iter4 = m_pNodes[6]->arcList2().begin();
	list<GraphArc>::iterator endIter4 = m_pNodes[6]->arcList2().end();

	int check = 0;
	for (; iter4 != endIter4; iter4++)
	{
		check++;
		std::cout << "check: " <<check << std::endl;
	}*/

	flag = false;
}



float Graph::CalculateHeuristic(GraphNode * node, bool obstacle)
{
	float dx, dy = 0;
	if (obstacle == false)
	{
		dx = abs(node->getWaypoint().x - node->getGoal().x);
		dy = abs(node->getWaypoint().y - node->getGoal().y);
	}
	else
	{
		//dx = abs(node->getWaypoint().x + std::numeric_limits<int>::max() - 50000);
		//dy = abs(node->getWaypoint().y + std::numeric_limits<int>::max() - 50000);
		dx = abs(std::numeric_limits<int>::max() - 50000);
		dy = abs(std::numeric_limits<int>::max() - 50000);
	}


	//dx = abs(node->getWaypoint().x - node->getGoal().x);
	//dy = abs(node->getWaypoint().y - node->getGoal().y);

	float result = dx + dy;
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
	float k1 = n1->getKey().x;
	float k2 = n1->getKey().y;

	float p1 = n2->getKey().x;
	float p2 = n2->getKey().y;

	std::cout << "k1" << k1 << std::endl;
	std::cout << "k2" << k2 << std::endl;
	std::cout << "p1" << p1 << std::endl;
	std::cout << "p2" << p2 << std::endl;

	std::vector<float> v1{ k1, k2 };
	std::vector<float> v2{ p1, p2 };

	bool check;
	if (k1 == p1)
	{
		return k2 < p2;
	}
	else
	{
		return (k1 < p1);
	}

	///*bool check = std::lexicographical_compare(v1.begin(), v1.end(),
	//	v2.begin(), v2.end());*/
	//std::cout << "check" << check << std::endl;

	//return check;
}



void Graph::ADStarInitialize(GraphNode * pStart, GraphNode * pDest, std::vector<GraphNode*>& path, float inflation)
{
	setInflation(inflation);
	if (pStart != 0)
	{
		///// g(sstart) = rhs(sstart) = ∞; g(sgoal) = ∞;
		// setting the initial values of all of the nodes
		for (int i = 0; i < m_maxNodes; i++)
		{
			//std::cout << m_pNodes[i]->getWaypoint().x << std::endl;
			m_pNodes[i]->setGoal(pStart->getWaypoint());

			float heuristic = CalculateHeuristic(m_pNodes[i], false);
			m_pNodes[i]->setHeuristic(heuristic);

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
		initialKey.x = CalculateHeuristic(pDest, false);
		initialKey.y = 0;
		pDest->setKey(initialKey);
		// insert starting node into the queue
		openQueue.insert(openQueue.begin(), pDest);

		//set as being marked/visited
		pDest->setMarked(true);
	}
	std::cout << "Anytime Dynamic A* initialized" << std::endl;

	// will call ComputeOrImprovePath here
	ComputeOrImprovePath(pStart, pDest);
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
		//node->setKey(CalculateKey(node));
	}

	// 08. if (s ∈ OPEN) remove s from OPEN;
	// Remove 'node' from the priority queue only if it is present.
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
		if (std::find(closedQueue.begin(), closedQueue.end(), node) != closedQueue.end())
		{
			// 13. insert s into INCONS;
			std::cout << node->data().first << "node pushed into incons queue" << std::endl;
			inconsQueue.push_back(node);
		}
		else //12. else
		{
			if (closedQueue.size() > 0)
			{
				std::cout << "closed" << closedQueue.front()->data().first << std::endl;
			}
			if (openQueue.size() > 0)
			{
				std::cout << "open" << openQueue.front()->data().first << std::endl;
			}
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



void Graph::ComputeOrImprovePath(GraphNode * pStart, GraphNode * pDest)
{
	//setInflation(inflation);
	if (openQueue.size() > 0)
	{
		std::sort(openQueue.begin(), openQueue.end(), pairCompare);

		while (flag == false || pStart->rhsData().second != pStart->data().second)
		{
			if (openQueue.front() == pStart)
			{
				flag = true;
			}

			// 15. remove state s with the minimum key from OPEN;
			GraphNode * node = openQueue.front();
			openQueue.erase(std::remove(openQueue.begin(), openQueue.end(), openQueue.front()), openQueue.end());

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

				//for each iteration though the nodes
				for (; iter != endIter; iter++)
				{
					ADStarUpdateState((*iter).node(), pDest);
				}
				ADStarUpdateState(node, pDest);
			}

		}

		std::cout << std::endl;
		std::cout << "Path Cost: " << pStart->data().second << std::endl;

		//**********************
		//nodeQueue.clear();
		//nodeQueue.push_back(pStart);
		std::cout << std::endl;
	}
}



void Graph::setInflation(float inflation)
{
	ADStarInflation = inflation;
}



float Graph::getInflation()
{
	return ADStarInflation;
}








