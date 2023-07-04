#pragma once
#include "Entity.h"
class Platform : public Entity
{
private:
	//sf::RectangleShape shape;
	sf::Sprite sprite;
	sf::Texture texture;
public:
	Platform(sf::Vector2f position,sf::RenderWindow& window, unsigned int index);

	void Render();
	void addTexture(std::string fileName);

	sf::FloatRect getGlobalBounds();
};

