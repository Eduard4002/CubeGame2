#include "UIManager.h"
UIManager::UIManager(sf::RenderWindow& window)
{
	this->window = &window;
	
	defaultWindowFlags = 
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize;
	ImGui::SFML::Init(window, false);
	ImGui::GetIO().Fonts->AddFontFromFileTTF("res/Font/ChakraPetch-Medium.ttf", 48);
	title = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/Font/ChakraPetch-Medium.ttf", 96);
	button = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/Font/ChakraPetch-Medium.ttf", 64);



	ImGui::SFML::UpdateFontTexture();

	inventory1.setSize(sf::Vector2f(55, 55));
	inventory1.setFillColor(sf::Color(76,86,106));
	inventory1.setOutlineColor(sf::Color(46,52,64));
	inventory1.setOutlineThickness(3);
	inventory1.setPosition(sf::Vector2f(20, 440));

	inventory2.setSize(inventory1.getSize());
	inventory2.setFillColor(inventory1.getFillColor());
	inventory2.setOutlineColor(inventory1.getOutlineColor());
	inventory2.setOutlineThickness(inventory1.getOutlineThickness());
	inventory2.setPosition(sf::Vector2f(inventory1.getPosition().x + 80,
									    inventory1.getPosition().y));

	healthBarRed = ImVec4(1, 0, 0, 1);
	healthBarGreen = ImVec4(0, 1, 0, 1);
	previousRightIconX = -1;
	previousLeftIconX = -1;

	timer.restart();
	
	animationSpeed = 60;
	

	// Create a PositionAnimation,values are for testing
	std::shared_ptr<PositionAnimation> pauseResumePos = std::make_shared<PositionAnimation>(1.f, ImVec2(0, 200), ImVec2(window.getSize().x / 2.f, 200));
	animator.addAnimation("Pause/Resume", pauseResumePos);

	std::shared_ptr<PositionAnimation> pauseSettingsPos = std::make_shared<PositionAnimation>(1.5f, ImVec2(0, 300), ImVec2(window.getSize().x / 2.f, 300));
	animator.addAnimation("Pause/Settings", pauseSettingsPos);

	std::shared_ptr<PositionAnimation> pauseQuitPos = std::make_shared<PositionAnimation>(2.f, ImVec2(0, 400), ImVec2(window.getSize().x / 2.f, 400));
	animator.addAnimation("Pause/Quit", pauseQuitPos);

	std::shared_ptr<ColorAnimation> testColAnimation = std::make_shared<ColorAnimation>(2, ImVec4(1.f,1.f,1.f,0.f), ImVec4(1.f,1.f,1.f,1.f));
	animator.addAnimation("Color", testColAnimation);

}
void UIManager::Update(float currDelta) {
	this->currDelta = currDelta;
	sf::Time time(sf::seconds(currDelta));

	ImGui::SFML::Update(*this->window, time);
	setPanel(currPanel);

	//If ESC is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !paused) {
		currPanel = PanelType_PausePanel;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
		currPanel = PanelType_GameOverPanel;
	}
	
	//testAnimation.Update(currDelta);
}
void UIManager::Render() {
	if (!paused) {
		this->window->draw(inventory1);
		this->window->draw(inventory2);
	}
	ImGui::SFML::Render(*this->window);
}

void UIManager::ShutDown()
{
	ImGui::SFML::Shutdown();
}

void UIManager::loadTexture(short iconX, short iconY, bool isTex1)
{
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("res/Icons/GunHalf.png", sf::IntRect(iconX * 32, iconY * 32, 32, 32))) {
		std::cout << "Error loading spritesheet with file path: " << "res/Icons/GunHalf.png" << std::endl;
	}
	if (isTex1) {
		tex1 = *texture;
	}
	else {
		tex2 = *texture;
	}

}

void UIManager::mainPanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.f));
	

	ImGui::Begin("Temp",NULL,defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0,0));

	GameManager::getInstance().getPlayerHealth(currPlayerHealth, maxPlayerHealth);
	bool holdingLeft =  GameManager::getInstance().getPlayerLeftInventory(leftIconX, leftIconY);
	bool holdingRight = GameManager::getInstance().getPlayerRightInventory(rightIconX, rightIconY);

	if (holdingLeft) {
		inventory1.setOutlineColor(sf::Color::White);
	}
	else {
		inventory1.setOutlineColor(sf::Color(46, 52, 64));
	}
	if (holdingRight) {
		inventory2.setOutlineColor(sf::Color::White);
	}
	else {
		inventory2.setOutlineColor(sf::Color(46, 52, 64));
	}
	if (previousLeftIconX != leftIconX) {
		loadTexture(leftIconX, leftIconY, true);
		previousLeftIconX = leftIconX;
	}
	if (rightIconX != -1 && previousRightIconX != rightIconX) {
		loadTexture(rightIconX, rightIconY, false);
		previousRightIconX = rightIconX;
	}
	ImGui::SetCursorPos(ImVec2(15, 375));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	ImGui::PushStyleColor(ImGuiCol_Button, healthBarRed);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, healthBarRed);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, healthBarRed);

	ImGui::Button(" ", ImVec2(200, 50));

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();


	ImGui::PushStyleColor(ImGuiCol_Button, healthBarGreen);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, healthBarGreen);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, healthBarGreen);

	ImGui::SetCursorPos(ImVec2(15, 375));
	ImGui::Button(" ", ImVec2(currPlayerHealth, 50));

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	
	ImGui::PopStyleVar();

	std::string healthText = std::to_string(currPlayerHealth) + "/" + std::to_string(maxPlayerHealth);
	
	ImGui::SetCursorPos(ImVec2(100 - ImGui::CalcTextSize(healthText.c_str()).x / 2 +15, 375));
	ImGui::Text(healthText.c_str());
	//Left inventory
	ImGui::SetCursorPos(ImVec2(15, 435));
	ImGui::Image(tex1,sf::Vector2f(64,64));

	//Right inventory
	if (rightIconX != -1) {
		ImGui::SetCursorPos(ImVec2(95, 435));
		ImGui::Image(tex2, sf::Vector2f(64, 64));
	}

	ImGui::End();
	ImGui::PopStyleColor();

}

