#include "Bullet.h"

Bullet::Bullet(int damageAmount, sf::Vector2f size, sf::Vector2f position, float angle, sf::Vector2f startVelocity,float speed, bool playerShoot, sf::RenderWindow& window, unsigned int index) :Entity(window, index) {
	this->shape.setSize(size);
	this->shape.setPosition(position);
	this->shape.rotate(angle - 90);
	this->velocity = startVelocity;
	this->speed = speed;
	this->damageAmount = damageAmount;
	this->playerShoot = playerShoot;
	if (!texture.loadFromFile("res/Icons/Bullet/row-1-column-2.png")) {
		std::cout << "Error loading from file" << std::endl;
	}
	this->shape.setTexture(&texture);
	//this->shape.setOutlineColor(sf::Color::Blue);
	//this->shape.setOutlineThickness(2);
}
void Bullet::Update(float dt) {

}

void Bullet::FixedUpdate() {
	this->shape.move(velocity * speed);

}
void Bullet::Render() {
	this->window->draw(shape);
}
sf::FloatRect Bullet::getGlobalBounds() {
	return this->shape.getGlobalBounds();
}