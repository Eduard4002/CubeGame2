#pragma once

#include "IMGUI_SFML/imgui.h"
#include "IMGUI_SFML/imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <iostream>
#include "../GameManager.h"

#include "UIAnimations.h"
#include "Animation/PositionAnimation.h"
#include "Animation/ColorAnimation.h"

enum PanelType {
	PanelType_MainPanel,
	PanelType_GameOverPanel,
	PanelType_SettingPanel,
	PanelType_PausePanel,
	PanelType_MainMenuPanel,
	PanelType_CreditsPanel
};
#include "UIAnimator.h"
class UIAnimator;

class UIManager
{
private:
	//Different font sizes
	ImFont* title;
	ImFont* button;
	ImFont* slider;

	float currDelta;
	ImGuiWindowFlags defaultWindowFlags;
	bool paused = false;

	sf::RenderWindow* window;
	sf::Clock timer;
	sf::Time survivedTime;
	bool calledGameOver;

	//Main Panel stuff
	sf::RectangleShape inventory1;
	sf::RectangleShape inventory2;

	sf::Texture tex1;
	sf::Texture tex2;

	int currPlayerHealth, maxPlayerHealth;

	short leftIconX, leftIconY;
	short previousLeftIconX, previousLeftIconY;

	short rightIconX, rightIconY;
	short previousRightIconX, previousRightIconY;

	int animationSpeed;
	ImVec2 buttonPos;

	UIAnimator* animator;

	ImVec4 healthBarRed;
	ImVec4 healthBarGreen;

	int currMusicVol = 100;
	int currSFXVol = 100;

	int prevMusicVol = 100;
	int prevSFXVol = 100;

	bool vSync = false;

	bool prevVSync;
public:
	PanelType currPanel = PanelType_MainMenuPanel;
	int highscore = 0;
private:
	void mainPanel();
	void pausePanel();
	void gameOverPanel();
	void settingsPanel();
	void mainMenuPanel();
	void creditsPanel();
	void HelpMarker(const char* desc);
public:
	UIManager(sf::RenderWindow& window);
	void setPanel(PanelType type);

	void Update(float currDelta);
	void Render();

	void ShutDown();

	void loadTexture(short iconX, short iconY, bool tex1);

	bool isPaused();

	float getPosY(short percent);
	float getPosX(short percent);

	void Restart();

};

