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
	playerPos = sf::Vector2f(window->getSize().x / 2, window->getSize().y - 48);
	objectIndex = 1;
	

	quadTree = Quadtree(sf::FloatRect(0, 0, 912, 512));
	

	UI = new UIManager(*window);

	audio = new AudioManager();
	//audio->playSFX("res/Audio/SFX/Weapon/Isolated/.22LR/WAV/22LR Single Isolated WAV.wav");
	audio->addMusic("1", "res/Audio/Music/Without Fear.ogg");
	audio->addMusic("2", "res/Audio/Music/Alone Against Enemy.ogg");
	audio->addMusic("3", "res/Audio/Music/Rain of Lasers.ogg");
	audio->addMusic("4", "res/Audio/Music/Space Heroes.ogg");
	audio->addMusic("5", "res/Audio/Music/Battle in the Stars.ogg");
	audio->addMusic("6", "res/Audio/Music/Epic End.ogg");


	//audio->playMusic("1");
	audio->playCustomMusic("res/Audio/Music/SkyFire (Title Screen) (1).ogg",true);
	
	//Get random seed
	std::time_t time = std::time(NULL);
	srand(time);


	background = new ParallaxEffect(5);
	std::ifstream file;
	file.open("data.txt");
	std::string index;
	std::getline(file, index);
	setBackground(std::stoi(index));

	UI->currBackgroundIndex = (short)std::stoi(index);
	if (!platformTexture.loadFromFile("res/steelPlatform/main.png")) {
		std::cout << "Error loading file: res/steelPlatform/main.png" << std::endl;
	}
	platform.setTexture(platformTexture);

	//Add platform
	sf::FloatRect firstPlatform(sf::Vector2f(0, window->getSize().y - (16 * 3)), sf::Vector2f(window->getSize().x, 16 * 3));


	platform.setPosition(firstPlatform.left, firstPlatform.top);
	//platforms.push_back(secondPlatform);

	player = new Player(firstPlatform,*window, playerPos, getNewEntityIndex());

	entities.push_back(player);

	spawner = std::make_unique<EnemySpawner>(3.f, player, *window);

	if (sf::Shader::isAvailable) {
		if (!shader.loadFromFile("res/Shader/Shader.txt", sf::Shader::Fragment)) {
			//error
			std::cout << "Couldnt open file" << std::endl;
		}
		else {
			shaderShape.setSize(static_cast<sf::Vector2f>(window->getSize()));
			shader.setUniform("u_resolution", sf::Glsl::Vec2(window->getSize()));
			shaderAvailable = true;
		}
	}


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
			droppedWeapons[indexWeapon].weapon->isDropped = false;
			droppedWeapons[indexWeapon].weapon->playerUsing = true;
			//if player doesnt have anything in the right inventory then add the weapon to right inventory
			if (player->rightWeapon == nullptr) {
				player->setInventoryItem(false, droppedWeapons[indexWeapon].weapon);
			}
			else {
				//otherwise, add the new weapon to the current holding one
				if (player->holdingLeftWeapon()) {
					player->setInventoryItem(true, droppedWeapons[indexWeapon].weapon);
				}
				else {
					player->setInventoryItem(false, droppedWeapons[indexWeapon].weapon);
				}
			}
			//player->setInventoryItem(false, droppedWeapons[indexWeapon].weapon);
			droppedWeapons.erase(droppedWeapons.begin() + indexWeapon);
		}
	 }
	 else if (evnt.type == sf::Event::LostFocus && UI->currPanel == PanelType_MainPanel) {
		 //UI->setPanel(PanelType_PausePanel);
		 UI->currPanel = PanelType_PausePanel;
	 }

}
void GameManager::Update(float currDelta) {
	
	
	
	//Update UI
	UI->Update(currDelta);

	if (!gameStarted) return;
	audio->Update();
	if (UI->isPaused()) {
		return;
	}
	//Handle enemy spawning
	spawner->Update(currDelta);
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
	for (int i = 0; i < droppedWeapons.size(); i++) {
		droppedWeapons[i].currDisTime -= currDelta;
		if (droppedWeapons[i].currDisTime <= 0) {
			droppedWeapons.erase(droppedWeapons.begin() + i);
		}
	}
	
	
	
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
			spawner->EnemyKilled();

			if (player->health <= 80 && getRandomInt(0, 100) < 40) {
				player->health += 20;
			}
		}
	}
	UI->showPickup = false;

	for (int i = 0; i < droppedWeapons.size(); i++){
		if (Physics::isColliding(player->getGlobalBounds(), droppedWeapons[i].weapon->getPickableZoneBounds())) {
			UI->showPickup = true;
			UI->weaponName = droppedWeapons[i].weapon->name;
			UI->playerPos = sf::Vector2f(player->getGlobalBounds().left, player->getGlobalBounds().top);
			
			indexWeapon = i;
		}
		else {

			indexWeapon = -1;
		}
		droppedWeapons[i].weapon->FixedUpdate();

		
	}
}

