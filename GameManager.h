#pragma once
#include "SFML/Graphics.hpp"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/Rectangle.h"
#include "Entities/Enemies/Enemy.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Item.h"
#include "Animation.h"
#include "Quadtree.h"
#include "Entities/Enemies/EnemySpawner.h"

class Enemy;
class Weapon;
class Item;
class EnemySpawner;

class GameManager
{
private:
	std::vector<Entity*> entities;
	std::vector<Item*> items;
	std::vector<Bullet*> bullets;
	std::vector<Enemy*> enemies;

	Quadtree quadTree;
	bool showQuadTree = true;
	Player* player;
	unsigned int objectIndex = 1;

	bool initialized = false;
	sf::RenderWindow* window;

	sf::Vector2f playerPos;
	//If we are colliding with a item, the name will be shown here
	std::string itemName = "";

	float enemySpawnTimer = 2.f;
	float currTime = 0.f;

	std::unique_ptr<EnemySpawner> spawner;
	sf::RectangleShape rec;
private:
	GameManager() : quadTree(sf::FloatRect(0,0,912,512)){};
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	int getItemIndexFromName(std::string name);
public:
	static GameManager& getInstance() {
		static GameManager instance;
		return instance;
	}
	void init(sf::RenderWindow& window);
	void Update(float currDelta);
	void FixedUpdate();
	void Render();

	void HandleEvent(sf::Event event);

	void addEntity(Entity* entity);
	void addBullet(Bullet* bullet);
	void addEnemy(Enemy* enemy);
	void addItem(Item* item);

	void RemoveEnemy(int enemyID, int weaponID);

	int findVecIndex(int bulletID);

	unsigned int getNewEntityIndex();

};

