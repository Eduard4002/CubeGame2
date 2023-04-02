#include "Rectangle.h"

Rectangle::Rectangle(sf::Vector2f size, sf::Vector2f position, sf::RenderWindow& window, unsigned int index, sf::Color fillColor) : Entity(window, index) {
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(fillColor);
}

void Rectangle::Render() {
	window->draw(shape);
}

sf::FloatRect Rectangle::getGlobalBounds() {
	return shape.getGlobalBounds();
}