#include "Unit.h"

Unit::Unit(int x, int y, int radius, sf::Color col)
{
	m_xPos = x;
	m_yPos = y;
	m_radius = radius;
	colour = col;

	/*shape.setPosition(x, y);
	shape.setRadius(55);
	s*/
	shape.setRadius(20);
	shape.setFillColor(sf::Color::Red);
	//shape.setOutlineColor(sf::Color::Blue);
	//shape.setOutlineThickness(4);
	shape.setPosition(700, 232);
}

Unit::~Unit()
{
}

void Unit::Move()
{
}

void Unit::SetTarget(sf::Vector2f target)
{
}

void Unit::SetPath(std::vector<GraphNode*>& path)
{
}

void Unit::Draw(sf::RenderWindow * window)
{
	window->draw(shape);
}
