#include "UIAnimations.h"
#include <iostream>
UIAnimations::UIAnimations(float _duration)
{
	duration = _duration;
	elapsed = 0;
	finished = false; // Ensure that finished is initialized to false
}

void UIAnimations::Update(float deltaTime)
{
	if (finished) return;
	elapsed += deltaTime;

	if (elapsed >= duration) {
		elapsed = duration;
		finished = true;
	}
}

float UIAnimations::progress()
{
	float p = duration > 0 ? elapsed / duration : 1.f;
	return p;
}

bool UIAnimations::isFinished()
{
	return finished;
}

void UIAnimations::restart()
{
	elapsed = 0;
	finished = false;
}
