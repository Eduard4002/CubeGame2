#pragma once

#include "IMGUI_SFML/imgui.h"
#include "IMGUI_SFML/imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <iostream>
#include "GameManager.h"
enum PanelType {
	PanelType_MainPanel,
	PanelType_GameOverPanel,
	PanelType_SettingPanel,
	PanelType_PausePanel,
	PanelType_MainMenuPanel
};
class UIManager
{
private:
	PanelType currPanel = PanelType_MainPanel;
	ImGuiWindowFlags defaultWindowFlags;
	bool isPaused = false;

	sf::RenderWindow* window;
	sf::Clock timer;

	sf::RectangleShape inventory1;
	sf::RectangleShape inventory2;
private:
	void mainPanel();
public:
	UIManager(sf::RenderWindow& window);
	void setPanel(PanelType type);

	void Update(float currDelta);
	void Render();

	void ShutDown();
};

