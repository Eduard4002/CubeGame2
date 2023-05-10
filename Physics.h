#pragma once
#include "SFML/Graphics.hpp"
class Physics
{
private:
	static constexpr float gravity = 4.f;
	static constexpr float friction = 1.f;
	static constexpr float PI = 3.14159265;
	//Divide by 60 because that is how many frames per second we want to call it
	static constexpr float fixedUpdateTime = 1.f / 60;
public:
	//Getters
	static float GRAVITY() {
		return gravity;
	}
	static float FRICTION() {
		return friction;
	}
	static float FIXED_UPDATE_TIME() {
		return fixedUpdateTime;
	}

public:
	//Collision detection
	 static bool isColliding(sf::FloatRect obj1, sf::FloatRect obj2);

	 //Return a angle to look at mouse
	 static float lookAtMouse(sf::RenderWindow& win, sf::Vector2f curPos);

	 //Return a angle to look at a specific point
	 static float lookAtPoint(sf::Vector2f currPos, sf::Vector2f lookTo);

	 static sf::Vector2f normalize(sf::Vector2f position);
};

