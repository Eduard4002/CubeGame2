#include "Enemy.h"



Enemy::Enemy(EnemyType type, Player* player, sf::Vector2f position, sf::RenderWindow& window, unsigned int index) : Entity(window,index)
{
	shape.setPosition(position);
	playerEntity = player;
	tag = "Enemy";
	
	setCharacteristics(type, position);
	stopFollowingShape.setSize({ stopFollowing, stopFollowing * 6});
	stopFollowingShape.setOutlineColor(sf::Color::Green);
	stopFollowingShape.setOutlineThickness(2);

	stopFollowingShape.setOrigin(sf::Vector2f( stopFollowing / 2 - shape.getSize().x / 2, stopFollowing - shape.getSize().y / 2));
	stopFollowingShape.setPosition(position);

	stopFollowingShape.setFillColor(sf::Color::Transparent);
	
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(2);
}

void Enemy::FixedUpdate()
{
	startShoot = false;
	//Move towards player
	playerPos = sf::Vector2f(playerEntity->getGlobalBounds().left, playerEntity->getGlobalBounds().top);
	velocity = Physics::normalize(playerPos - shape.getPosition());
	
	
	velocity.y += Physics::GRAVITY();
	if (this->shape.getGlobalBounds().top + this->shape.getGlobalBounds().height > windowSize.y) {
		velocity.y = 0;
	}
	/*
	
	else {
		direction.y = 0.f;
	}*/
	
	//Follow the player only if we are outside range
	if (stopFollowingShape.getGlobalBounds().contains(playerPos)) {
		//shape.move(velocity * speed);
		velocity.x = 0;
		
	}
	else {
		//Start shooting 
		startShoot = true;
	}
	shape.move(velocity * speed);
	currWeapon->SetPosition(shape.getPosition());
	stopFollowingShape.move(velocity * speed);
}

void Enemy::Update(float dt)
{
	
	currWeapon->RotateToPoint(playerPos);
	currWeapon->UpdateWeapon(dt,playerPos,startShoot);
	if (currWeapon->getAmmoAmount() == 0) {
		currWeapon->Reload();
	}
}

void Enemy::Render()
{
	window->draw(shape);
	//window->draw(stopFollowingShape);
	
}

sf::FloatRect Enemy::getGlobalBounds()
{
	return shape.getGlobalBounds();
}

void Enemy::TakeDamage(int amount)
{

	if (health - amount <= 0) {
		//Enemy dead 
		GameManager::getInstance().RemoveEnemy(this->index,this->currWeapon->index);
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
	shape.setSize(sf::Vector2f(50, 50));
	switch (type) {
	case EnemyType_Easy:
		shape.setFillColor(sf::Color::Green);
		speed = 3;
		stopFollowing = 300;
		currWeapon = new Weapon(Weapon_Pistol, pos, false, *window, GameManager::getInstance().getNewEntityIndex());
		health = 50;


	case EnemyType_Normal:
		
		shape.setFillColor(sf::Color::Cyan);
		speed = 3;
		stopFollowing = 400;
		currWeapon = new Weapon(Weapon_AK47, pos, false, *window, GameManager::getInstance().getNewEntityIndex());

		health = 100;
		break;
	}

	currWeapon->isUsing = true;
	GameManager::getInstance().addItem(currWeapon);
}
