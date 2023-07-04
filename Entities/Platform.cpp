#include "Platform.h"

Platform::Platform(sf::Vector2f position, sf::RenderWindow& window, unsigned int index) : Entity(window, index) {
	sprite.setPosition(position);
}

void Platform::Render() {
	window->draw(sprite);
}

void Platform::addTexture(std::string fileName)
{
	if (!texture.loadFromFile(fileName))
	{
		std::cout << "Not able to load from: " << fileName << std::endl;
	}
	sprite.setTexture(texture);
}

sf::FloatRect Platform::getGlobalBounds() {
	return sprite.getGlobalBounds();
}