#pragma once
#include "../Entities/Entity.h"
class Item : public Entity
{
protected:
	sf::Sprite sprite;
	sf::RectangleShape shape;
	sf::RectangleShape pickableZone;
public:
	bool isUsing = false;
	bool isInInventory = false;
	bool isDropped;
public:

	Item(std::string name, sf::Vector2f position, std::string textureFileName, sf::RenderWindow& window, unsigned int index);
	Item(std::string name, sf::Vector2f position, std::string textureFileName );
	Item(sf::Vector2f position, sf::RenderWindow& window, unsigned int index);
	Item();
	void Render();
	virtual void FixedUpdate();
	virtual void Update(float dt);

	virtual void Use();
	sf::FloatRect getGlobalBounds();
	sf::FloatRect getPickableZoneBounds();

	virtual void FollowPlayer(sf::Vector2f playerPos);
};

