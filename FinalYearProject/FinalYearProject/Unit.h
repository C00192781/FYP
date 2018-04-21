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
	void SetTarget(int tarX, int tarY);
	void SetPath(std::vector<GraphNode*> &path);
	void Draw(sf::RenderWindow * window);
	int searchNearestWaypoint(sf::Vector2f position);

private:

	sf::CircleShape shape;
	sf::Color colour;

	sf::Vector2f m_position;

	int m_xPos;
	int m_yPos;
	int m_radius;

	int m_xSpeed;
	int m_ySpeed;

	bool move;
	sf::Vector2f m_target;

	int m_targetX;
	int m_targetY; 

	std::vector<GraphNode*> m_path;
};