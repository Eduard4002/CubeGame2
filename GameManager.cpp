#include "GameManager.h"
#include <fstream>
#include <string>
void GameManager::init(sf::RenderWindow* _window = nullptr) {
	if (initialized) {
		std::cout << "Already called Gamemanager.init()" << std::endl;
		exit(-1);
	}
	if (_window != nullptr) {
		this->window = _window;
	}
	playerPos = sf::Vector2f(100, window->getSize().y - 200);
	objectIndex = 1;
	

	
	//entities.push_back(object);
	Enemy* enemy = new Enemy(EnemyType_Easy, player, { playerPos.x + 300,playerPos.y }, *window, getNewEntityIndex());
	//enemies.push_back(enemy);
	quadTree = Quadtree(sf::FloatRect(0, 0, 912, 512));
	
	spawner = std::make_unique<EnemySpawner>(3.f, player, *window);

	UI = new UIManager(*window);

	audio = new AudioManager();
	audio->addSFX("Testing", "res/Audio/SFX/test.wav");

	audio->addMusic("Test", "res/Audio/Music/Without Fear.ogg");
	audio->addMusic("Test2", "res/Audio/Music/DeathMatch (Boss Theme).ogg");

	//audio->playMusic("Test");
	
	audio->setMusicVolume(10);

	//Get random seed
	std::time_t time = std::time(NULL);
	srand(time);

	enemySpawnTimer = 2.f;

	background = new ParallaxEffect(5);
	background->addLayer("res/city 1/1.png", 0.4f, playerPos);
	background->addLayer("res/city 1/2.png", 0.3f, playerPos);
	background->addLayer("res/city 1/3.png", 0.2f, playerPos);
	background->addLayer("res/city 1/4.png", 0.1f, playerPos);
	background->addLayer("res/city 1/5.png", 0.0f, playerPos);



	//Add platform
	sf::FloatRect firstPlatform(sf::Vector2f(0, window->getSize().y - (16 * 3)), sf::Vector2f(16 * (window->getSize().x / 16), 16 * 3));
	//sf::FloatRect secondPlatform(sf::Vector2f(16 * 35, window->getSize().y - (16 * 3)), sf::Vector2f(16 * 23, 16 * 3));

	AddPlatform(sf::Vector2f(firstPlatform.left,  firstPlatform.top),  sf::Vector2f(firstPlatform.width,  firstPlatform.height));
	//AddPlatform(sf::Vector2f(secondPlatform.left, secondPlatform.top), sf::Vector2f(secondPlatform.width, secondPlatform.height));

	
	std::vector<sf::FloatRect> platforms;
	platforms.push_back(firstPlatform);
	//platforms.push_back(secondPlatform);

	player = new Player(platforms,*window, playerPos, getNewEntityIndex());

	//Entity* object = new Platform(sf::Vector2f(50, 50), sf::Vector2f(300, window.getSize().y - 50), window, getNewEntityIndex());
	entities.push_back(player);

} 
void GameManager::AddPlatform(sf::Vector2f startPos,sf::Vector2f size) {


	int countX = size.x / 16;
	int countY = size.y / 16;
	for (int i = 0; i <= countY-1; i++) {
		Platform* tile = new Platform(sf::Vector2f(startPos.x , startPos.y + (16 * i)), *window, getNewEntityIndex());
		if (i == countY - 1) {
			tile->addTexture("res/steelPlatform/row-5-column-1.png");
		}
		else {
			tile->addTexture("res/steelPlatform/row-3-column-1.png");
		}
		platformTiles.push_back(tile);

		for (int j = 1; j <= countX - 2; j++) {
			Platform* tile = new Platform(sf::Vector2f(startPos.x + (16 * j), startPos.y + (16 * i)), *window, getNewEntityIndex());
			if (i == 0) {
				tile->addTexture("res/steelPlatform/row-1-column-3.png");
			}
			else if (i > 0 && i < countY - 1) {
				tile->addTexture("res/steelPlatform/row-3-column-3.png");
			}
			else if (i == countY - 1) {
				tile->addTexture("res/steelPlatform/row-5-column-3.png");

			}
			platformTiles.push_back(tile);
		}
	}
	
	
	Platform* startTile = new Platform(startPos, *window, getNewEntityIndex());
	startTile->addTexture("res/steelPlatform/row-1-column-1.png");
	platformTiles.push_back(startTile);

	if (size.y > 16) {
		for (int i = 0; i < countY; i++) {
			Platform* endTile = new Platform(sf::Vector2f(startPos.x + size.x - 16, startPos.y + (16 * i)), *window, getNewEntityIndex());
			if (i == 0) {
				endTile->addTexture("res/steelPlatform/row-1-column-5.png");
			}
			else if (i > 0 && i < countY -1) {
				endTile->addTexture("res/steelPlatform/row-3-column-5.png");
			}
			else if (i == countY-1) {
				endTile->addTexture("res/steelPlatform/row-5-column-5.png");
			}
			platformTiles.push_back(endTile);

		}
	}
	/*
	if (size.x > 16) {
		Platform* endTile = new Platform(sf::Vector2f(startPos.x + (size.x-16), startPos.y), *window, getNewEntityIndex());
		endTile->addTexture("res/steelPlatform/row-1-column-5.png");
		platforms.push_back(endTile);

	}*/

}
void GameManager::HandleEvent(sf::Event evnt) {
	 if (evnt.type == sf::Event::KeyPressed) {
		if (evnt.key.code == sf::Keyboard::F && indexWeapon != -1) {
			//Pickup the item
			//items.erase(items.begin() + getItemIndexFromName(itemName));
			std::cout << "Added into inventory" << droppedWeapons[indexWeapon]->name<< std::endl;
			droppedWeapons[indexWeapon]->isDropped = false;
			player->setInventoryItem(false, droppedWeapons[indexWeapon]);
			droppedWeapons.erase(droppedWeapons.begin() + indexWeapon);
		}
		else if (evnt.key.code == sf::Keyboard::T) {
			//spawn new item
			sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
			//std::string name = "AK-47_" + std::to_string(entityIndex);
			//Item* item = new Weapon(name, mousePos, sf::Vector2f(10, 10), sf::Vector2f(10, -20), "Weapon/AK-47.png", *window, getNewEntityIndex());
			//Item* item = new Weapon(Weapon_AK47, mousePos,*window, getNewEntityIndex());
			//items.push_back(item);
		}
		else if (evnt.key.code == sf::Keyboard::Z) {
			Weapon* w = new Weapon(Weapon_M4, sf::Vector2f(player->getGlobalBounds().left, player->getGlobalBounds().top), true, *window, getNewEntityIndex());
			player->setInventoryItem(false, w);
		}
		else if (evnt.key.code == sf::Keyboard::M) {
			GameOver();
		}
	 }
}
void GameManager::Update(float currDelta) {
	//Update UI
	//audio->Update();
	UI->Update(currDelta);

	if (!gameStarted) return;

	if (UI->isPaused()) {
		return;
	}
	background->Update(sf::Vector2f(player->getGlobalBounds().left, player->getGlobalBounds().top));
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->Update(currDelta);
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Update(currDelta);
	}
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Update(currDelta);
	}
	//Handle enemy spawning
	//spawner->Update(currDelta);
	
	
}
void GameManager::FixedUpdate() {
	
	if (!gameStarted) return;
	if (UI->isPaused()) {
		return;
	}
	//Clear the quad tree
	quadTree.clear();

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();
	}
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->isOutsideOfWindow(bullets[i]->getGlobalBounds())) {
			bullets[i]->toRemove = true;
		}
		bullets[i]->FixedUpdate();
		//Insert a new point into the quad tree with the updated position
		Point* p = new Point("Bullet", bullets[i]->getGlobalBounds(), i);
		quadTree.insert(p);
	}
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->FixedUpdate();
		//Insert a new point into the quad tree with the updated position
		Point* p = new Point("Enemy", enemies[i]->getGlobalBounds(),i);
		quadTree.insert(p);
	}

	Point* playerPoint = new Point("Player", player->getGlobalBounds(), -1);
	quadTree.insert(playerPoint);

	for (int i = 0; i < bullets.size(); i++) {
		std::vector<Point*> points = quadTree.queryRange(bullets[i]->getGlobalBounds());
		for (int j = 0; j < points.size(); j++) {
			//We dont want collision detection with bullets and itself
			if (points[j]->tag != "Bullet") {
				if (points[j]->tag == "Enemy" && bullets[i]->playerShoot && enemies[points[j]->index]->getGlobalBounds().intersects(bullets[i]->getGlobalBounds())) {
					enemies[points[j]->index]->TakeDamage(bullets[i]->damageAmount);
					bullets[i]->toRemove = true;
				}
				else if (points[j]->tag == "Player" && !bullets[i]->playerShoot && player->getGlobalBounds().intersects(bullets[i]->getGlobalBounds())) {
					player->TakeDamage(bullets[i]->damageAmount);
					bullets[i]->toRemove = true;
				}


			}
			
		}
	}
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->toRemove) { 
			bullets.erase(bullets.begin() + i);
		}
	}
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->toRemove) {
			currentScore += enemies[i]->score;
			enemies.erase(enemies.begin() + i);
		}
	}

	
	//Check for item collision
	/*
	for (auto& item : items) {
		if (!item->isInInventory && item->isDropped && Physics::isColliding(player->getGlobalBounds(), item->getPickableZoneBounds())) {
			std::cout << "Press E to pickup item: "<<item->name << std::endl;
			itemName = item->name;
		}
		else {
			itemName = "";
		}
		item->FixedUpdate();
	}*/
	for (int i = 0; i < droppedWeapons.size(); i++){
		if (Physics::isColliding(player->getGlobalBounds(), droppedWeapons[i]->getPickableZoneBounds())) {
			//std::cout << "Press F to pickup item: " << droppedWeapons[i]->name << std::endl;
			indexWeapon = i;
		}
		else {
			indexWeapon = -1;
		}
		droppedWeapons[i]->FixedUpdate();

		//TODO Add a function for dropped weapons
			
	}
}

