#include "Physics.h"

bool Physics::isColliding(sf::FloatRect obj1, sf::FloatRect obj2) {
	if (obj1.intersects(obj2)) {
		return true;
	}
	return false;
}

float Physics::lookAtMouse(sf::RenderWindow& win, sf::Vector2f curPos)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(win);
	float dx = curPos.x - mousePosition.x;
	float dy = curPos.y - mousePosition.y;

	float rotation = (atan2(dy, dx)) * 180 / PI;

	return rotation + 180;
}

float Physics::lookAtPoint(sf::Vector2f curPos, sf::Vector2f lookTo)
{
	float dx = curPos.x - lookTo.x;
	float dy = curPos.y - lookTo.y;

	float rotation = (atan2(dy, dx)) * 180 / PI;

	return rotation + 180;
}
sf::Vector2f Physics::normalize(sf::Vector2f position) {
	return position / std::sqrt(position.x * position.x + position.y * position.y);
}
