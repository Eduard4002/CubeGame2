#pragma once
#include "Enemy.h"
#include "../GameManager.h"

class GameManager;
enum EnemyType;
class EnemySpawner
{
private:
	std::vector<EnemyType> types;

	int maxAmount = 15;
	int currAmount = 0;
	float spawnerTime;
	float elapsedTime;

	Player* player;
	sf::RenderWindow* window;
public:
	EnemySpawner(float spawnerTime,Player* player, sf::RenderWindow& window);
	void Update(float dt);
};

