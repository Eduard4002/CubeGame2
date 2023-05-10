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
    totalSurvivalTime += dt;
	if (currAmount < maxAmount && elapsedTime >= spawnerTime) {
        // Reset elapsedTime
        elapsedTime = 0.f;
        currAmount++;

        // Calculate the spawn probabilities for each enemy type
        std::vector<float> spawnProbabilities;
        float totalProbability = 0.f;
        for (const auto& spawnInfo : enemySpawnInfo) {
            float probability = 0.f;
            // Check if the player's survival time is within the spawn time thresholds of the current enemy type
            if (totalSurvivalTime >= spawnInfo.minTime && totalSurvivalTime <= spawnInfo.maxTime) {
                // If it is, set the probability for this enemy type to 1
                probability = 1.f;
            }
            // Add the calculated probability to the spawnProbabilities vector and update the totalProbability
            spawnProbabilities.push_back(probability);
            totalProbability += probability;
        }

        // Normalize the probabilities so that they sum up to 1
        for (float& probability : spawnProbabilities) {
            probability /= totalProbability;
        }

        // Choose a random enemy type based on the calculated probabilities
        // Generate a random number between 0 and 1
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float cumulativeProbability = 0.f;
        int chosenType = enemySpawnInfo.back().type; // Default to the last enemy type in the list
        for (size_t i = 0; i < enemySpawnInfo.size(); ++i) {
            // Update the cumulativeProbability with the probability of the current enemy type
            cumulativeProbability += spawnProbabilities[i];
            // Check if the randomValue is less than or equal to the cumulativeProbability
            if (randomValue <= cumulativeProbability) {
                // If it is, choose the current enemy type and break the loop
                chosenType = enemySpawnInfo[i].type;
                break;
            }
        }

        // Spawn a new enemy with the chosen type
        Enemy* enemy = new Enemy(static_cast<EnemyType>(chosenType), player, sf::Vector2f(rand() % window->getSize().x, 0), *window, GameManager::getInstance().getNewEntityIndex());
        GameManager::getInstance().addEnemy(enemy);
	}
}

void EnemySpawner::Reset()
{
    totalSurvivalTime = 0.f;
}
