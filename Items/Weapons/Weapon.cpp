#include "Weapon.h"
/*
Weapon::Weapon(std::string name, sf::Vector2f spawnPosition,  sf::Vector2f weaponOffset, std::string textureFileName ,sf::RenderWindow& window, unsigned int index) :
	Item(name, spawnPosition, textureFileName, window, index), gm(GameManager::getInstance()){
	this->weaponOffset = weaponOffset;
}*/
Weapon::Weapon(WeaponType type, sf::Vector2f position, bool playerUsing,sf::RenderWindow& window, unsigned int index) :Item(position, window, index),gm(GameManager::getInstance())
{
	this->playerUsing = playerUsing;
	
	setCharacteristic(type);

	currentAmmo = ammoAmount;
	this->shape.setOrigin(shape.getSize().x * 0.5f, shape.getSize().y * 0.5f);
	/*
	if (type == Weapon_Rocket || type == Weapon_Sniper) {
		this->shape.setSize(sf::Vector2f(48 * 1.5f, 32));
		animationClass = new Animation(this->shape, 48, 32);
		//Shoot animation
		animationClass->addAnimation(shootAnimation, animX, animY, 4, 0.25f, 32);
		//Reload animation
		animationClass->addAnimation(reloadAnimation, animX, animY, 8, reloadTime / 8, 32);
		if (type == Weapon_Rocket) {
			//Dropped animation
			animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * shinningX, shinningY, 5, 0.2f, 48 * 3 + 32 * 2);
		}
		else {
			//Dropped animation
			animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * shinningX, shinningY, 5, 0.2f, 48 + 32 * 5);
		}
		

	}
	else {
		animationClass = new Animation(this->shape, 32, 32);
		//Shoot animation
		animationClass->addAnimation(shootAnimation, animX, animY, 4, 0.25f, 32);
		//Reload animation
		animationClass->addAnimation(reloadAnimation, animX, animY, 8, reloadTime / 8, 32);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * shinningX, shinningY, 5, 0.2f, 32 * 7);
	}
	*/

	//Play animation for very short time, just to get the first frame
	animationClass->playAnimation(1, 0.00000001f);
	audio = new AudioManager();
}
Weapon::Weapon() : Item(std::string("undefined"), sf::Vector2f(0,0), std::string("undefined")),gm(GameManager::getInstance()){

}

void Weapon::UpdateWeapon(float dt,sf::Vector2f pointTo,bool shouldShoot) {

	if (isShooting) {
		animationClass->playAnimation(0, dt);
		if (animationClass->isAnimationFinished(0)) {
			isShooting = false;
			//Play animation for very short time, just to get the first frame
			animationClass->playAnimation(1, 0.00000001f);
		}
	}else if (isReloading) {
		animationClass->playAnimation(1, dt);
		//if animation finished, set currentAmmo to max ammo amount
		if (animationClass->isAnimationFinished(1)) {
			isReloading = false;
			currentAmmo = ammoAmount;
		}
	}/*
	else if (isDropped) {
		animationClass->playAnimation(2, dt);
		
	}*/
	

	timeSinceLastShot += dt;
	
	if (shouldShoot && currentAmmo > 0 && timeSinceLastShot >= fireRate) {
		Shoot(pointTo);
		isShooting = true;

		timeSinceLastShot = 0;
	}

	RotateToPoint(pointTo);
}
void Weapon::SetPosition(sf::Vector2f position) {
	
		//std::cout << entityReference.x << std::endl;
	pickableZone.setPosition(position + weaponOffset);
	shape.setPosition(position + weaponOffset);
	
}
void Weapon::FixedUpdate() {
	if (!isDropped) return;
	//Platform height - 48
	sf::Vector2f position = shape.getPosition();
	
	if (goingUp) {
		shape.move(0, -Physics::GRAVITY() * 0.1f);

		if ((int)position.y == (int)(window->getSize().y) - 90) {
			goingUp = false;
		}
	}
	else {
		shape.move(0, Physics::GRAVITY() * 0.1f);

		if ((int)position.y == (int)(window->getSize().y - 70)) {
			goingUp = true;
		}
	}
	
}
void Weapon::Reset() {
	shape.setRotation(0);
	currentAmmo = ammoAmount;
	//Play animation for very short time, just to get the first frame
	animationClass->playAnimation(1, 0.00000001f);
}

