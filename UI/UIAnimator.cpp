#include "UIAnimator.h"

void UIAnimator::addAnimation(std::string ID,std::shared_ptr<UIAnimations> anim)
{
	animations[ID] = anim;
}

void UIAnimator::Update(float deltaTime)
{
	for (const auto& list : animations) {
		list.second->Update(deltaTime);
	}
}

void UIAnimator::Restart()
{
	for (const auto& list : animations) {
		list.second->restart();
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
   auto it = animations.find(ID);
   if (it != animations.end()) {
      return it->second;
   }
   return nullptr;
    

}

