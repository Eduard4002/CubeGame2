#pragma once
#include "../Entity.h"
#include "../../GameManager.h"
#include "../../Items/Weapons/Weapon.h"
#include "../Entities/Player.h"
class GameManager;
class Weapon;
class Player;
enum EnemyType {
	EnemyType_Easy = 0,
	EnemyType_Normal = 1,
	EnemyType_Fast = 2,
	EnemyType_Tank = 3,
	EnemyType_Sniper = 4
};
class Enemy : public Entity
{
private:
	//GameManager& gm;
	sf::RectangleShape shape;
	Weapon* currWeapon;

	int health;
	float speed;
	float stopFollowing; //how far should the enemy be to stop following and start shooting

	Player* playerEntity;

	sf::RectangleShape stopFollowingShape;

	bool startShoot;
	sf::Vector2f playerPos; //Where is player? Updates every "FixedUpdate" call

	sf::Vector2f velocity;

private:
	void setCharacteristics(EnemyType type,sf::Vector2f pos);
public:
	Enemy(EnemyType type, Player* player,sf::Vector2f position, sf::RenderWindow& window, unsigned int index);
	void FixedUpdate();
	void Update(float dt);
	void Render();
	sf::FloatRect getGlobalBounds();

	void TakeDamage(int amount);
	int getHealth();

	void getSpawnThreshold(float& min, float& max);
};

