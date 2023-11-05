#include "stdafx.h"
#include "StarBullet.h"
#include "Ship.h"

StarBullet::StarBullet(Game* game, Vec2 pos, Vec2 dir)
	: Actor(game, pos)
{
	mTexture = Texture(Resource(U"resources/bullets.png"));
	mDrawOrder = 810;
	mDirection = dir;
	SetColliderSize(10, 10);
	mVariant = Random(7);
	mRotateSpeed = Random(-360_deg, 360_deg);
}

void StarBullet::UpdateActor(double deltaTime)
{
	if (mPosition.x < -10 || mPosition.x > 330 || mPosition.y < -10 || mPosition.y > 490)
	{
		mState = EDead;
	}
	else if (intersects(mGame->GetShip()))
	{
		mGame->TakeDamage(mGame->GetUpgradeManager()->mStarDmg);
		mState = EDead;
	}

	mPosition += deltaTime * 80.0 * mDirection;
	mRotation += deltaTime * mRotateSpeed;

	Actor::UpdateActor(deltaTime);
}

void StarBullet::Draw()
{
	mTexture(mVariant * 32, 0, 32, 32).rotated(mRotation).drawAt(mPosition);
}
