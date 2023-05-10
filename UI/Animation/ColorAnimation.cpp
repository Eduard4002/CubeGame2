#include "ColorAnimation.h"
#include <iostream>
ColorAnimation::ColorAnimation(float duration, ImVec4 _startColor, ImVec4 _endColor) : UIAnimations(duration)
{
	startColor = _startColor;
	endColor = _endColor;
}

ImVec4 ColorAnimation::getCurrColor()
{
	ImVec4 currColor;
	float progress = this->progress();
	currColor.x = startColor.x + (endColor.x - startColor.x) * progress;
	currColor.y = startColor.y + (endColor.y - startColor.y) * progress;
	currColor.z = startColor.z + (endColor.z - startColor.z) * progress;
	currColor.w = startColor.w + (endColor.w - startColor.w) * progress;


	//std::cout<<currColor.x<<std::e
	return currColor;
}
