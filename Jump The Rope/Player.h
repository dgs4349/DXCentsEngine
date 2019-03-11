#pragma once

#include "Component.h"

class Player : public Component
{
public:
	Player();

	bool onGround = true;
	float accelleration;

	void Update(float deltaTime);
	void Jump(float jumpHeight);

protected:

	~Player();

private:
};

