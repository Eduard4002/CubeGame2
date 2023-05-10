#include <iostream>
#include <ctime>
#include "SFML/Graphics.hpp"
#include "GameManager.h"
#include "Physics.h"

#include "IMGUI_SFML/imgui.h"
#include "IMGUI_SFML/imgui-SFML.h"
int main() {
	std::time_t time = std::time(NULL);
	sf::Texture screenshotTexture;
	sf::Clock deltaClock;
	float currDelta = 0;
	//call Fixed update 50 times per second
	const float fixedUpdate = 0.02f;
	float fixedDelta = 0;
	int FPS = 0;
	float counterFPS = 0;

	bool useQuadTree = true;
	//912x512
	sf::RenderWindow window(sf::VideoMode(912, 512), "FPS: ", sf::Style::Close);
	window.setKeyRepeatEnabled(false);

	screenshotTexture.create(window.getSize().x, window.getSize().y);

	GameManager& gm = GameManager::getInstance();
    gm.init(&window);
	while (window.isOpen()) {
		currDelta = deltaClock.restart().asSeconds();
		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			gm.HandleEvent(evnt);	
			ImGui::SFML::ProcessEvent(window, evnt);
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
	
		if (fixedDelta >= Physics::FIXED_UPDATE_TIME()) {
			gm.FixedUpdate();
			fixedDelta = 0;

		}

		if (counterFPS >= 1.f) {
			std::string title = "FPS: " + std::to_string(FPS);
			window.setTitle(title);

			FPS = 0;
			counterFPS = 0;
		}
		gm.Update(currDelta);

		window.clear();
		gm.Render();
		window.display();
		FPS++;
	}
	gm.ShutDown();
}
