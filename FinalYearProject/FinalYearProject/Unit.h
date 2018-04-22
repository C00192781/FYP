#pragma once

#include "GraphNode.h"
#include "SFML\Graphics.hpp"
#include <iostream>

using namespace std;

class Unit
{

public:
	Unit(float x, float y, int radius, sf::Color col);
	~Unit();

	void Move();
	void SetTarget(float tarX, float tarY);
	void SetPath(std::vector<GraphNode*> &path, int startX, int startY);
	void Draw(sf::RenderWindow * window);
	int searchNearestWaypoint(float xPos, float yPos);

	void Reset();

private:

	sf::CircleShape shape;
	sf::Color colour;

	sf::Vector2f m_position;

	float m_xPos;
	float m_yPos;
	float m_startX;
	float m_startY;
	int m_radius;

	int m_xSpeed;
	int m_ySpeed;

	bool move;
	sf::Vector2f m_target;

	int m_targetX;
	int m_targetY; 

	std::vector<GraphNode*> m_path;

	bool moving;
	sf::Clock clock;
};