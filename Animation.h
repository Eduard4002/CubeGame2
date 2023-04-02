#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <dos.h>
#include <Windows.h>
#include <functional>

class Animation
{
private:
    sf::RectangleShape& shape;
    std::vector<sf::Texture*> textureSheets;
    std::vector<sf::IntRect> frames;
    std::vector<int> frameCounts;
    std::vector<float> frameDurations;

    sf::IntRect defaultFrame;
    int indexDefaultFrame;
    int currentAnimation;
    int currentFrame;
    float elapsedTime;
    int frameWidth;
    int frameHeight;

    bool isPlaying;

    int startingFrame;
    int endFrame;
public:
    Animation(sf::RectangleShape& shape, int width, int height);
    ~Animation();

    void addAnimation(std::string filepath, int startX, int startY, int frames, float frameDuration, int skipX = 0,bool containsDefaultFrame = false);
    void playAnimation(int index, float deltaTime);
    bool isAnimationPlaying(int index);
    bool isAnimationFinished(int index);

    void setDefaultFrame();

};

