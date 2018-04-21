#include "Unit.h"

Unit::Unit(int x, int y, int radius, sf::Color col)
{
	m_xPos = x;
	m_yPos = y;
	m_radius = radius;
	colour = col;
	move = false;

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
			m_xPos += 1;
		}
		if (m_xPos > m_targetX)
		{
			m_xPos -= 1;
		}
		if (m_yPos < m_targetY)
		{
			m_yPos += 1;
		}
		if (m_yPos > m_targetY)
		{
			m_yPos -= 1;
		}
		shape.setPosition(m_xPos, m_yPos);




		if (m_xPos == m_targetX && m_yPos == m_targetY)
		{

			if (m_path.size() > 0)
			{
				m_path.erase(m_path.begin());
				//std::cout << m_path.front()->data().first << " " << m_path.size() << std::endl;

				std::cout << "Path Size: " << m_path.size() << std::endl;
				SetTarget(m_path.front()->getWaypoint().x, m_path.front()->getWaypoint().y);
			}
			
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

void Unit::SetTarget(int tarX, int tarY)
{
	m_targetX = tarX;
	m_targetY = tarY;
}

void Unit::SetPath(std::vector<GraphNode*>& path)
{
	std::cout << "path " << path.size() << std::endl;
	
	if (path.size() > 0)
	{
		move = true;
		m_path = path;
		std::cout << path.front()->getWaypoint().x << " " << path.front()->getWaypoint().y;
		SetTarget(path.front()->getWaypoint().x, path.front()->getWaypoint().y);
	}
}

void Unit::Draw(sf::RenderWindow * window)
{
	window->draw(shape);
}


int Unit::searchNearestWaypoint(sf::Vector2f position)
{
	float differenceValue = std::numeric_limits<int>::max() - 10000;
	int differenceIndex;

	for (int i = 0; i < m_path.size(); i++)
	{
		sf::Vector2f difference;
		difference.x = position.x - m_path.at(i)->getWaypoint().x;
		difference.y = position.y - m_path.at(i)->getWaypoint().y;

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
