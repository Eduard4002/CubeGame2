#pragma once
#include "UIAnimations.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
class UIAnimator
{
private:
	std::unordered_map<std::string, std::shared_ptr<UIAnimations>> animations;
public:
	void addAnimation(std::string ID, std::shared_ptr<UIAnimations> type);
	void Update(float deltaTime);
	void Restart();
	void Restart(std::string ID);
	std::shared_ptr<UIAnimations> getAnimation(std::string ID);

};

