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
	float dashSpeed;

	sf::Vector2f currPos; //Holds current position, updates every "Update" call
	sf::Vector2f startPos;//Holds position when restarting the game

	sf::RectangleShape shape;	//Shape
	sf::Vector2u shapeSize;     //Size of the shape

	//is player jumping?
	bool isJumping;
	bool jumped;

	bool _isOnPlatform;
	
	bool movingLeft; //is player moving left
	bool movingRight;//is player moving right

	//Dashing
	bool isDashing;
	bool spawnedDashEffects;
	bool removedDashEffects;

	bool showEffect = true;

	float removingEffectSpeed;
	float dashTime;
	float dashDuration;

	Weapon* defaultWeapon;

	// Other member variables
	sf::RectangleShape dashEffect1;
	sf::RectangleShape dashEffect2;
	sf::RectangleShape dashEffect3;

	// Function to spawn the dash effects
	void SpawnDashEffects();
	void RemoveDashEffects();

	//Debugging
	bool showGameOver = true;

	sf::FloatRect mainPlatform;
public:
	int health;
	int maxHealth = 100;

	//Inventory
	Weapon* leftWeapon;
	Weapon* rightWeapon = nullptr;
	Weapon* usingWeapon;

public:
	Player(sf::FloatRect platformInfo,sf::RenderWindow& window, sf::Vector2f position, unsigned int index);
	void Update(float deltaTime);
	void FixedUpdate();
	void Render();
	sf::FloatRect getGlobalBounds();

	void outsideOfWindow();
	void setInventoryItem(bool left, Weapon* item);

	void TakeDamage(int amount);
	bool holdingLeftWeapon();

	void Reset();

	int isOnPlatform();
};

