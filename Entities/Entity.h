#pragma once
#include "SFML/Graphics.hpp"
#include "../Physics.h"
#include <iostream>
class Item;
class Entity
{
private:
	
	sf::View windowView;

protected:
	sf::RenderWindow* window = nullptr;
	sf::Vector2u windowSize;

	bool isCollidable = false;

public:
	std::string name;
	std::string tag;
	unsigned int index;
public:
	//Function for checking if a entity is outside of window
	bool isOutsideOfWindow(sf::FloatRect shape);
public:
	Entity(sf::RenderWindow& window, unsigned int index);
	Entity();
	virtual void Update(float deltaTime);
	virtual void FixedUpdate();
	virtual void Render();

	//What should the entity do IF outside of window
	virtual void outsideOfWindow();

	//Get global bounds
	virtual sf::FloatRect getGlobalBounds();

	virtual void TakeDamage(int amount);

};

