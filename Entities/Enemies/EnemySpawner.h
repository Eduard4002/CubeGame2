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
	//std::vector<EnemyType> types;
	std::vector<EnemySpawnInfo> enemySpawnInfo = {
	EnemySpawnInfo(0, 0.f, 120.),
	EnemySpawnInfo(1, 15.f, 300.f),
	EnemySpawnInfo(2, 60., 400.f),
	EnemySpawnInfo(3, 120.f, 600.),
	EnemySpawnInfo(4, 200.f, 800.f),
	EnemySpawnInfo(5, 300.f, 100000.f)

	// Add more enemy types and their time thresholds here
	};

	int maxAmount = 10;
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
	void EnemyKilled();
};

