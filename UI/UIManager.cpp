#include "UIManager.h"

void UIManager::HelpMarker(const char* desc)
{
	ImGui::PushFont(slider);

	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
	ImGui::PopFont();
}
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
	slider = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/Font/ChakraPetch-Medium.ttf", 32);
	pickup = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/Font/ChakraPetch-Medium.ttf", 16);





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
	
	animator = new UIAnimator();
	// Create a PositionAnimation,values are for testing
	std::shared_ptr<PositionAnimation> pauseResumePos = std::make_shared<PositionAnimation>(1.f, ImVec2(0, 200), ImVec2(window.getSize().x / 2.f, 200));
	animator->addAnimation("Pause/Resume", pauseResumePos, PanelType_PausePanel);

	std::shared_ptr<PositionAnimation> pauseSettingsPos = std::make_shared<PositionAnimation>(1.5f, ImVec2(0, 300), ImVec2(window.getSize().x / 2.f, 300));
	animator->addAnimation("Pause/Settings", pauseSettingsPos, PanelType_PausePanel);

	std::shared_ptr<PositionAnimation> pauseQuitPos = std::make_shared<PositionAnimation>(2.f, ImVec2(0, 400), ImVec2(window.getSize().x / 2.f, 400));
	animator->addAnimation("Pause/Quit", pauseQuitPos, PanelType_PausePanel);

	/*
	std::shared_ptr<ColorAnimation> testColAnimation = std::make_shared<ColorAnimation>(2, ImVec4(1.f,1.f,1.f,0.f), ImVec4(1.f,1.f,1.f,1.f));
	animator.addAnimation("Color", testColAnimation);*/
	/*
	titleAudio = new AudioManager();
	titleAudio->addMusic("Main", "res/Audio/Music/SkyFire (Title Screen) (1).ogg");
	titleAudio->playMusic("Main");

	gameOverAudio = new AudioManager();
	gameOverAudio->addMusic("Main", "res/Audio/Music/Defeated (Game Over Tune).ogg");*/
	
}
void UIManager::Update(float currDelta) {
	this->currDelta = currDelta;
	sf::Time time(sf::seconds(currDelta));

	ImGui::SFML::Update(*this->window, time);
	setPanel(currPanel);

	//ImGui::ShowDemoWindow();

	//If ESC is pressed
	if (currPanel != PanelType_MainMenuPanel && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !paused) {
		currPanel = PanelType_PausePanel;
	}
	//As the background we can have the parrallax effect moving with the mouse
	if (currPanel == PanelType_MainMenuPanel || currPanel == PanelType_CreditsPanel || currPanel == PanelType_ChangeBackground) {
		GameManager::getInstance().background->Update(sf::Vector2f(sf::Mouse::getPosition()));
		
	}

	//testAnimation.Update(currDelta);
}
void UIManager::Render() {
	if (!paused ) {
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
	if (!texture->loadFromFile("res/Icons/Guns.png", sf::IntRect(iconX * 32, iconY * 32, 32, 32))) {
		std::cout << "Error loading spritesheet with file path: " << "res/Icons/Guns.png" << std::endl;
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
	float healthPercent = (float)currPlayerHealth / (float)maxPlayerHealth;
	bool holdingLeft =  GameManager::getInstance().getPlayerLeftInventory(leftIconX, leftIconY,leftAmmoAmount);
	bool holdingRight = GameManager::getInstance().getPlayerRightInventory(rightIconX, rightIconY,rightAmmoAmount);
	//sf::Vector2f(20, 440)

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
	ImGui::PushFont(pickup);
	ImGui::SetCursorPos(ImVec2(22,440));
	ImGui::Text(std::to_string(leftAmmoAmount).c_str());
	if (rightAmmoAmount != -1) {
		ImGui::SetCursorPos(ImVec2(102, 440));
		ImGui::Text(std::to_string(rightAmmoAmount).c_str());
	}
	

	ImGui::PopFont();
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
	ImGui::Button(" ", ImVec2(200 * healthPercent, 50));

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
	if (currBackgroundIndex == 7) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));
	}
	std::string scoreText = "SCORE: " + std::to_string(GameManager::getInstance().getCurrentScore());
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - ImGui::CalcTextSize(scoreText.c_str()).x / 2 , 0));
	ImGui::Text(scoreText.c_str());

	if (currBackgroundIndex == 7) {
		ImGui::PopStyleColor();
	}
	//Pickup weapon text
	if (showPickup) {
		ImGui::SetCursorPos(ImVec2(playerPos.x + 50,playerPos.y - 20));
		std::string pickupText = "Press F to pickup " + weaponName;
		ImGui::PushFont(pickup);
		ImGui::Text(pickupText.c_str());
		ImGui::PopFont();

	}
	ImGui::End();
	ImGui::PopStyleColor();

}

