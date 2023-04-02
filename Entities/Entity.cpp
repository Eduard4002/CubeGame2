#include "Entity.h"
Entity::Entity(sf::RenderWindow& window, unsigned int index) {
	this->window = &window;
	this->index = index;

	windowSize = this->window->getSize();
	windowView = this->window->getView();

}
Entity::Entity() {

}
bool Entity::isOutsideOfWindow(sf::FloatRect shape) {
	sf::FloatRect windowRect = sf::FloatRect(windowView.getCenter() - windowView.getSize() / 2.f, windowView.getSize());
	if (!windowRect.intersects(shape)) {
		return true;
	}
	return false;
}
void Entity::outsideOfWindow() {

}
void Entity::Update(float deltaTime) {

}
void Entity::FixedUpdate() {

}
void Entity::Render() {
	std::cout << "Make sure to implement render function" << std::endl;
}
sf::FloatRect Entity::getGlobalBounds() {
	std::cout << "It is important to implement global bounds inside derived class" << std::endl;
	return sf::FloatRect();
}

void Entity::TakeDamage(int amount)
{
}
