#pragma once
#include "SFML/Graphics.hpp"
#include "Entities/Entity.h"
#include "Entities/Player.h"
#include "Entities/Enemies/Enemy.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Item.h"
#include "Animation.h"
#include "Quadtree.h"
#include "Entities/Enemies/EnemySpawner.h"
#include "UI/UIManager.h"
#include "Audio/AudioManager.h"
#include "ParallaxEffect.h"
#include "Entities/Platform.h"

class Enemy;
class Weapon;
class Item;
class EnemySpawner;
class UIManager;
struct DroppedWeapon {
	Weapon* weapon;
	float currDisTime;
};
class GameManager
{
public:
	std::vector<Entity*> entities;
	std::vector<Item*> items;
	std::vector<Bullet*> bullets;
	std::vector<Enemy*> enemies;
	std::vector<DroppedWeapon> droppedWeapons;
	std::vector<Platform*> platformTiles;
	
	UIManager* UI;
	AudioManager* audio;

	ParallaxEffect* background;
	float despawnTime = 10.f;

	bool onMainMenu = true;

private:
	Quadtree quadTree;
	bool showQuadTree = false;
	Player* player;
	unsigned int objectIndex;

	bool initialized;
	sf::RenderWindow* window;

	sf::Vector2f playerPos;
	//If we are colliding with a item, the index in the "droppedWeapons" will be shown here
	int indexWeapon = -1;

	float enemySpawnTimer;
	float currTime;

	int currentScore = 0;

	std::unique_ptr<EnemySpawner> spawner;


	bool gameStarted = false;

	sf::Sprite platform;
	sf::Texture platformTexture;

	sf::Shader shader;
	sf::RectangleShape shaderShape;

	bool shaderAvailable = false;


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
	void init(sf::RenderWindow* window);
	void Update(float currDelta);
	void FixedUpdate();
	void Render();

	void AddPlatform(sf::Vector2f position, sf::Vector2f size);

	void HandleEvent(sf::Event event);
	void ShutDown();

	void addEntity(Entity* entity);
	void addBullet(Bullet* bullet);
	void addEnemy(Enemy* enemy);
	void addItem(Item* item);

	void RemoveEnemy(int enemyID, int weaponID);

	int findVecIndex(int bulletID);

	unsigned int getNewEntityIndex();

	void getPlayerHealth(int& currHealth, int& maxHealth);
	bool getPlayerLeftInventory(short& iconX, short& iconY, int& ammoAmount);
	bool getPlayerRightInventory(short& iconX, short& iconY,int& ammoAmount);

	int getRandomInt(int min, int max);

	void RestartGame();
	void GameOver();

	int getCurrentScore();

	void setVSync(bool set);

	void startGame();
	void quitGame();


	//Very simple Encrypt/Decrypt for saving and loading highscore
	void Encrypt(std::string& data, std::string key);
	void Decrypt(std::string& data, std::string key);

	void setBackground(short index);
	void saveBackground(short index);

	void setMusicVolume(int vol);
	void setSFXVolume(int vol);
};