void GameManager::Render() {
	if (onMainMenu) {
		UI->Render();
	}

	if (!gameStarted) return;

	background->Render(*window);
	for (int i = 0; i < platformTiles.size(); i++) {
		platformTiles[i]->Render();
	}
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->Render();
	}
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Render();
	}
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Render();
	}
	for (int i = 0; i < items.size(); i++) {
		items[i]->Render();
	}
	for (int i = 0; i < droppedWeapons.size(); i++) {
		droppedWeapons[i]->Render();
	}
	
	if (showQuadTree) {
		quadTree.show(*window);
	}
	if (!onMainMenu) {
		UI->Render();
	}
}
int GameManager::getItemIndexFromName(std::string _name)
{
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->name == _name) {
			return i;
		}
	}
	return 0;
}

void GameManager::addEntity(Entity* entity) {
	entities.push_back(entity);
}

void GameManager::addBullet(Bullet* bullet) { 
	bullets.push_back(bullet); 
}
void GameManager::addEnemy(Enemy* enemy){
	enemies.push_back(enemy);
}
void GameManager::addItem(Item* item){
	items.push_back(item);

}
void GameManager::RemoveEnemy(int enemyID, int weaponID)
{
	//Delete enemy object
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->index == enemyID) {
			enemies.erase(enemies.begin() + i);
		}
	}
	//Delete weapon object
	for (int i = 0; i < items.size(); i++) {
		if (items[i]->index == weaponID) {
			items.erase(items.begin() + i);
		}
	}
}
int GameManager::findVecIndex(int bulletID)
{
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->index = bulletID) {
			return i;
		}
	}
	return -1;
}
unsigned int GameManager::getNewEntityIndex() {
	return objectIndex++;
}
void GameManager::getPlayerHealth(int& currHealth, int& maxHealth)
{
	currHealth = player->health;
	maxHealth =  player->maxHealth;
}
bool GameManager::getPlayerLeftInventory(short& iconX, short& iconY) {
	iconX = player->leftWeapon->iconX;
	iconY = player->leftWeapon->iconY;

	if (player->holdingLeftWeapon()) {
		return true;
	}
	return false;
}
bool GameManager::getPlayerRightInventory(short& iconX, short& iconY) {
	if (player->rightWeapon != nullptr) {
		iconX = player->rightWeapon->iconX;
		iconY = player->rightWeapon->iconY;
	}else{
		iconX = -1;
		iconY = -1;
	}

	if (!player->holdingLeftWeapon()) {
		return true;
	}
	return false;
	
}
void GameManager::ShutDown() {
	UI->ShutDown();
}
int GameManager::getRandomInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}
void GameManager::RestartGame() {
	std::cout << "restart";
	entities.clear();
	items.clear();
	bullets.clear();
	enemies.clear();
	droppedWeapons.clear();

	player->Reset();
	entities.push_back(player);

	spawner->Reset();

	UI->Restart();
	UI->setPanel(PanelType_MainPanel);
}

