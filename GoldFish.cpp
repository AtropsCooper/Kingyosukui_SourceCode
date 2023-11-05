#include "stdafx.h"
#include "GoldFish.h"
#include "Ship.h"
#include "Effects.h"

GoldFish::GoldFish(Game* game, Vec2 pos)
	: Fish (game, pos)
{
	mTextureAsset = TextureAsset(U"goldfish");
	mDrawOrder = 799;
	SetColliderSize(15, 15);
	mExp = mManager->mPufferFishExp;
	mLife = 20;
}

void GoldFish::Launch(double launchScale, Vec2 des)
{
	Fish::Launch(launchScale, des);
	double speed = Random(400.0, 1600.0) * launchScale;
	mVelocity.y = -speed;
	// G = 200.0
	mVelocity.x = (mGame->GetShip()->GetPosition().x - mPosition.x) * 100.0 / speed;
	AudioAsset(U"audio_kingyo").playOneShot();
}

void GoldFish::LaunchedRoutine(double deltaTime)
{
	Fish::LaunchedRoutine(deltaTime);
	mGame->GetEffect()->add<GoldTrace>(mPosition, mVelocity.y);
	mRotation = Atan2(mVelocity.y, mVelocity.x);
	mPosition += mVelocity * deltaTime;
	mVelocity += Vec2::Down(200.0) * deltaTime;
	if (intersects(mGame->GetShip()))
	{
		mGame->GainExp(100);
		Die();
	}
}
