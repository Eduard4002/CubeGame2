#pragma once
#include "../UIAnimations.h"
class PositionAnimation : public UIAnimations
{
private:
	ImVec2 startPos;
	ImVec2 endPos;
public:
	PositionAnimation(float _duration, ImVec2 _startPos, ImVec2 _endPos);

	ImVec2 getCurrPos();
};

