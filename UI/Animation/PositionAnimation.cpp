#include "PositionAnimation.h"
PositionAnimation::PositionAnimation(float _duration, ImVec2 _startPos, ImVec2 _endPos) : UIAnimations(_duration)
{
	startPos = _startPos;
	endPos = _endPos;
}

ImVec2 PositionAnimation::getCurrPos()
{
	ImVec2 currentPos;
	float progress = this->progress();
	currentPos.x = startPos.x + (endPos.x - startPos.x) * progress;
	currentPos.y = startPos.y + (endPos.y - startPos.y) * progress;

	return currentPos;
}
