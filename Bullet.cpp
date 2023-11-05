#include "stdafx.h"
#include "Bullet.h"
#include "Ship.h"
#include "Effects.h"
#include "UpgradeManager.h"

Bullet::Bullet(Game* game, Vec2 pos)
	: Actor(game, pos)
{
	mTextureAsset = TextureAsset(U"fireball");
	mDrawOrder = 800;
	mDirection = (mGame->GetShip()->GetPosition() - mPosition).normalized();
	SetColliderSize(5, 5);

}

void Bullet::UpdateActor(double deltaTime)
{
	if (mLivingTime > 3)
	{
		mState = EDead;
	}
	else if (intersects(mGame->GetShip()))
	{
		mGame->TakeDamage(mGame->GetUpgradeManager()->mFireballDmg);
		mGame->GetEffect()->add<FireBallHit>(mPosition + mDirection * 2, Atan2(mDirection.y, mDirection.x));
		mState = EDead;
	}
	
	mPosition += deltaTime * 100 * mDirection;
	mRotation = mDirection.getAngle() + Math::QuarterPi + Math::Pi;
	
	Actor::UpdateActor(deltaTime);
}
