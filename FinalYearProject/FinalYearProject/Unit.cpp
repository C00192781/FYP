#include "Unit.h"

Unit::Unit(float x, float y, int radius, sf::Color col)
{
	m_xPos = -100;
	m_yPos = -100;
	m_radius = radius;
	colour = col;
	move = false;

	moving = false;

	/*shape.setPosition(x, y);
	shape.setRadius(55);
	s*/
	//m_position = sf::Vector2f{ m_xPos, m_yPos };
	shape.setRadius(20);
	shape.setFillColor(sf::Color::Red);
	//shape.setOutlineColor(sf::Color::Blue);
	//shape.setOutlineThickness(4);
	shape.setPosition(m_xPos, m_yPos);
}

Unit::~Unit()
{
}

void Unit::Move()
{
	//std::cout << shape.getGlobalBounds().left << " " << shape.getGlobalBounds().top << std::endl
	if (move == true)
	{
		if (m_xPos < m_targetX)
		{
			m_xPos += 0.5;
		}
		if (m_xPos > m_targetX)
		{
			m_xPos -= 0.5;
		}
		if (m_yPos < m_targetY)
		{
			m_yPos += 0.5;
		}
		if (m_yPos > m_targetY)
		{
			m_yPos -= 0.5;
		}
		shape.setPosition(m_xPos, m_yPos);

		if (m_xPos == m_targetX && m_yPos == m_targetY)
		{

			if (m_path.size() > 0)
			{
				std::cout << m_path.size() << std::endl;
				SetTarget(m_path.front()->getWaypoint().x, m_path.front()->getWaypoint().y);
				std::cout << m_path.front()->getWaypoint().x << " " << m_path.front()->getWaypoint().y;
				m_path.erase(m_path.begin());
				//std::cout << m_path.front()->data().first << " " << m_path.size() << std::endl;
				clock.restart();

				//std::cout << "Path Size: " << m_path.size() << std::endl;
			}
			
			if (m_path.size() <= 0)
			{
				if (clock.getElapsedTime().asSeconds() >= 1)
				{
					m_xPos = m_startX;
					m_yPos = m_startY;
					SetTarget(m_startX, m_startY);
				}
			}
			
			//std::cout << "test" << std::endl;
			/*if (pathWaypoints.size() > 0)
			{
				std::reverse(pathWaypoints.begin(), pathWaypoints.end());
				pathWaypoints.pop_back();
				if (pathWaypoints.size() > 0)
				{
					std::reverse(pathWaypoints.begin(), pathWaypoints.end());
					SetTarget(pathWaypoints.front());
				}
			}*/
		}

	}
}

void Unit::SetTarget(float tarX, float tarY)
{
	m_targetX = tarX;
	m_targetY = tarY;
}

void Unit::SetPath(std::vector<GraphNode*>& path, int startX, int startY)
{
	//std::cout << "path " << path.size() << std::endl;
	m_startX = startX;
	m_startY = startY;
	
	// NOTE: MAY REMOVE THESE 2 LINES
	m_xPos = startX;
	m_yPos = startY;
	
	bool extraNode = false;
	int index = 0;
	GraphNode *node;
	//if (m_path.size() > 0)
	//{
	//	index = searchNearestWaypoint(m_xPos, m_yPos);
	//	//std::cout << "dfsffdfggfgfgfgffgfgffdff" << m_path.at(index)->data().first << std::endl;
	//	std::cout << std::endl;
	//	extraNode = true;
	//	node = m_path.front();
	//}

	if (path.size() > 0)
	{
		move = true;
		m_path = path;
		std::cout << path.size() << std::endl;
		/*if (extraNode == true)
		{
			m_path.insert(m_path.begin(), node);
		}*/
		index = searchNearestWaypoint(m_xPos, m_yPos);
		std::cout << "dfsffdfggfgfgfgffgfgffdff" << index << std::endl;
		//std::cout << path.front()->getWaypoint().x << " " << path.front()->getWaypoint().y;
		
		
		
		//***********************************IMPORTANT LINE COMMENTED OUT
		//m_path.erase(m_path.begin(), m_path.begin() + index);

		std::cout << m_path.size() << std::endl;
		SetTarget(m_path.front()->getWaypoint().x, m_path.front()->getWaypoint().y);
	}
}

void Unit::Draw(sf::RenderWindow * window)
{
	window->draw(shape);
}


int Unit::searchNearestWaypoint(float xPos, float yPos)
{
	float differenceValue = std::numeric_limits<int>::max() - 10000;
	int differenceIndex;

	for (int i = 0; i < m_path.size(); i++)
	{
		sf::Vector2f difference;
		difference.x = xPos - m_path.at(i)->getWaypoint().x;
		difference.y = yPos - m_path.at(i)->getWaypoint().y;

		float diff = sqrtf((difference.x*difference.x) + (difference.y*difference.y));
		if (diff < differenceValue)
		{
			differenceValue = diff;
			differenceIndex = i;
		}
	}

	//return everyWaypoint.at(differenceIndex);
	return differenceIndex;
}

void Unit::Reset()
{
	m_xPos = m_startX;
	m_yPos = m_startY;
}