void UIManager::pausePanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));
	std::shared_ptr<ColorAnimation> color = std::static_pointer_cast<ColorAnimation>(animator.getAnimation("Color"));
	ImGui::PushStyleColor(ImGuiCol_Text, color->getCurrColor());

	ImGui::Begin("Temp", NULL, defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0, 0));

	ImGui::PushFont(title);
	ImVec2 txtSize = ImGui::CalcTextSize("PAUSE PANEL");
	
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x /2, 20));
	ImGui::Text("PAUSE PANEL");
	ImGui::PopFont();
	
	ImGui::PushFont(button);

	ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4((float)76 / 255, (float)86 / 255, (float)106 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4((float)59 / 255, (float)66 / 255, (float)82 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	// Access the PositionAnimation by its unique identifier
	std::shared_ptr<PositionAnimation> position = std::static_pointer_cast<PositionAnimation>(animator.getAnimation("Pause/Resume"));

	ImGui::SetCursorPos(ImVec2(position->getCurrPos().x - 250 / 2, position->getCurrPos().y));
	//Resume button
	//ImGui::SetCursorPos(ImVec2(window->getSize().x - 250 / 2, 200));
	if (ImGui::Button("RESUME", ImVec2(250, 75))) {
		currPanel = PanelType_MainPanel;
	}

	// Access the PositionAnimation by its unique identifier
	position = std::static_pointer_cast<PositionAnimation>(animator.getAnimation("Pause/Settings"));
	//Settings button
	ImGui::SetCursorPos(ImVec2(position->getCurrPos().x - 250 / 2, position->getCurrPos().y));
	if (ImGui::Button("SETTINGS", ImVec2(250, 75))) {
		std::cout << "Settings clicked" << std::endl;
	}
	// Access the PositionAnimation by its unique identifier
	position = std::static_pointer_cast<PositionAnimation>(animator.getAnimation("Pause/Quit"));
	//Quit button
	ImGui::SetCursorPos(ImVec2(position->getCurrPos().x - 250 / 2, position->getCurrPos().y));
	if (ImGui::Button("QUIT", ImVec2(250, 75))) {
		std::cout << "Quit clicked" << std::endl;
	}

	animator.Update(currDelta);

	ImGui::PopFont();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

}

void UIManager::gameOverPanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 1.f));

	ImGui::Begin("Temp", NULL, defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0, 0));


	ImGui::PushFont(title);
	ImVec2 txtSize = ImGui::CalcTextSize("GAME OVER");
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, getPosY(5)));
	ImGui::Text("GAME OVER");
	ImGui::PopFont();
	if (!calledGameOver) {
		survivedTime = timer.restart();
		calledGameOver = true;
	}
	std::string timeText = "You failed in: " + std::to_string((int)floor(survivedTime.asSeconds())) + " seconds...";

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, 200));
	ImGui::Text(timeText.c_str());

	ImGui::PushFont(button);

	ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4((float)76 / 255, (float)86 / 255, (float)106 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4((float)59 / 255, (float)66 / 255, (float)82 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);

	//Restart button
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2, 300));
	if (ImGui::Button("RESTART", ImVec2(250, 75))) {
		GameManager::getInstance().RestartGame();
	}
	//Quit button
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2, 400));
	if (ImGui::Button("QUIT", ImVec2(250, 75))) {
		std::cout << "Quit clicked" << std::endl;
	}
	ImGui::PopFont();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::End();
	ImGui::PopStyleColor();
}
bool UIManager::isPaused() {
	return paused;
}
float UIManager::getPosY(short percent)
{
	int perc = std::clamp((int)percent, 0, 100);
	return (perc / 100.f) * window->getSize().y;
}

float UIManager::getPosX(short percent)
{
	int perc = std::clamp((int)percent, 0, 100);
	return (perc / 100.f) * window->getSize().x;
}
void UIManager::setPanel(PanelType type) {
	currPanel = type;
	switch (type) {
	case PanelType_MainPanel:
		paused = false;
		animator.Restart("Pause/Resume");
		animator.Restart("Pause/Settings");
		animator.Restart("Pause/Quit");

		mainPanel();
		break;
	case PanelType_PausePanel:
		paused = true;
		pausePanel();
		break;
	case PanelType_GameOverPanel:
		paused = true;
		gameOverPanel();
		break;
	}
}
void UIManager::Restart()
{
	timer.restart();
	calledGameOver = false;
}