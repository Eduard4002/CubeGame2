#include "Item.h"
Item::Item(std::string name, sf::Vector2f position, std::string textureFileName, sf::RenderWindow& window, unsigned int index) : Entity(window, index){
	sf::Vector2f shapeSize = sf::Vector2f(200, 200);

	this->pickableZone.setSize({ shapeSize.x * 2.f,shapeSize.y * 2.f });
	this->pickableZone.setOrigin(sf::Vector2f(shapeSize.x * 0.5f,shapeSize.y * 0.5f));
	this->pickableZone.setPosition(position);
	this->pickableZone.setOutlineColor(sf::Color::Green);
	this->pickableZone.setOutlineThickness(2);
	this->pickableZone.setFillColor(sf::Color::Transparent);

	this->shape.setSize(shapeSize);
	this->shape.setOrigin(shape.getSize().x * 0.5f, shape.getSize().y * 0.5f);

	this->shape.setPosition(position);

	this->shape.setOrigin(sf::Vector2f(shapeSize.x * 0.5f, shapeSize.y * 0.5f));
	this->shape.setFillColor(sf::Color::Red);

	this->name = name;
	this->tag = "Item";
}
Item::Item(std::string name, sf::Vector2f position, std::string textureFileName ) : Entity(){
}
Item::Item() : Entity(){}
Item::Item(sf::Vector2f position, sf::RenderWindow& window, unsigned int index) : Entity(window, index){
	sf::Vector2f shapeSize = sf::Vector2f(32 * 1.5f, 32 * 1.5f);

	this->pickableZone.setSize({ shapeSize.x * 2.f,shapeSize.y * 2.f });
	this->pickableZone.setOrigin(sf::Vector2f(shapeSize.x, shapeSize.y));
	this->pickableZone.setPosition(position);
	this->pickableZone.setOutlineColor(sf::Color::Green);
	this->pickableZone.setOutlineThickness(2);
	this->pickableZone.setFillColor(sf::Color::Transparent);


	this->shape.setPosition(position);

	this->shape.setSize(shapeSize);
	//this->shape.setOutlineColor(sf::Color::Red);
	//this->shape.setOutlineThickness(1);

	if (this->window == nullptr) {
		std::cout << "Window is nullptr" << std::endl;
	}
}
void Item::FixedUpdate() {

}
void Item::Update(float dt) {

}
void Item::Use() {

}
void Item::Render() {
	if (window != nullptr) {
		//If in inventory, dont draw the pickable zone
		if (!isUsing) {
			//this->window->draw(pickableZone);
		}
		//this->shape.setTexture(sprite.getTexture());
		this->window->draw(shape);
	}
	else {
		std::cout << "Window is a null pointer" << std::endl;
	}
}

sf::FloatRect Item::getGlobalBounds() {
	return shape.getGlobalBounds();
}
sf::FloatRect Item::getPickableZoneBounds() {
	return pickableZone.getGlobalBounds();
}

void Item::FollowPlayer(sf::Vector2f playerPos) {

}