void Weapon::Shoot(sf::Vector2f rotateTo) {
	audio->playSFX(shootSFX);
	//shootAnimation->playAnimation(texture, isShooting);
	//float rotation = Physics::lookAtMouse(*window, sf::Vector2f(shape.getGlobalBounds().left, shape.getGlobalBounds().top));
	float rotation = Physics::lookAtPoint(sf::Vector2f(shape.getGlobalBounds().left, shape.getGlobalBounds().top), rotateTo);
	sf::FloatRect gb = shape.getGlobalBounds();

	// Calculate the center point of the unrotated weapon shape
	sf::Vector2f center(gb.left + gb.width * 0.5f, gb.top + gb.height * 0.5f);

	// Use the rotation of the weapon shape to rotate the spawn position vector
	float rotation2 = shape.getRotation();
	sf::Transform rotationTransform;
	rotationTransform.rotate(rotation2, center);

	// Calculate the spawn position based on the rotated weapon shape
	sf::Vector2f spawnPosition(gb.left + gb.width - bulletSize.x, center.y + bulletSize.y * 0.5f);
	spawnPosition = rotationTransform.transformPoint(spawnPosition);

	sf::Vector2f direction = rotateTo - spawnPosition;
	sf::Vector2f normalized = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
	//Spawn new bullet
	Bullet* temp = new Bullet(damageAmount,bulletSize, spawnPosition, rotation, normalized,7.5f,playerUsing, *window, gm.getNewEntityIndex());
	temp->name = "NormalBullet";
	temp->tag = "Bullet";
	gm.addBullet(temp);
	//shootAnimation->reset();
	currentAmmo--;
	
}



void Weapon::RotateToPoint( sf::Vector2f point) {
	shape.setRotation(Physics::lookAtPoint(shape.getPosition(), point));
}

void Weapon::Reload()
{
	isReloading = true;
	audio->playSFX(reloadSFX);
}


