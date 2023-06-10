#pragma once
#include "UIManager.h"

#include "UIAnimations.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

struct PanelAnimation{
	PanelType panelType;
	std::unordered_map<std::string, std::shared_ptr<UIAnimations>> animations;
};
class UIAnimator
{
private:
	short currPanelIndex;
	std::vector<PanelAnimation> panelsAnim;
	
public:
	void addAnimation(std::string ID, std::shared_ptr<UIAnimations> type, PanelType panel);
	void Update(PanelType currPanel, float deltaTime);
	void Restart(PanelType type);
	void Restart(std::string ID);
	std::shared_ptr<UIAnimations> getAnimation(std::string ID);

};

