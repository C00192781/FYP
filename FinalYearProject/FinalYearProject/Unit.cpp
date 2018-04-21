#include "Unit.h"

Unit::Unit(int x, int y, int radius, sf::Color col)
{
	m_xPos = x;
	m_yPos = y;
	m_radius = radius;
	colour = col;
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
