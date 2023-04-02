#pragma once
#include "SFML/Graphics.hpp"
#include "../Entities/Entity.h"
class Bullet : public Entity
{
private:
	sf::Texture texture;
	sf::RectangleShape shape;
	sf::Vector2f velocity;
	float speed = 10;
public:
	int damageAmount;
	bool playerShoot;
	bool toRemove;
public:
	Bullet(int damageAmount,sf::Vector2f size,sf::Vector2f position, float angle, sf::Vector2f startVelocity,float speed, bool playerShoot, sf::RenderWindow& window, unsigned int index);

	void Update(float dt);
	void FixedUpdate();
	void Render();

	sf::FloatRect getGlobalBounds();
};

