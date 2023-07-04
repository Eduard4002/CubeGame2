#include "Player.h"
Player::Player(std::vector<sf::FloatRect> _platforms,sf::RenderWindow& window, sf::Vector2f position, unsigned int index) : Entity(window, index),GM(GameManager::getInstance()){
	this->velocity = sf::Vector2f(0.f, 0.f);
	this->movingSpeed = 15;
	this->jumpingSpeed = 40;
	this->shapeSize = sf::Vector2u(50, 50);
	this->shape = sf::RectangleShape(static_cast<sf::Vector2f>(shapeSize));
	this->shape.setPosition(position );
	this->shape.setFillColor(sf::Color::Blue);
	this->startPos = position;

	this->name = "Player";
	this->tag = "Player";
	//Default Weapon
	leftWeapon = new Weapon(Weapon_AK47, position, true, window, GM.getNewEntityIndex());
	usingWeapon = leftWeapon;
	defaultWeapon = leftWeapon;

	health = maxHealth;

	dashTime = 0.f;
	dashDuration = 0.1f;
	dashSpeed = 25;


	// Set up dash effects
	dashEffect1.setSize(shape.getSize());
	dashEffect1.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, 0));

	dashEffect2.setSize(shape.getSize());
	dashEffect2.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, 0));

	dashEffect3.setSize(shape.getSize());
	dashEffect3.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, 0));

	removingEffectSpeed = 0.95f;


	platforms = _platforms;
}
void Player::Update(float deltaTime) {
	currPos = shape.getPosition();
	//Get input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		movingLeft = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
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
		usingWeapon->Reload();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		usingWeapon = leftWeapon;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && rightWeapon != nullptr) {
		usingWeapon = rightWeapon;
	}

	// Check for dashing
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !isDashing && removedDashEffects) {
		isDashing = true;
		dashTime = 0.0f;
		if (movingLeft) {
			velocity.x = -dashSpeed;
		}
		else if (movingRight) {
			velocity.x = dashSpeed;
		}
	}
	/*
	//Window barriers
	if (currPos.y + shapeSize.y > platforms[0].top) {
		shape.setPosition(currPos.x, platforms[0].top- shapeSize.y);
		isJumping = false;
		jumped = false;
	}*/
	
	if (Entity::isOutsideOfWindow(shape.getGlobalBounds())) {
		outsideOfWindow();
	}
	usingWeapon->SetPosition(this->shape.getPosition());
	usingWeapon->UpdateWeapon(deltaTime, static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)), sf::Mouse::isButtonPressed(sf::Mouse::Left));
	//Right side
	if (currPos.x + shapeSize.x > windowSize.x) {
		shape.setPosition(windowSize.x - shapeSize.x, currPos.y);
		velocity.x = 0;
	}
	else if (currPos.x < 0) {
		shape.setPosition(0, currPos.y);
		velocity.x = 0;
	}

		
	if (!_isOnPlatform) {
		for (int i = 0; i < platforms.size(); i++) {
			if (Physics::isColliding(platforms[i], shape.getGlobalBounds())) {
				//std::cout << "player is colliding with plaform with index: " << i << std::endl;
				shape.setPosition(shape.getPosition().x, platforms[i].top - shape.getSize().y);
				_isOnPlatform = true;
				isJumping = false;
				jumped = false;
				velocity.y = 0;
			}

		}
	}

	
	
	
	
}
void Player::FixedUpdate() {
	usingWeapon->FixedUpdate();
	RemoveDashEffects();
	// Check if dashing
	if (isDashing) {
		if (showEffect && !spawnedDashEffects) {
			SpawnDashEffects();
			spawnedDashEffects = true;
			removedDashEffects = false;
		}
		
		dashTime += Physics::FIXED_UPDATE_TIME();
		if (dashTime >= dashDuration) {
			isDashing = false;
			dashTime = 0.0f;
		}
	}
	else {
		// Previous movement logic...
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
			_isOnPlatform = false;
		}
		/*
		if (velocity.y > 0) {
			velocity.y += Physics::GRAVITY();
			if (velocity.y < 0.f) {
				velocity.y = 0;
			}
		}*/


	}
	
	

	shape.move(velocity);

	
}
void Player::Render() {
	if (!removedDashEffects) {
		window->draw(dashEffect1);
		window->draw(dashEffect2);
		window->draw(dashEffect3);
	}


	window->draw(shape);
	usingWeapon->Render();
	

}

