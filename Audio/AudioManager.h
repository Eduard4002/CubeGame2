#pragma once
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
struct SFX {
	std::string name;
	sf::SoundBuffer buff;
};
struct BackgroundMusic {
	std::string name;
	std::string fileName;
};
class AudioManager
{
private:
	//SFX array
	std::vector<SFX> sfxArr;
	//Music array
	std::vector<BackgroundMusic> bgArr;

	sf::Sound sound;
	sf::Music music;

	int currMusicIndex;

public:
	bool addSFX(std::string name, std::string fileName);
	void playSFX(std::string name);
	void setSFXVolume(int val);

	bool addMusic(std::string name, std::string fileName);
	void playMusic(std::string name);
	void setMusicVolume(int val);

	void Update();
};