void Weapon::setCharacteristic(WeaponType type)
{
	weaponOffset = sf::Vector2f(40, 0);
	bulletSize = sf::Vector2f(32, 32);

	switch (type) {
	case Weapon_AK47:
		name = "AK-47";
		fireRate = 0.2f;
		reloadTime = 3.f;
		ammoAmount = 30;

		damageAmount = 10;

		iconX = 7;
		iconY = 0;

		shootSFX = "res/Audio/SFX/Weapon/556 Single Isolated WAV.wav";
		reloadSFX = "res/Audio/SFX/Weapon/AK Reload Full WAV.wav";
		
		animationClass = new Animation(this->shape, 32, 32);
		//Shoot animation
		animationClass->addAnimation("SMG/SMGsAnimations.png", 0, 7, 4, 0.25f, 32);
		//Reload animation
		animationClass->addAnimation("SMG/ReloadingSMGs.png", 0, 7, 8, reloadTime / 8, 32);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * 7, 0, 5, 0.2f, 32 * 7);

		break;
	case Weapon_M4: 
		name = "M4";
		reloadTime = 2.f;
		fireRate = 0.175f;
		ammoAmount = 25;

		damageAmount = 10;

		iconX = 0;
		iconY = 0;

		shootSFX = "res/Audio/SFX/Weapon/762x39 Single Isolated WAV.wav";
		reloadSFX = "res/Audio/SFX/Weapon/AR Reload Full WAV.wav";


		animationClass = new Animation(this->shape, 32, 32);
		//Shoot animation
		animationClass->addAnimation("SMG/SMGsAnimations.png", 0, 0, 4, 0.25f, 32);
		//Reload animation
		animationClass->addAnimation("SMG/ReloadingSMGs.png", 0, 0, 8, reloadTime / 8, 32);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 0, 0, 5, 0.2f, 32 * 7);
		break;
	case Weapon_Pistol:
		name = "Pistol";
		reloadTime = 2.f;
		ammoAmount = 5;
		fireRate = 0.5f;
		damageAmount = 5;

		iconX = 4;
		iconY = 1;

		shootSFX = "res/Audio/SFX/Weapon/9mm Single Isolated.wav";
		reloadSFX = "res/Audio/SFX/Weapon/9mm Pistol Reload 2.wav";

		animationClass = new Animation(this->shape, 32, 32);
		//Shoot animation
		animationClass->addAnimation("Pistol/PistolsAnimation.png", 0, 1, 4, 0.25f, 32);
		//Reload animation
		animationClass->addAnimation("Pistol/ReloadingPistols.png", 0, 1, 10, reloadTime / 10);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 32*4, 1, 5, 0.2f, 32 * 4);
		break;
	case Weapon_Shotgun:
		name = "Shotgun";
		reloadTime = 3.5f;
		ammoAmount = 1;
		fireRate = 1.f;
		damageAmount = 50;

		iconX = 3;
		iconY = 3;

		shootSFX = "res/Audio/SFX/Weapon/762x54r Single Isolated WAV.wav";
		reloadSFX = "res/Audio/SFX/Weapon/Pump Reload Full WAV.wav";


		animationClass = new Animation(this->shape, 32, 32);
		//Shoot animation
		animationClass->addAnimation("Shotgun/ShotgunOnlyAnimations.png", 0, 1, 5, 0.2f, 32);
		//Reload animation
		animationClass->addAnimation("Shotgun/ReloadingShotguns.png", 0, 1, 8, reloadTime / 12);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * 2, 2, 5, 0.2f, 32 * 4);
		break;
	case Weapon_Sub:
		name = "Submachine";
		reloadTime = 1.5f;
		ammoAmount = 20;
		fireRate = 0.1f;
		damageAmount = 5;
		iconX = 7;
		iconY = 1;

		shootSFX = "res/Audio/SFX/Weapon/22LR Spray Isolated WAV.wav";
		reloadSFX = "res/Audio/SFX/Weapon/Semi 22LR Reload Part 2 WAV.wav";

		animationClass = new Animation(this->shape, 32, 32);
		//Shoot animation
		animationClass->addAnimation("Pistol/PistolsAnimation.png", 0, 4, 4, 0.25f, 32);
		//Reload animation
		animationClass->addAnimation("Pistol/ReloadingPistols.png", 0, 4, 10, reloadTime / 10);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * 4, 1, 5, 0.2f, 32 * 4);
		break;
	case Weapon_Sniper:

		reloadTime = 3.5f;
		ammoAmount = 1;
		fireRate = 1.f;

		name = "Sniper";

		this->shape.setSize(sf::Vector2f(40 * 1.5f, 32));
		animationClass = new Animation(this->shape, 40, 32);
		//Shoot animation
		animationClass->addAnimation("Other/OtherGunsAnimations.png", 0, 0, 4, 0.25f, 40);
		//Reload animation
		animationClass->addAnimation("Other/ReloadingOtherGuns.png", 0, 0, 8, reloadTime / 8, 24);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * 5, 2, 5, 0.2f, 48 + 32 * 5);
		break;
	case Weapon_Rocket:
		name = "Rocket";
		//shinningY = 3;
		reloadTime = 3.5f;
		ammoAmount = 1;
		fireRate = 1.f;

		this->shape.setSize(sf::Vector2f(64, 32));
		animationClass = new Animation(this->shape, 64, 32);
		//Shoot animation
		animationClass->addAnimation("Other/OtherGunsAnimations.png", 0, 2, 4, 0.25f);
		//Reload animation
		animationClass->addAnimation("Other/ReloadingOtherGuns.png", 0, 2, 7, reloadTime / 7);
		//Dropped animation
		animationClass->addAnimation("GunsShiningSpriteSheet.png", 32 * 0, 3, 5, 0.2f, 48 * 3 + 32 * 2);
		break;
	}
}
int Weapon::getAmmoAmount()
{
	return currentAmmo;
}

void Weapon::Render()
{
	window->draw(this->shape);
	//window->draw(this->pickableZone);
}

void Weapon::ResetAmmoAmount()
{
	currentAmmo = ammoAmount;
}