void UIManager::pausePanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));

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
	std::shared_ptr<PositionAnimation> position = std::static_pointer_cast<PositionAnimation>(animator->getAnimation("Pause/Resume"));

	//ImGui::SetCursorPos(ImVec2(position->getCurrPos().x - 250 / 2, position->getCurrPos().y));
	//Resume button
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2, 150));
	if (ImGui::Button("RESUME", ImVec2(250, 75))) {
		currPanel = PanelType_MainPanel;
	}

	// Access the PositionAnimation by its unique identifier
	position = std::static_pointer_cast<PositionAnimation>(animator->getAnimation("Pause/Settings"));
	//Settings button
	//ImGui::SetCursorPos(ImVec2(position->getCurrPos().x - 250 / 2, position->getCurrPos().y));

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2 , 250));

	if (ImGui::Button("SETTINGS", ImVec2(250, 75))) {
		setPanel(PanelType_SettingPanel);
	}
	// Access the PositionAnimation by its unique identifier
	position = std::static_pointer_cast<PositionAnimation>(animator->getAnimation("Pause/Quit"));
	//Quit button
	//ImGui::SetCursorPos(ImVec2(position->getCurrPos().x - 250 / 2, position->getCurrPos().y));

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 300 / 2, 350));

	if (ImGui::Button("MAIN MENU", ImVec2(300, 75))) {
		//setPanel(PanelType_MainMenuPanel);
		GameManager::getInstance().quitGame();
	}

	animator->Update(currPanel,currDelta);

	ImGui::PopFont();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();

}
void UIManager::mainMenuPanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.0f));

	ImGui::Begin("Temp", NULL, defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0, 0));

	ImGui::PushFont(title);
	ImVec2 txtSize = ImGui::CalcTextSize("Blocky Brawl");
	if (currBackgroundIndex == 7) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));
	}
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, 20));
	ImGui::Text("Blocky Brawl");
	ImGui::PopFont();
	if (currBackgroundIndex == 7) {
		ImGui::PopStyleColor();
	}
	ImGui::PushFont(button);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4((float)76 / 255, (float)86 / 255, (float)106 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4((float)59 / 255, (float)66 / 255, (float)82 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);


	//Resume button
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2, 150));
	if (ImGui::Button("START", ImVec2(250, 75))) {
		
		GameManager::getInstance().startGame();
	}
	//Settings button
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2, 250));

	if (ImGui::Button("SETTINGS", ImVec2(250, 75))) {
		setPanel(PanelType_SettingPanel);
	}
	
	//Quit button

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2, 350));

	if (ImGui::Button("QUIT", ImVec2(250, 75))) {
		ShutDown();
		window->close();
	}
	ImGui::PopFont();

	ImGui::SetCursorPos(ImVec2(window->getSize().x - 200, 405));

	if (ImGui::Button("CREDITS", ImVec2(175, 60))) {
		setPanel(PanelType_CreditsPanel);
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();

}
void UIManager::creditsPanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.f));

	ImGui::Begin("Temp", NULL, defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0, 0));
	if (currBackgroundIndex == 7 || currBackgroundIndex == 5) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));
	}
	ImGui::PushFont(title);
	ImVec2 txtSize = ImGui::CalcTextSize("CREDITS");

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, 20));
	ImGui::Text("CREDITS");
	ImGui::PopFont();
	
	ImGui::PushFont(button);

	

	txtSize = ImGui::CalcTextSize("DEVELOPED BY");
	ImGui::SetCursorPos(ImVec2(80, 120));
	ImGui::Text("DEVELOPED BY");
	ImGui::SetCursorPos(ImVec2(80, 170));
	ImGui::Text("EDUARD");

	ImGui::SetCursorPos(ImVec2(130 + txtSize.x , 120));
	ImGui::Text("BETA TESTERS");


	ImGui::SetCursorPos(ImVec2(130 + txtSize.x, 170));
	ImGui::Text("-Kajzera");
	ImGui::SetCursorPos(ImVec2(130 + txtSize.x, 220));
	ImGui::Text("-FKF");
	ImGui::PushFont(slider);

	ImGui::SetCursorPos(ImVec2(80, 250));
	ImGui::Text("GUN PACK: DotStudio[itch.io]");

	ImGui::SetCursorPos(ImVec2(80, 300));
	ImGui::Text("SFX: SnakeF8 Part1&Part2[itch.io]");

	if (currBackgroundIndex == 7 || currBackgroundIndex == 5) {
		ImGui::PopStyleColor();
	}
	ImGui::PopFont();
	//Back button
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 250 / 2, 425));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4((float)76 / 255, (float)86 / 255, (float)106 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4((float)59 / 255, (float)66 / 255, (float)82 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	if (ImGui::Button("BACK", ImVec2(250, 75))) {
		setPanel(PanelType_MainMenuPanel);
	}
	ImGui::PopFont();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::End();
	ImGui::PopStyleColor();
}
void UIManager::changeBackgroundPanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.f));

	ImGui::Begin("Temp", NULL, defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0, 0));

	ImGui::PushFont(title);
	ImVec2 txtSize = ImGui::CalcTextSize("BACKGROUND");
	if (currBackgroundIndex == 7) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));
	}
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, 20));
	ImGui::Text("BACKGROUND");
	ImGui::PopFont();
	if (currBackgroundIndex == 7) {
		ImGui::PopStyleColor();
	}
	ImGui::SetCursorPos(ImVec2(window->getSize().x - 200, getPosY(50)));

	ImGui::PushFont(button);
	if (ImGui::Button("NEXT", ImVec2(200, 75))) {
		currBackgroundIndex++;
		if (currBackgroundIndex > 8) {
			currBackgroundIndex = 1;
		}
		GameManager::getInstance().setBackground(currBackgroundIndex);
	}

	ImGui::SetCursorPos(ImVec2(0, getPosY(50)));

	if (ImGui::Button("PREV", ImVec2(200, 75))) {
		currBackgroundIndex--;

		if (currBackgroundIndex < 1) {
			currBackgroundIndex = 8;
		}
		GameManager::getInstance().setBackground(currBackgroundIndex);

	}

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 125, getPosY(80)));
	if (ImGui::Button("BACK", ImVec2(250, 75))) {
		GameManager::getInstance().saveBackground(currBackgroundIndex);
		setPanel(PanelType_SettingPanel);
	}
	ImGui::PopFont();
	ImGui::End();
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

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, 150));

	int seconds = survivedTime.asSeconds();
	if (seconds >= 60 && seconds < 120) {
		std::string timeText = "You failed in: " + std::to_string((int(seconds / 60) % 60)) + " minute & " + std::to_string((int(seconds % 60))) + " seconds ";
		ImGui::Text(timeText.c_str());
	}
	else if (seconds >= 120) {
		std::string timeText = "You failed in: " + std::to_string((int(seconds / 60) % 60)) + " minutes & " + std::to_string((int(seconds % 60))) + " seconds ";
		ImGui::Text(timeText.c_str());
	}
	else {
		std::string timeText = "You failed in: " + std::to_string((int(seconds % 60))) + " seconds ";
		ImGui::Text(timeText.c_str());
	}

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, 190));

	std::string scoreTxt = "SCORE: " + std::to_string(score) + " ";
	ImGui::Text(scoreTxt.c_str());

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, 230));

	std::string highscoreText = "HIGHSCORE: " + std::to_string(highscore) + " ";
	ImGui::Text(highscoreText.c_str());

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
		GameManager::getInstance().quitGame();
	}
	ImGui::PopFont();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::End();
	ImGui::PopStyleColor();
}
void UIManager::settingsPanel()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));

	ImGui::Begin("Temp", NULL, defaultWindowFlags);
	ImGui::SetWindowSize("Temp", ImVec2(window->getSize().x, window->getSize().y));
	ImGui::SetWindowPos("Temp", ImVec2(0, 0));

	//Start Title
	ImGui::PushFont(title);
	ImVec2 txtSize = ImGui::CalcTextSize("SETTINGS");
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - txtSize.x / 2, getPosY(5)));
	ImGui::Text("SETTINGS");
	ImGui::PopFont();
	//End title
	//Start Music/SFX

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
	/*
	ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4((float)135 / 255, (float)135 / 255, (float)135 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4((float)64 / 255, (float)65 / 255, (float)66 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ImVec4((float)39 / 255, (float)39 / 255, (float)41 / 255, 1));*/

	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4((float)76 / 255, (float)86 / 255, (float)106 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4((float)59 / 255, (float)66 / 255, (float)82 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4((float)76 / 255, (float)86 / 255, (float)106 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4((float)59 / 255, (float)66 / 255, (float)82 / 255, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4((float)46 / 255, (float)52 / 255, (float)64 / 255, 1));

	float width = ImGui::GetWindowWidth() * 0.3f;
	ImGui::PushItemWidth(width);

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - width / 2 , getPosY(30)));
	ImGui::DragInt("MUSIC", &currMusicVol, 0.2f,0,100);
	ImGui::SameLine(); HelpMarker(
		"Click and drag to edit value.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");
	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - width / 2, getPosY(45)));

	ImGui::DragInt("SFX", &currSFXVol, 0.2f, 0, 100);
	ImGui::SameLine(); HelpMarker(
		"Click and drag to edit value.\n"
		"Hold SHIFT/ALT for faster/slower edit.\n"
		"Double-click or CTRL+click to input value.");

	

	//End Music/SFX
	//Start Checkbox
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4((float)39 / 255, (float)39 / 255, (float)41 / 255, 1));
	
	ImGui::PushItemWidth(width);

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - width / 2 + 50, getPosY(60)));


	ImGui::Checkbox("V-Sync", &vSync);

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 - 125, getPosY(80)));
	if (ImGui::Button("BACK", ImVec2(250, 75))) {
		if (GameManager::getInstance().onMainMenu) {
			setPanel(PanelType_MainMenuPanel);
		}
		else {
			setPanel(PanelType_PausePanel);
		}
	}

	ImGui::SetCursorPos(ImVec2(window->getSize().x / 2 + 200 , getPosY(80)));

	if (ImGui::Button("BACKGROUND", ImVec2(250, 75))) {
		setPanel(PanelType_ChangeBackground);
	}

	//Change respective values
	if (prevMusicVol != currMusicVol) {
		GameManager::getInstance().setMusicVolume(currMusicVol);
		prevMusicVol = currMusicVol;
	}
	if (prevSFXVol != currSFXVol) {
		GameManager::getInstance().setSFXVolume(currSFXVol);
		prevSFXVol = currSFXVol;
	}
	if (prevVSync != vSync) {
		GameManager::getInstance().setVSync(vSync);
		prevVSync = vSync;
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::End();
	ImGui::PopStyleColor();
}

bool UIManager::isPaused() {
	return paused;
}
void UIManager::setPaused(bool paused)
{
	this->paused = paused;
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
		//animator.Restart("Pause/Resume");
		//animator.Restart("Pause/Settings");
		//animator.Restart("Pause/Quit");

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
	case PanelType_SettingPanel:
		paused = true;
		settingsPanel();
		break;
	case PanelType_MainMenuPanel:
		paused = true;
		mainMenuPanel();
		break;
	case PanelType_CreditsPanel:
		paused = true;
		creditsPanel();
		break;
	case PanelType_ChangeBackground:
		paused = true;
		changeBackgroundPanel();
		break;
	}
}
void UIManager::Restart()
{
	timer.restart();
	calledGameOver = false;
}