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

	ImGui::SFML::UpdateFontTexture();

	inventory1.setSize(sf::Vector2f(64, 64));
	inventory1.setFillColor(sf::Color::Red);
	inventory1.setOutlineColor(sf::Color::Black);
	inventory1.setOutlineThickness(5);
	inventory1.setPosition(sf::Vector2f(20, 440));

	inventory2.setSize(sf::Vector2f(64, 64));
	inventory2.setFillColor(sf::Color::Blue);
	inventory2.setOutlineColor(sf::Color::Black);
	inventory2.setOutlineThickness(5);
	inventory2.setPosition(sf::Vector2f(20 + 64, 440));

}
void UIManager::Update(float currDelta) {
	sf::Time time(sf::seconds(currDelta));

	ImGui::SFML::Update(*this->window, time);
	setPanel(currPanel);

	//If ESC is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !isPaused) {
		setPanel(PanelType_PausePanel);
	}
}
void UIManager::Render() {
	this->window->draw(inventory1);
	this->window->draw(inventory2);
	ImGui::SFML::Render(*this->window);

	

}
void UIManager::ShutDown()
{
	ImGui::SFML::Shutdown();
}

void UIManager::mainPanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.f));

	ImGui::Begin("Temp",NULL,defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0,0));

	int currPlayerHealth, maxPlayerHealth;
	short leftIconX, leftIconY;
	short rightIconX, rightIconY;
	GameManager::getInstance().getPlayerHealth(currPlayerHealth, maxPlayerHealth);
	GameManager::getInstance().getPlayerLeftInventory(leftIconX, leftIconY);
	GameManager::getInstance().getPlayerRightInventory(rightIconX, rightIconY);
	std::string healthText = std::to_string(currPlayerHealth) + "/" + std::to_string(maxPlayerHealth);
	ImGui::SetCursorPos(ImVec2(20, 375));
	ImGui::Text(healthText.c_str());
	sf::Texture* texture = new sf::Texture();
	if (!texture->loadFromFile("res/Icons/GunHalf.png",sf::IntRect(leftIconX * 32, leftIconY * 32,32,32))) {
		std::cout << "Error loading spritesheet with file path: " << "res/Icons/AK-47.png" << std::endl;
	}
	ImGui::SetCursorPos(ImVec2(20, 440));
	ImGui::Image(*texture,sf::Vector2f(64,64));
	ImGui::End();

	ImGui::PopStyleColor();
}



void UIManager::setPanel(PanelType type) {
	switch (type) {
	case PanelType_MainPanel:
		mainPanel();
		break;
	}
}