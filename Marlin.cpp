#include "stdafx.h"
#include "Marlin.h"
#include "Bird.h"

Marlin::Marlin(Game* game, Vec2 pos)
	: Fish(game, pos)
{
	mTextureAsset = TextureAsset(U"marlin");
	mDrawOrder = 701;
	SetColliderSize(25, 10);
	mWanderSpeed = 200;
	mDashSpeed = 400;
	mExp = mManager->mMarlinExp;
}

void Marlin::Launch(double launchScale, Vec2 des)
{
	Fish::Launch(launchScale, des);
	double radian = Random(ToRadians(45.0 + ((mPosition.x / 320.0) * 45.0)), ToRadians(90.0 + ((mPosition.x / 320.0) * 45.0)));
	mVelocity = Vec2{ Cos(radian), -Sin(radian) } *mDashSpeed * launchScale;
}

void Marlin::LaunchedRoutine(double deltaTime)
{
	Fish::LaunchedRoutine(deltaTime);
	if (mVelocity.x < 0)
	{
		mRotation = Atan2(mVelocity.normalized().y, mVelocity.normalized().x) + Math::Pi;
	}
	else
	{
		mRotation = Atan2(mVelocity.normalized().y, mVelocity.normalized().x);
	}
	mPosition += deltaTime * mVelocity;
	mVelocity += deltaTime * Vec2{ 0, 200.0 }; // gravity
	if (mVelocity.y < 0 && mVelocity.length() > 60 && RandomBool())
	{
		mGame->GetEffect()->add<Wake>(mPosition - mVelocity.normalized() * 15);
	}
	for (auto bird : mGame->GetBirds())
	{
		if (intersects(bird))
		{
			bird->HittedBy(this);
			Vec2 loss = mManager->mMarlinPiercingLoss * mVelocity.normalized();
			if ((mVelocity - loss).y * mVelocity.y < 0)
			{
				mVelocity *= 0.1;
			}
			else
			{
				mVelocity -= loss;
			}
		}
	}
}



