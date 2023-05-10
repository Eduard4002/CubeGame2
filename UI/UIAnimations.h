#pragma once
#include "IMGUI_SFML/imgui.h"
#include "IMGUI_SFML/imgui-SFML.h"
class UIAnimations
{
private:
	float duration;
	float elapsed;
	bool finished;
public:
	UIAnimations(float duration);

	void Update(float deltaTime);
	float progress();
	bool isFinished();
	void restart();
};

