#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include <iostream>
struct Layer {
	sf::Sprite sprite;
	sf::Texture texture;
	float effect;
};
class ParallaxEffect
{
private:
	int maxLayers;

	std::vector<Layer> layers;

	std::vector<sf::Sprite> sprites;
	std::vector<sf::Texture> textures;

	
	std::vector<float> effects;

	int currLayer = 0;

	bool isFirstLayer = true;

	sf::Vector2f startPos;
	
public:
	ParallaxEffect(int layers);

	void addLayer(std::string fileName,float effect, sf::Vector2f playerPos);
	void Update(sf::Vector2f playerPos);
	void Render(sf::RenderWindow& window);
};

