#include "Player.h"
Player::Player(sf::RenderWindow& window, sf::Vector2f position, unsigned int index) : Entity(window, index),GM(GameManager::getInstance()){
	this->velocity = sf::Vector2f(0.f, 0.f);
	this->movingSpeed = 20;//1000
	this->jumpingSpeed = 40; //2500
	this->shapeSize = sf::Vector2u(50, 50);
	this->shape = sf::RectangleShape(static_cast<sf::Vector2f>(shapeSize));
	this->shape.setPosition(position );
	this->shape.setFillColor(sf::Color::Blue);

	this->name = "Player";
	this->tag = "Player";

	Weapon* firstWeapon = new Weapon(Weapon_AK47, position, true,window, GM.getNewEntityIndex());
	firstWeapon->isUsing = true;
	firstWeapon->isInInventory = true;
	inventory = firstWeapon;
	GM.addItem(firstWeapon);
}
void Player::Update(float deltaTime) {
	currPos = shape.getPosition();
	//Get input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//velocity.x = -movingSpeed;
		movingLeft = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//velocity.x = movingSpeed;
		movingRight = true;
	}
	else {
		movingLeft = false;
		movingRight = false;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isJumping) {
		isJumping = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		inventory->Reload();
	}
	//Window barriers
	if (currPos.y + shapeSize.y > windowSize.y) {
		shape.setPosition(currPos.x, windowSize.y- shapeSize.y);
		isJumping = false;
		jumped = false;
	}
	if (Entity::isOutsideOfWindow(shape.getGlobalBounds())) {
		outsideOfWindow();
	}
	inventory->UpdateWeapon(deltaTime, static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)), sf::Mouse::isButtonPressed(sf::Mouse::Left));

	//inventory->RotateToMouse();

}
void Player::FixedUpdate() {
	inventory->SetPosition(currPos);

	if (movingLeft) {
		velocity.x = -movingSpeed;
	}
	else if (movingRight) {
		velocity.x = movingSpeed;
	}
	else {
		//if not moving slowly come to descent, friction
		if (velocity.x > 0) {
			velocity.x -= Physics::FRICTION();
			if (velocity.x < 0) {
				velocity.x = 0;
			}
		}
		else if (velocity.x < 0) {
			velocity.x += Physics::FRICTION();
			if (velocity.x > 0) {
				velocity.x = 0;
			}
		}

	}
	if (isJumping && !jumped) {
		velocity.y = -jumpingSpeed;
		jumped = true;
	}
	else if (isJumping) {
		velocity.y += Physics::GRAVITY();
	}

	shape.move(velocity);

	
}
void Player::Render() {
	window->draw(shape);
}

sf::FloatRect Player::getGlobalBounds() {
	return shape.getGlobalBounds();
}

void Player::outsideOfWindow() {
	//std::cout << "Player outside of window" << std::endl;
}

void Player::setInventoryItem(Weapon* item)
{
	inventory = item;
}

void Player::TakeDamage(int damage) {
	if (health - damage <= 0) {
		//Enemy dead 
		std::cout << "PLAYER IS DEAD" << std::endl;
		std::cout << "GAME OVER!!!" << std::endl;
		return;
	}
	health -= damage;
}
