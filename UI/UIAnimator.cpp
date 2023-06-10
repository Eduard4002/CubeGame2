#include "UIAnimator.h"

void UIAnimator::addAnimation(std::string ID,std::shared_ptr<UIAnimations> anim, PanelType panel)
{
	if (panelsAnim.size() != 0) {
		//Loop through all panelAnimation structs
		for (int i = 0; i < panelsAnim.size(); i++) {
			if (panelsAnim[i].panelType == panel) {
				panelsAnim[i].animations[ID] = anim;
			}
		}
	}
	else {
		PanelAnimation temp;
		temp.panelType = panel;
		temp.animations[ID] = anim;
		panelsAnim.push_back(temp);
	}
	
}

void UIAnimator::Update(PanelType currPanel, float deltaTime)
{
	for (const auto& list : panelsAnim[currPanelIndex].animations) {
		list.second->Update(deltaTime);
	}
}

void UIAnimator::Restart(PanelType type)
{
	for (int i = 0; i < panelsAnim.size(); i++) {
		if (panelsAnim[i].panelType = type) {
			for (const auto& list : panelsAnim[i].animations) {
				list.second->restart();
			}
		}
	}
	
}

void UIAnimator::Restart(std::string ID)
{
	std::shared_ptr<UIAnimations> animation = getAnimation(ID);
	if (animation != nullptr) {
		animation->restart();
	}
}

std::shared_ptr<UIAnimations> UIAnimator::getAnimation(std::string ID)
{
	for (int i = 0; i < panelsAnim.size(); i++) {
		auto it = panelsAnim[i].animations.find(ID);
		if (it != panelsAnim[i].animations.end()) {
			return it->second;
		}
	}
   return nullptr;
    

}

