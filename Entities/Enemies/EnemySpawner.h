#pragma once
#include "Enemy.h"
#include "../GameManager.h"

class GameManager;
enum EnemyType;
struct EnemySpawnInfo {
	int type;
	float minTime;
	float maxTime;
};
class EnemySpawner
{
private:
	std::vector<EnemyType> types;
	std::vector<EnemySpawnInfo> enemySpawnInfo = {
	EnemySpawnInfo(0, 0.f, 20.f),
	EnemySpawnInfo(1, 5.f, 20.f)
	// Add more enemy types and their time thresholds here
	};

	int maxAmount = 15;
	int currAmount = 0;
	float spawnerTime;
	float elapsedTime;

	Player* player;
	sf::RenderWindow* window;

	float totalSurvivalTime;
public:
	EnemySpawner(float spawnerTime,Player* player, sf::RenderWindow& window);
	void Update(float dt);
	void Reset();
};

