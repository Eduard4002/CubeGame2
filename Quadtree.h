#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
class Point {
public:
	Point(std::string tag,sf::FloatRect boundary, int index) {
		this->tag = tag;
		this->boundary = boundary;
		this->index = index;
	}
	//Is it a bullet? Enemy?
	std::string tag;
	//Boundary
	sf::FloatRect boundary;
	//What is the std::vector index corresponding to this enemy?
	int index;
};
class Quadtree
{
private:
	sf::FloatRect boundary;
	int capacity;

	std::vector<Point*> points;

	// Children
	Quadtree* northWest;
	Quadtree* northEast;
	Quadtree* southWest;
	Quadtree* southEast;
private:
	void subdivide();
	
public:
	Quadtree(sf::FloatRect boundary);

	bool insert(Point* point);
	std::vector<Point*> queryRange(sf::FloatRect boundary);

	void show(sf::RenderWindow& window);

	void clear();
};

