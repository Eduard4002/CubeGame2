#pragma once
#include "Entity.h"
class Rectangle : public Entity
{
private:
	sf::RectangleShape shape;
public:
	Rectangle(sf::Vector2f size, sf::Vector2f position,sf::RenderWindow& window, unsigned int index, sf::Color fillColor = sf::Color::White);

	void Render();

	sf::FloatRect getGlobalBounds();
};

