#pragma once
#include "../UIAnimations.h"
class ColorAnimation : public UIAnimations
{
private:
	ImVec4 startColor;
	ImVec4 endColor;
public:
	ColorAnimation(float duration, ImVec4 startColor,ImVec4 endColor);

	ImVec4 getCurrColor();
};

