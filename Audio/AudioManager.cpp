#include "AudioManager.h"

bool AudioManager::addSFX(std::string name, std::string fileName)
{
	//Check if sfx with the same name already exists
	for (int i = 0; i < sfxArr.size(); i++) {
		if (sfxArr[i].name == name) {
			return false;
		}
	}

	SFX temp;
	temp.name = name;
	if (!temp.buff.loadFromFile(fileName)) {
		return false;
	}

	sfxArr.push_back(temp);
}

void AudioManager::playSFX(std::string name)
{
	//Check if sfx with the same name already exists
	for (int i = 0; i < sfxArr.size(); i++) {
		if (sfxArr[i].name == name) {
			sound.setBuffer(sfxArr[i].buff);
			sound.play();
		}
	}
}

void AudioManager::setSFXVolume(int val)
{
	int newVal = std::clamp(val, 0, 100);
	sound.setVolume(newVal);
}

bool AudioManager::addMusic(std::string name, std::string fileName)
{
	//Check if sfx with the same name already exists
	for (int i = 0; i < bgArr.size(); i++) {
		if (bgArr[i].name == name) {
			return false;
		}
	}

	BackgroundMusic temp;
	temp.name = name;
	temp.fileName = fileName;

	bgArr.push_back(temp);
}

void AudioManager::playMusic(std::string name)
{
	for (int i = 0; i < bgArr.size(); i++) {
		if (bgArr[i].name == name) {
			if (!music.openFromFile(bgArr[i].fileName))
				return; // error
			//music.setPlayingOffset(sf::seconds(90.f));
			music.play();
		}
	}
}

void AudioManager::setMusicVolume(int val)
{
	int newVal = std::clamp(val, 0, 100);
	music.setVolume(newVal);
}

void AudioManager::Update()
{
	sf::SoundSource::Status status = music.getStatus();
	//Stopped, music is finished
	if (status == 0) {
		if (bgArr.size() == 1) {
			playMusic(bgArr[0].name);
			currMusicIndex = 0;
			std::cout << "Looped the song" << std::endl;
		}
		else {
			if (currMusicIndex + 1 >= bgArr.size()) {
				playMusic(bgArr[0].name);
				currMusicIndex = 0;
			}
			else {
				currMusicIndex++;
				playMusic(bgArr[currMusicIndex].name);
			}
		}
	}
}