void GameManager::GameOver()
{

	std::fstream file;
	file.open("save.txt");
	std::string line;

	std::getline(file, line);
	int shiftNum = std::stoi(line);
	std::getline(file, line);
	long encHighScore = std::stoi(line);

	//std::cout << "DEC: " << (highScore >> shiftNum) << std::endl;
	//Delete previous line and insert new highscore
	if (currentScore >= (encHighScore >> shiftNum)) {
		std::ofstream temp;
		temp.open("temp.txt", std::ios::trunc);
		short shiftNum = getRandomInt(1, 255);
		
		while ((currentScore << shiftNum) <= 0 ) {
			shiftNum = getRandomInt(1, 255);
		}

		//std::cout << "SHIFT NUM: " << shiftNum << std::endl;
		//std::cout << "ENC: " << (currentScore << shiftNum) << std::endl;
		temp << std::to_string(shiftNum) + "\n";
		temp << std::to_string(currentScore << shiftNum);

		temp.close();
		file.close();
		remove("save.txt");
		
		if (rename("temp.txt", "save.txt") != 0)
			std::cout << "Error renaming file \n";

		UI->highscore = currentScore;
	}
	else {
		UI->highscore = (encHighScore >> shiftNum);
	}
	
	UI->currPanel = PanelType_GameOverPanel;
}

int GameManager::getCurrentScore()
{
	return currentScore;
}

void GameManager::setVSync(bool set)
{
	window->setVerticalSyncEnabled(set);
}

void GameManager::startGame()
{
	gameStarted = true;
	onMainMenu = false;
	UI->setPanel(PanelType_MainPanel);
}




