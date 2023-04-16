#pragma once

#include "SFML/Graphics.hpp"
#include "Entity.h"
#include "../Items/Weapons/Weapon.h"
#include "../GameManager.h"
#include <iostream>
class Weapon;
class GameManager;
class Player : public Entity
{
private:
	GameManager& GM;
	//Variables for moving player
	sf::Vector2f velocity;
	float movingSpeed;
	float jumpingSpeed;

	sf::Vector2f currPos;//Holds current position, updates every "Update" call

	sf::RectangleShape shape;	//Shape
	sf::Vector2u shapeSize; //Size of the shape

	//is player jumping?
	bool isJumping;
	bool jumped = false;
	
	bool movingLeft; //is player moving left
	bool movingRight;//is player moving right

	

	Weapon* usingWeapon;
	bool usingLeftWeapon;
public:
	int health;
	int maxHealth = 200;

	//Inventory
	Weapon* leftWeapon;
	Weapon* rightWeapon;
public:
	Player(sf::RenderWindow& window, sf::Vector2f position, unsigned int index);
	void Update(float deltaTime);
	void FixedUpdate();
	void Render();
	sf::FloatRect getGlobalBounds();

	void outsideOfWindow();
	void setInventoryItem(bool left, Weapon* item);

	void TakeDamage(int amount);
};

