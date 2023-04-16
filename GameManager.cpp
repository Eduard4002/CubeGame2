#include "GameManager.h"
void GameManager::init(sf::RenderWindow& window) {
	if (initialized) {
		std::cout << "Already called Gamemanager.init()" << std::endl;
		exit(-1);
	}
	this->window = &window;
	playerPos = sf::Vector2f(100, window.getSize().y - 50);
	player = new Player(window,playerPos, getNewEntityIndex());

	//Entity* object = new Rectangle(sf::Vector2f(50, 50), sf::Vector2f(300, window.getSize().y - 50), window, getNewEntityIndex());
	entities.push_back(player);
	//entities.push_back(object);
	Enemy* enemy = new Enemy(EnemyType_Easy, player, { playerPos.x + 300,playerPos.y }, window, getNewEntityIndex());
	//enemies.push_back(enemy);
	quadTree = Quadtree(sf::FloatRect(0, 0, 912, 512));
	
	spawner = std::make_unique<EnemySpawner>(3.f, player, window);

	UI = new UIManager(window);
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
	 }
}
void GameManager::Update(float currDelta) {
	

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
	
	//Update UI
	UI->Update(currDelta);
}
void GameManager::FixedUpdate() {
	//Clear the quad tree
	quadTree.clear();
	//playerPos = sf::Vector2f(entities[0]->getGlobalBounds().left, entities[0]->getGlobalBounds().top);
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->FixedUpdate();
	}
	for (int i = 0; i < bullets.size(); i++) {
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
			std::cout << "Press F to pickup item: " << droppedWeapons[i]->name << std::endl;
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
	UI->Render();
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
void GameManager::getPlayerLeftInventory(short& iconX, short& iconY) {
	iconX = player->leftWeapon->iconX;
	iconY = player->leftWeapon->iconY;
}
void GameManager::getPlayerRightInventory(short& iconX, short& iconY) {
	if (player->rightWeapon != nullptr) {
		iconX = player->rightWeapon->iconX;
		iconY = player->rightWeapon->iconY;
	}
	
}
void GameManager::ShutDown() {
	UI->ShutDown();
}




