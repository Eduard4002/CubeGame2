#include "ParallaxEffect.h"
ParallaxEffect::ParallaxEffect(int layers) {
	maxLayers = layers;
}
void ParallaxEffect::addLayer(std::string fileName, float effect, sf::Vector2f playerPos)
{
	
	float tempEff = std::clamp(effect, 0.f, 1.f);

	sf::Texture temp;
	if (!temp.loadFromFile(fileName))
	{
		std::cout << "Not able to load from: " << fileName << std::endl;
	}
	temp.setRepeated(true);
	
	textures.push_back(temp);

	sf::Sprite temp2;

	if (isFirstLayer) {
		temp2.setTextureRect(sf::IntRect(0, 0, 3000, 512));
		temp2.setPosition(sf::Vector2f(-1500, 0));

		isFirstLayer = false;
	}
	else {
		temp2.setTextureRect(sf::IntRect(0, 0, 3000, 324));
		//										    Image height  Platform offset
		temp2.setPosition(sf::Vector2f(-1500, 512 - 324  -        (16 * 3)));

	}
	startPos = temp2.getPosition();
	sprites.push_back(temp2);

	effects.push_back(tempEff);
}

void ParallaxEffect::Update(sf::Vector2f playerPos)
{
	for (int i = 0; i < maxLayers; i++) {
		float newX = playerPos.x * effects[i];
		//sf::Vector2f startPos = sprites[i].getPosition();
		sprites[i].setPosition(sf::Vector2f(startPos.x + newX, sprites[i].getPosition().y));
	}
}

void ParallaxEffect::Render(sf::RenderWindow& window)
{
	for (int i = 0; i < maxLayers; i++) {

		sprites[i].setTexture(textures[i]);
		window.draw(sprites[i]);
	}
}
