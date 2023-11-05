#include "stdafx.h"
#include "Crab.h"
#include "ExpOrb.h"
#include "Effects.h"

Crab::Crab(Game* game, Vec2 pos)
	: Fish(game, pos)
	, mIsDigging (false)
{
	mTextureAsset = TextureAsset(U"crab");
	mDrawOrder = 798;
	SetColliderSize(15, 15);
	mExp = mManager->mPufferFishExp;
	mDestination = RandomVec2(Rect{ 10, 446, 300, 24 });
}

void Crab::Launch(double launchScale, Vec2 des)
{
	Fish::Launch(launchScale, des);
	double radian = Random(30, 150);
	mVelocity = Vec2{ Cos(radian), -Sin(radian) } * 100.0 * launchScale;
}

void Crab::WanderingRoutine(double deltaTime)
{
	if (!mIsDigging)
	{
		if (RandomBool(mManager->mCrabDigChance * deltaTime))
		{
			mIsDigging = true;
			mDigFinishTime = Scene::Time() + Random(6.0, 12.0);
		}
		else if ((mPosition - mDestination).lengthSq() < 4)
		{
			mDestination = RandomVec2(Rect{ 10, 446, 300, 24 });
		}
		else
		{
			mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(3.0, 5.0), mWanderSpeed);
		}
	}
	else
	{
		if (RandomBool(0.6))
		{
			mGame->GetEffect()->add<Digging>(mPosition);
		}
		if (Scene::Time() > mDigFinishTime)
		{
			mIsDigging = false;
			int exp = Random(10, 30);
			for (auto i : step(exp))
			{
				new ExpOrb(mGame, mPosition);
			}
		}
	}

}


void Crab::LaunchedRoutine(double deltaTime)
{
	Fish::LaunchedRoutine(deltaTime);
	mRotation += deltaTime * Math::Pi;
	mPosition += mVelocity * deltaTime;
	if (mPosition.x < 0 || mPosition.x > 320)
	{
		Die();
	}
}




