#pragma once

#include "GraphNode.h"
#include "SFML\Graphics.hpp"
#include <iostream>

using namespace std;

class Unit
{

public:
	Unit(int x, int y, int radius, sf::Color col);
	~Unit();

	void Move();
	void SetTarget(sf::Vector2f target);
	void SetPath(std::vector<GraphNode*> &path);
	void Draw(sf::RenderWindow * window);

private:

	sf::CircleShape shape;
	sf::Color colour;

	sf::Vector2f m_position;

	int m_xPos;
	int m_yPos;
	int m_radius;

	int m_xSpeed;
	int m_ySpeed;

	std::vector<GraphNode*> path;
};