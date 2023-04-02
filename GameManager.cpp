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
	Enemy* enemy = new Enemy(EnemyType_Normal,player, sf::Vector2f(400,playerPos.y), window, getNewEntityIndex());
	enemies.push_back(enemy);
	//quadTree = Quadtree(sf::FloatRect(0, 0, 912, 512));
	
	spawner = std::make_unique<EnemySpawner>(5.f, player, window);

	rec.setFillColor(sf::Color::Transparent);
	rec.setOutlineColor(sf::Color::Red);
	rec.setOutlineThickness(2);

	rec.setSize(sf::Vector2f(50, 50));
	rec.setPosition(sf::Vector2f(400, playerPos.y));
} 
void GameManager::HandleEvent(sf::Event evnt) {
	 if (evnt.type == sf::Event::KeyPressed) {
		if (evnt.key.code == sf::Keyboard::E && itemName != "") {
			//Pickup the item
			items.erase(items.begin() + getItemIndexFromName(itemName));
		}
		else if (evnt.key.code == sf::Keyboard::T) {
			//spawn new item
			sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
			//std::string name = "AK-47_" + std::to_string(entityIndex);
			//Item* item = new Weapon(name, mousePos, sf::Vector2f(10, 10), sf::Vector2f(10, -20), "Weapon/AK-47.png", *window, getNewEntityIndex());
			//Item* item = new Weapon(Weapon_AK47, mousePos,*window, getNewEntityIndex());
			//items.push_back(item);
			
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
	//quadTree.insert(Point("Player", player->getGlobalBounds(), -1));
	//Handle enemy spawning
	//spawner->Update(currDelta);
	
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
					std::cout << "Enemy took damage" << std::endl;
					enemies[points[j]->index]->TakeDamage(bullets[i]->damageAmount);
					bullets[i]->toRemove = true;
				}
				else if (points[j]->tag == "Player" && !bullets[i]->playerShoot && player->getGlobalBounds().intersects(bullets[i]->getGlobalBounds())) {
					std::cout << "Player took damage" << std::endl;
					player->TakeDamage(bullets[i]->damageAmount);
					bullets[i]->toRemove = true;
				}


			}
			
		}
	}
	/*
	for (int i = 0; i < enemies.size(); i++) {

		std::vector<Point*> points = quadTree.queryRange(enemies[i]->getGlobalBounds());
		for (int j = 0; j < points.size(); j++) {
			//We dont want collision detection with enemies and itself
			if (points[j]->tag != "Enemy") {
				if (points[j]->tag == "Bullet" && bullets[points[j]->index]->playerShoot && enemies[i]->getGlobalBounds().intersects(bullets[points[j]->index]->getGlobalBounds())) {
					std::cout << "Enemy took damage" << std::endl;
					bullets[points[j]->index]->toRemove = true;
				}
				
				
			}
			//collision with player
			else if (points[j]->index == -1) {

			}
		}
	}*/
	
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->toRemove) { 
			bullets.erase(bullets.begin() + i);
		}
	}
/*
	//Checking for bullets colliding with enemies/player
	for (int i = 0; i < bullets.size();i++) {
		for (int j = 0; j < enemies.size(); j++) {
			if (bullets[i]->playerShoot && enemies[j]->getGlobalBounds().intersects(bullets[i]->getGlobalBounds())) {
				//take damage
				enemies[j]->TakeDamage(bullets[i]->damageAmount);
				bullets[i]->toRemove = true;
			}
			else if (!bullets[i]->playerShoot && player->getGlobalBounds().intersects(bullets[i]->getGlobalBounds())) {
				player->TakeDamage(bullets[i]->damageAmount);
				bullets[i]->toRemove = true;

			}
			
		}
		
	}
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->toRemove) {
			bullets.erase(bullets.begin() + i);
		}
	}
	*/
	//Check for item collision
	for (auto& item : items) {
		if (!item->isInInventory && item->isDropped && Physics::isColliding(entities[0]->getGlobalBounds(), item->getPickableZoneBounds())) {
			std::cout << "Press E to pickup item: "<<item->name << std::endl;
			itemName = item->name;
		}
		else {
			itemName = "";
		}
		item->FixedUpdate();
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
	if (showQuadTree) {
		quadTree.show(*window);
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





