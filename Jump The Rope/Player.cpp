#include "Player.h"
#include "Transform.h"


Player::Player() : Component("Player")
{
}


void Player::Update(float deltaTime)
{
	DirectX::XMFLOAT3 pos = transform->Position();
	pos.y += accelleration * deltaTime;
	transform->Position(pos);
}

void Player::Jump(float jumpHeight)
{
	if (onGround)
	{
		accelleration += jumpHeight;

		onGround = false;
	}
}

Player::~Player()
{
}
