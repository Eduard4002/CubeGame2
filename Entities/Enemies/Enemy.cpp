#include "Enemy.h"



Enemy::Enemy(EnemyType type, Player* player, sf::Vector2f position, sf::RenderWindow& window, unsigned int index) : Entity(window,index)
{
	shape.setPosition(position);
	playerEntity = player;
	tag = "Enemy";
	setCharacteristics(type, position);
	stopFollowingShape.setSize({ stopFollowing, stopFollowing});
	stopFollowingShape.setOutlineColor(sf::Color::Green);
	stopFollowingShape.setOutlineThickness(2);

	stopFollowingShape.setOrigin(sf::Vector2f( stopFollowing / 2 - shape.getSize().x / 2, stopFollowing / 2 - shape.getSize().y / 2));
	stopFollowingShape.setPosition(position);

	stopFollowingShape.setFillColor(sf::Color::Transparent);

	//For the explosive type
	explosiveShape.setSize(sf::Vector2f(128, 128));
	explosiveShape.setOrigin(sf::Vector2f(shape.getSize().x , shape.getSize().y +32));

	explosive = new Animation(explosiveShape, 64, 64);
	explosive->addAnimation("Explosion/spritesheet.png", 0, 0, 36, 1.25f / 36);
}

void Enemy::FixedUpdate()
{
	startShoot = false;
	//Move towards player
	playerPos = sf::Vector2f(playerEntity->getGlobalBounds().left, playerEntity->getGlobalBounds().top);
	velocity = Physics::normalize(playerPos - shape.getPosition());


	velocity.y += Physics::GRAVITY();
	if (this->shape.getGlobalBounds().top + this->shape.getGlobalBounds().height > windowSize.y - platformHeight) {
		velocity.y = 0;
	}

	//Follow the player only if we are outside range
	if (type != EnemyType_Explosive && stopFollowingShape.getGlobalBounds().contains(playerPos)) {
		velocity.x = 0;
		//Start shooting 
		startShoot = true;

	}
	shape.move(velocity * speed);
	stopFollowingShape.move(velocity * speed);
	if (type != EnemyType_Explosive) {
		currWeapon->SetPosition(shape.getPosition());
	}
	else {
		explosiveShape.setPosition(shape.getPosition());
	}

	
}

void Enemy::Update(float dt)
{
	if (type == EnemyType_Explosive && shape.getGlobalBounds().intersects(playerEntity->getGlobalBounds())) {
		//explosive->playAnimation(0, dt);
		enemyExploded = true;
		GameManager::getInstance().UI->setPaused(true);
	}
	if (enemyExploded) {
		explosive->playAnimation(0, dt);
	}
	if (enemyExploded && explosive->isAnimationFinished(0)) {
		toRemove = true;
		playerEntity->TakeDamage(200);
	}
	if (type != EnemyType_Explosive) {
		currWeapon->RotateToPoint(playerPos);
		currWeapon->UpdateWeapon(dt, playerPos, startShoot);
		if (currWeapon->getAmmoAmount() == 0) {
			currWeapon->Reload();
		}
	}
	
}

void Enemy::Render()
{
	if (!enemyExploded) {
		window->draw(shape);
	}
	else {
		window->draw(explosiveShape);
	}
	if (type != EnemyType_Explosive) {
		currWeapon->Render();
	}
	
}

sf::FloatRect Enemy::getGlobalBounds()
{
	return shape.getGlobalBounds();
}

void Enemy::TakeDamage(int amount)
{

	if (health - amount <= 0) {
		//Enemy dead 
		toRemove = true;
		if (type != EnemyType_Explosive) {
			currWeapon->isDropped = true;
			currWeapon->Reset();
			GameManager::getInstance().droppedWeapons.push_back(DroppedWeapon(currWeapon, GameManager::getInstance().despawnTime));
		}
		return;
	}
	health -= amount;
}

int Enemy::getHealth()
{
	return health;
}



void Enemy::setCharacteristics(EnemyType type,sf::Vector2f pos)
{
	this->type = type;
	shape.setSize(sf::Vector2f(50, 50));
	switch (type) {
	case EnemyType_Easy:
		shape.setFillColor(sf::Color::Green);
		speed = 3;
		stopFollowing = 300;
		currWeapon = new Weapon(Weapon_Pistol, pos, false, *window, GameManager::getInstance().getNewEntityIndex());
		health = 50;

		score = 100;
		break;
	case EnemyType_Normal:
		
		shape.setFillColor(sf::Color::Cyan);
		speed = 2.5f;
		stopFollowing = 400;
		currWeapon = new Weapon(Weapon_M4, pos, false, *window, GameManager::getInstance().getNewEntityIndex());

		health = 100;

		score = 200;
		break;
	case EnemyType_Harder:
		shape.setFillColor(sf::Color(255,165,0,255));
		//shape.setFillColor(sf::Color(0,0,0,255));
		speed = 3.f;
		stopFollowing = 400;
		currWeapon = new Weapon(Weapon_AK47, pos, false, *window, GameManager::getInstance().getNewEntityIndex());

		health = 100;

		score = 200;
		break;
	case EnemyType_Fast:

		shape.setFillColor(sf::Color::Yellow);
		speed = 5;
		stopFollowing = 300;
		currWeapon = new Weapon(Weapon_Sub, pos, false, *window, GameManager::getInstance().getNewEntityIndex());

		health = 75;

		score = 250;
		break;
	case EnemyType_Explosive:
		shape.setFillColor(sf::Color::White);
		speed = 1.5f;
		stopFollowing = 100000;

		health = 100;

		score = 300;

		break;
	case EnemyType_Tank:

		shape.setFillColor(sf::Color::Magenta);
		speed = 1.5f;
		stopFollowing = 150;
		currWeapon = new Weapon(Weapon_Shotgun, pos, false, *window, GameManager::getInstance().getNewEntityIndex());

		health = 150;

		score = 350;
		break;
	}
	if (type != EnemyType_Explosive) {
		currWeapon->isUsing = true;

	}
	//GameManager::getInstance().addItem(currWeapon);
}
