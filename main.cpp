#include <iostream>
#include <ctime>
#include "SFML/Graphics.hpp"
#include "Entities/Player.h"
#include "Entities/Entity.h"
#include "Entities/Rectangle.h"
#include "Items/Item.h"
#include "GameManager.h"
#include "Animation.h"
#include "Quadtree.h"
int main() {
	std::time_t time = std::time(NULL);
	sf::Texture screenshotTexture;
	sf::Clock deltaClock;
	srand(time);
	float currDelta = 0;
	//call Fixed update 50 times per second
	const float fixedUpdate = 0.02f;
	float fixedDelta = 0;
	int FPS = 0;
	float counterFPS = 0;

	bool useQuadTree = false;
	sf::RenderWindow window(sf::VideoMode(912, 512), "FPS: ", sf::Style::Close);
	window.setKeyRepeatEnabled(false);

	screenshotTexture.create(window.getSize().x, window.getSize().y);

	GameManager& gm = GameManager::getInstance();
   //gm.init(window);

	Quadtree qd(sf::FloatRect(0, 0, 912, 512));

	

	std::vector<sf::CircleShape> circles;

	sf::CircleShape temp(2);
	
	for (int i = 0; i < 300; i++) {
		sf::CircleShape temp(4);
		temp.setPosition(sf::Vector2f(rand() % 912, rand() % 512));
		circles.push_back(temp);
	}

	while (window.isOpen()) {
		currDelta = deltaClock.restart().asSeconds();
		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			gm.HandleEvent(evnt);
			if (evnt.type == sf::Event::Closed) {
				window.close();
			}
			else if (evnt.type == sf::Event::KeyPressed) {
				if (evnt.key.code == sf::Keyboard::C) {
					std::cout << "Started taking screenshot" << std::endl;
					screenshotTexture.update(window);
					sf::Image screenshot = screenshotTexture.copyToImage();
					std::tm tm_local;
					errno_t err = localtime_s(&tm_local, &time);

					std::string fileName = "res/" + std::to_string(tm_local.tm_hour) + "_" + std::to_string(tm_local.tm_min) + "_" + std::to_string(tm_local.tm_sec) + ".png";
					std::cout << tm_local.tm_hour << ":" << tm_local.tm_min << ":" << tm_local.tm_sec << std::endl;
					//std::cout << "res/" << std::asctime(std::localtime(&time)) << std::endl;
					screenshot.saveToFile(fileName);
				}
			}
		}

		counterFPS += currDelta;
		fixedDelta += currDelta;
	
		if (fixedDelta >= fixedUpdate) {
			//gm.FixedUpdate();
			fixedDelta = 0;

		}

		if (counterFPS >= 1.f) {
			std::string title = "FPS: " + std::to_string(FPS);
			window.setTitle(title);

			FPS = 0;
			counterFPS = 0;
		}
		
		gm.Update(currDelta);
		//Update position
		qd.clear();
		for (int i = 0; i < circles.size(); i++) {
			sf::Vector2f pos = sf::Vector2f(((float)(rand()) / (float)(RAND_MAX)) - 0.5f, ((float)(rand()) / (float)(RAND_MAX)) - 0.5f);
			circles[i].move(pos);
			circles[i].setFillColor(sf::Color::White);
			Point* temp = new Point("Circle", circles[i].getGlobalBounds(), i);
			qd.insert(temp);
		}
		if (useQuadTree) {
			//Quad tree collision detection <- Very kul :)
			for (int i = 0; i < circles.size(); i++) {
				std::vector<Point*> points = qd.queryRange(circles[i].getGlobalBounds());
				for (int j = 0; j < points.size(); j++) {
					if (i != points[j]->index && circles[i].getGlobalBounds().contains(sf::Vector2(points[j]->boundary.left,points[j]->boundary.top))) {
						circles[i].setFillColor(sf::Color::Green);
						circles[points[j]->index].setFillColor(sf::Color::Green);
					}
				}
			}
		}
		else {
			//Normal collision detection <- Very bad :(
			for (int i = 0; i < circles.size(); i++) {
				for (int j = 0; j < circles.size(); j++) {
					if (circles[i].getGlobalBounds() != circles[j].getGlobalBounds() && circles[i].getGlobalBounds().intersects(circles[j].getGlobalBounds())) {
						circles[i].setFillColor(sf::Color::Green);
					}
				}
			}
		}
		window.clear();
		
		
		//Draw circles
		for (int i = 0; i < circles.size(); i++) {
			window.draw(circles[i]);
		}
		if (useQuadTree) {
			qd.show(window);
		}

		//gm.Render();
		window.display();
		FPS++;
	}
}
