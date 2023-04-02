#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(float spawnerTime, Player* player,sf::RenderWindow& window)
{
	std::time_t time = std::time(NULL);
	srand(time);
	
	this->player = player;
	this->window = &window;

	this->spawnerTime = spawnerTime;
}

void EnemySpawner::Update(float dt)
{
	elapsedTime += dt;
	if (currAmount < maxAmount && elapsedTime >= spawnerTime) {
		//Spawn new enemy
		Enemy* enemy = new Enemy(EnemyType_Normal, player,sf::Vector2f(rand() % window->getSize().x , 0), *window, GameManager::getInstance().getNewEntityIndex());
		GameManager::getInstance().addEnemy(enemy);

		elapsedTime = 0;
		currAmount++;
	}
}