sf::FloatRect Player::getGlobalBounds() {
	return shape.getGlobalBounds();
}

void Player::SpawnDashEffects()
{
	// Position the dash effects at the player's current position
	sf::Vector2f pos = shape.getPosition();
	if (movingRight) {
		dashEffect1.setPosition(pos.x - 60, pos.y);
		dashEffect2.setPosition(pos.x - 120, pos.y);
		dashEffect3.setPosition(pos.x - 180, pos.y);
	}
	else {
		dashEffect1.setPosition(pos.x + 60, pos.y);
		dashEffect2.setPosition(pos.x + 120, pos.y);
		dashEffect3.setPosition(pos.x + 180, pos.y);
	}
	
	sf::Color playerColor = shape.getFillColor();
	dashEffect1.setFillColor(sf::Color(playerColor.r, playerColor.g, playerColor.b, 255  ));
	dashEffect2.setFillColor(sf::Color(playerColor.r, playerColor.g, playerColor.b, 255 / 2.f));
	dashEffect3.setFillColor(sf::Color(playerColor.r, playerColor.g, playerColor.b, 255 / 4.f));

}
void Player::RemoveDashEffects() {
	if (removedDashEffects) {
		return;
	}
	sf::Color tempColor = dashEffect1.getFillColor();
	dashEffect1.setFillColor(sf::Color(tempColor.r, tempColor.g, tempColor.b,tempColor.a *= removingEffectSpeed));
	if (tempColor.a < 1) {
		dashEffect1.setFillColor(sf::Color(tempColor.r, tempColor.g, tempColor.b, 0));
	}

	tempColor = dashEffect2.getFillColor();
	dashEffect2.setFillColor(sf::Color(tempColor.r, tempColor.g, tempColor.b, tempColor.a *= removingEffectSpeed));
	if (tempColor.a < 1) {
		dashEffect2.setFillColor(sf::Color(tempColor.r, tempColor.g, tempColor.b, 0));
	}

	tempColor = dashEffect3.getFillColor();
	dashEffect3.setFillColor(sf::Color(tempColor.r, tempColor.g, tempColor.b, tempColor.a *= removingEffectSpeed));
	if (tempColor.a < 1) {
		dashEffect3.setFillColor(sf::Color(tempColor.r, tempColor.g, tempColor.b, 0));
	}
	if (dashEffect1.getFillColor().a == 0 && dashEffect2.getFillColor().a == 0 && dashEffect3.getFillColor().a == 0) {
		spawnedDashEffects = false;
		removedDashEffects = true;
		return;
	}
}
void Player::outsideOfWindow() {
	//std::cout << "Player outside of window" << std::endl;
}

void Player::setInventoryItem(bool left,Weapon* item)
{
	if (left) {
		this->leftWeapon = item;
	}
	else {
		this->rightWeapon = item;
	}
}

void Player::TakeDamage(int damage) {
	if (health - damage <= 0) {
		//Player dead 
		if (showGameOver) {
			//GameManager::getInstance().GameOver();
			GameManager::getInstance().UI->setPanel(PanelType_GameOverPanel);
		}
		else {
			GameManager::getInstance().GameOver();
			std::cout << "PLAYER IS DEAD" << std::endl;
			std::cout << "GAME OVER!!!" << std::endl;
		}
		
		return;
	}
	health -= damage;
}

bool Player::holdingLeftWeapon()
{
	if (leftWeapon == usingWeapon) {
		return true;
	}
	return false;
}

int Player::isOnPlatform()
{
	for (int i = 0; i < platforms.size(); i++) {
		if (Physics::isColliding(platforms[i], shape.getGlobalBounds())) {
			//std::cout << "player is colliding with plaform with index: " << i << std::endl;

			shape.setPosition(shape.getPosition().x, platforms[i].top - shape.getSize().y);
			std::cout << shape.getPosition().y << std::endl;
			_isOnPlatform = true;
			isJumping = false;
			jumped = false;
			velocity.y = 0;
		}
	}
	return false;
}

void Player::Reset()
{
	shape.setPosition(startPos);
	leftWeapon = defaultWeapon;
	usingWeapon = leftWeapon;
	rightWeapon = nullptr;
	velocity.x = 0;
	velocity.y = 0;

	health = maxHealth;
}
