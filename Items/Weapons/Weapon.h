#pragma once
#include "../Item.h"
#include "Bullet.h"
#include "../GameManager.h"
#include "../Animation.h"
#include "../Audio/AudioManager.h"
class GameManager;
class Animation;
enum WeaponType {
	Weapon_AK47 = 0,
	Weapon_M4 = 1,
	Weapon_Pistol = 2,
	Weapon_Shotgun = 3,
	Weapon_Sub = 4,
	Weapon_Sniper = 5,
	Weapon_Rocket = 6
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
	float damageAmount = 10;
	int ammoAmount;

	std::string shootSFX;
	std::string reloadSFX;

	bool isShooting;
	bool isReloading;


	int currentAmmo;

	//Animations
	Animation* animationClass;

	//dropped weapon animation variables
	bool goingUp = false;
public:
	int iconX;
	int iconY;

	//Is the player shooting from this weapon?
	bool playerUsing;

	AudioManager* audio;

private:
	void setCharacteristic(WeaponType type);
public:
	Weapon(WeaponType type,sf::Vector2f position, bool playerUsing,sf::RenderWindow& window, unsigned int index);
	//Weapon(std::string name, sf::Vector2f position,sf::Vector2f weaponOffset,std::string textureFileName, sf::RenderWindow& window, unsigned int index);
	Weapon();
	//Shoot the weapon towards a specific point
	void Shoot(sf::Vector2f rotateTo);

	void UpdateWeapon(float dt,sf::Vector2f pointTo,bool shouldShoot);
	void SetPosition(sf::Vector2f position);
	void FixedUpdate();

	//Rotation
	void RotateToPoint(sf::Vector2f point);

	/*Resets the rotation and goes back to the first frame*/
	void Reset();

	//Reload
	void Reload();

	void Render();

	void ResetAmmoAmount();
public:
	int getAmmoAmount();
};