void GameManager::Render() {
	
	
	
	background->Render(*window);
	window->draw(platform);
	
	UI->Render();
	
	if (!gameStarted) return;
	
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
		droppedWeapons[i].weapon->Render();
	}
	
	if (showQuadTree) {
		quadTree.show(*window);
	}
	if (!onMainMenu) {
		UI->Render();
	}
	if (shaderAvailable) {
		this->window->draw(shaderShape, &shader);
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
	if (enemy->type != EnemyType_Explosive) {
		enemy->currWeapon->audio->setMusicVolume(UI->currMusicVol);
		enemy->currWeapon->audio->setSFXVolume(UI->currSFXVol);
	}
	
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
bool GameManager::getPlayerLeftInventory(short& iconX, short& iconY,int& ammoAmount) {
	iconX = player->leftWeapon->iconX;
	iconY = player->leftWeapon->iconY;
	ammoAmount = player->leftWeapon->getAmmoAmount();
	if (player->holdingLeftWeapon()) {
		return true;
	}
	return false;
}
bool GameManager::getPlayerRightInventory(short& iconX, short& iconY,int& ammoAmount) {
	if (player->rightWeapon != nullptr) {
		iconX = player->rightWeapon->iconX;
		iconY = player->rightWeapon->iconY;
		ammoAmount = player->rightWeapon->getAmmoAmount();

	}else{
		iconX = -1;
		iconY = -1;
		ammoAmount = -1;

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
	entities.clear();
	items.clear();
	bullets.clear();
	enemies.clear();
	droppedWeapons.clear();

	entities.push_back(player);

	currentScore = 0;

	player->Reset();

	spawner->Reset();

	UI->Restart();
	UI->setPanel(PanelType_MainPanel);
}
void GameManager::Encrypt(std::string& data, std::string key)
{
	for (unsigned i = 0; i < data.size(); i++)
		data[i] ^= key[i % key.size()];
}

void GameManager::Decrypt(std::string& data, std::string key)
{
	for (unsigned i = 0; i < data.size(); i++)
		data[i] ^= key[i % key.size()];
}



void GameManager::setBackground(short index)
{
	background->removeLayers();
	background->addLayer(("res/city " + std::to_string(index) + "/1.png"), 0.4f);
	background->addLayer(("res/city " + std::to_string(index) + "/2.png"), 0.3f);
	background->addLayer(("res/city " + std::to_string(index) + "/3.png"), 0.2f);
	background->addLayer(("res/city " + std::to_string(index) + "/4.png"), 0.1f);
	background->addLayer(("res/city " + std::to_string(index) + "/5.png"), 0.0f);
}

void GameManager::saveBackground(short index)
{
	std::fstream file;
	file.open("data.txt");

	std::ofstream temp;
	temp.open("temp2.txt", std::ios::trunc);

	temp << std::to_string(index);

	temp.close();
	file.close();
	remove("data.txt");
	rename("temp2.txt", "data.txt");
}

void GameManager::setMusicVolume(int vol)
{
	audio->setMusicVolume(vol);
	
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->type != EnemyType_Explosive) {
			enemies[i]->currWeapon->audio->setMusicVolume(vol);
		}
	}
	player->usingWeapon->audio->setMusicVolume(vol);
}

void GameManager::setSFXVolume(int vol)
{
	audio->setSFXVolume(vol);
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->type != EnemyType_Explosive) {
			enemies[i]->currWeapon->audio->setSFXVolume(vol);
		}
	}
	player->usingWeapon->audio->setSFXVolume(vol);
}

void GameManager::GameOver()
{
	
	audio->playCustomMusic("res/Audio/Music/Defeated (Game Over Tune).ogg", false);

	UI->currPanel = PanelType_GameOverPanel;

	std::string key = "YES";
	std::fstream file;
	file.open("save.txt");
	std::string line;
	//get the encrypted line
	std::getline(file, line);
	std::string data = line;

	int decHighScore = 0;
	//If its the first time saving, there isnt anything to decrypt
	if (line != "") {

		Decrypt(data, key);

		decHighScore = std::stoi(data);
	}
	

	if (currentScore > decHighScore) {
		std::ofstream temp;
		temp.open("temp.txt", std::ios::trunc);

		data = std::to_string(currentScore);
		Encrypt(data, key);
		//temp << std::to_string(shiftNum) + "\n";
		temp << data;

		temp.close();
		file.close();
		remove("save.txt");

		if (rename("temp.txt", "save.txt") != 0)
			std::cout << "Error renaming file \n";

		UI->highscore = currentScore;
	}
	else {
		UI->highscore = decHighScore;
	}
	UI->score = currentScore;

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
	audio->stopCustomMusic();
	gameStarted = true;
	onMainMenu = false;
	audio->playMusic("1");
	UI->setPanel(PanelType_MainPanel);
}
void GameManager::quitGame()
{
	audio->playCustomMusic("res/Audio/Music/SkyFire (Title Screen) (1).ogg", true);

	gameStarted = false;
	onMainMenu = true;
	RestartGame();
	UI->currPanel = PanelType_MainMenuPanel;

}




