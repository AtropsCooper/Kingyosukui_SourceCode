#include "stdafx.h"
#include "Ship.h"
#include "Game.h"

Ship::Ship(Game* game)
	: Actor(game)
	, mMoveSpeed(50.0)
{
	mTextureAsset = TextureAsset(U"ship");
	mDrawOrder = 500;
	mScale = 1.0;
	mPosition = { 160, 316 };
	SetColliderSize(70, 30);
}

void Ship::UpdateActor(double deltaTime)
{
	mVelocity.x = mMoveSpeed * (KeyD.pressed() - KeyA.pressed());
	if (KeyA.pressed())
	{
		mPosition.x = Max(30.0, mPosition.x - mMoveSpeed * deltaTime);
	}
	if (KeyD.pressed())
	{
		mPosition.x = Min(290.0, mPosition.x + mMoveSpeed * deltaTime);
	}

	Actor::UpdateActor(deltaTime);
}

void Ship::Draw()
{
	Actor::Draw();
	if (mIsFacingRight)
	{
		TextureAsset(U"sail").drawAt(mPosition + Vec2{ 4, -32 });
	}
	else
	{
		TextureAsset(U"sail").mirrored().drawAt(mPosition+ Vec2{ -4, -32 });
	}
	TextureAsset(U"girl").drawAt(mGame->GetShip()->GetPosition() + Vec2{ -5, -19 });
	if (mVelocity.x > 0)
	{
		mGame->AnimSplash[static_cast<int>(Scene::Time() / 0.1) % 4].drawAt(mPosition + Vec2{ 0, 8 });
	}
	else if (mVelocity.x < 0)
	{
		mGame->AnimSplash[static_cast<int>(Scene::Time() / 0.1) % 4].mirrored().drawAt(mPosition + Vec2{ 0, 8 });
	}
}

