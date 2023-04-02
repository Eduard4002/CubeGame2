#pragma once
#include "../Item.h"
#include "Bullet.h"
#include "../GameManager.h"
#include "../Animation.h"
class GameManager;
class Animation;
enum WeaponType {
	Weapon_AK47 = 0,
	Weapon_M4 = 1,
	Weapon_Pistol = 2,
	Weapon_Shotgun = 3,
	Weapon_Sniper = 4,
	Weapon_Rocket = 5
};
class Weapon : public Item
{
private:
	GameManager& gm;
	sf::Vector2f weaponOffset;

	sf::Vector2f bulletSize;
	//time since the last shot was fired 
	float timeSinceLastShot;
	//Weapon characteristics
	float accuracy;
	float fireRate;
	float reloadTime;
	float damageAmount;
	int ammoAmount;

	bool isShooting;
	bool isReloading;

	//Is the player shooting from this weapon?
	bool playerUsing;

	int currentAmmo;

	//Animations
	Animation* animationClass;
private:
	void setCharacteristic(WeaponType type,int& animX, int& animY,int& shinningX,int& shinningY);
public:
	Weapon(WeaponType type,sf::Vector2f position, bool playerUsing,sf::RenderWindow& window, unsigned int index);
	//Weapon(std::string name, sf::Vector2f position,sf::Vector2f weaponOffset,std::string textureFileName, sf::RenderWindow& window, unsigned int index);
	Weapon();
	//Shoot the weapon towards a specific point
	void Shoot(sf::Vector2f rotateTo);

	void UpdateWeapon(float dt,sf::Vector2f pointTo,bool shouldShoot);
	void SetPosition(sf::Vector2f position);

	//Rotation
	void RotateToPoint(sf::Vector2f point);

	//Reload
	void Reload();
public:
	int getAmmoAmount();
};

