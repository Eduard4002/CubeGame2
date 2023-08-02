#include "Animation.h"

Animation::Animation(sf::RectangleShape& shape, int width, int height) : shape(shape){
    this->frameWidth = width;
    this->frameHeight = height;
    currentAnimation = -1;
    currentFrame = 0;
    elapsedTime = 0.f;
    isPlaying = false;
    shape.setScale(sf::Vector2f(1.5, 1.5));
}

Animation::~Animation()
{
    
    for (auto texture : textureSheets)
    {
        delete texture;
    }
}

void Animation::addAnimation(std::string filepath,int startX, int startY, int frames, float frameDuration, int skipX, bool containsDefaultFrame )
{
	//Load textureSheet
	std::string fullPath = "res/Animation/" + filepath;
    sf::Texture* texture = new sf::Texture();
    if (!texture->loadFromFile(fullPath)) {
        std::cout << "Error loading spritesheet with file path: " << fullPath << std::endl;
    }
    texture->setSmooth(false);
    textureSheets.push_back(texture);
    if (containsDefaultFrame) {
        defaultFrame = sf::IntRect(0, startY * frameHeight, frameWidth, frameHeight);
        indexDefaultFrame = 0;
    }
    // Add the frames
    for (int i = 0; i < frames; i++)
    {
        this->frames.push_back(sf::IntRect(startX + i * (frameWidth + skipX), startY * frameHeight, frameWidth, frameHeight));
    }
    this->frameCounts.push_back(frames);
    this->frameDurations.push_back(frameDuration);
}


void Animation::setDefaultFrame()
{
    
   shape.setTexture(textureSheets[0]);
   shape.setTextureRect(defaultFrame);
    
}


void Animation::playAnimation(int index, float deltaTime)
{
    if (index > frameCounts.size()) {
        std::cout << "Invalid index" << std::endl;
    }
    if (index != currentAnimation)
    {
        currentAnimation = index;
        startingFrame = 0;
        for (int i = 0; i < index; i++)
        {
            startingFrame += frameCounts[i];
        }
        currentFrame = startingFrame;
        endFrame = startingFrame + frameCounts[index];

        isPlaying = true;

        shape.setTexture(textureSheets[currentAnimation]);

    }

    elapsedTime += deltaTime;
    if (elapsedTime >= frameDurations[currentAnimation])
    {
        elapsedTime -= frameDurations[currentAnimation];
        currentFrame++;

        if (currentFrame >= endFrame)
        {
            currentFrame = startingFrame;
            isPlaying = false; // Set the flag to false when the animation is completed
        }
    }
    shape.setTextureRect(frames[currentFrame]);

}

bool Animation::isAnimationPlaying(int index)
{
    return currentAnimation == index && isPlaying;
}
bool Animation::isAnimationFinished(int index)
{
    if (index != currentAnimation)
    {
        return true; // If the animation is not currently playing, it's considered finished
    }
    else
    {
        // Check if the current animation has completed playing
        int endFrame = 0;
        if (index == 0) {
            endFrame = frameCounts[0];
        }
        else {
            for (int i = 0; i <= index; ++i)
            {
                endFrame += frameCounts[i];
            }
        }
        
        return currentFrame == (endFrame - 1);
    }
}

