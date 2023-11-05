#include "stdafx.h"
#include "Pufferfish.h"
#include "Ship.h"
#include "Effects.h"

Pufferfish::Pufferfish(Game* game, Vec2 pos)
	: Fish(game,pos)
	, mBounceTimes(0)
{
	mDrawOrder = 700;
	SetColliderSize(20, 20);
	mExp = mManager->mPufferFishExp;
}

void Pufferfish::Draw()
{
	if (mState == EActive)
	{
		Texture* frame;
		if (mFishState != ELaunched)
		{
			frame = &mGame->AnimPufferfish[0];

		}
		else
		{
			double delay = Scene::Time() - mLaunchedTime;
			frame = &mGame->AnimPufferfish[Min(static_cast<int>(delay / 0.06), 4)];
		}
		if (mIsFacingRight == true)
		{
			frame->scaled(mScale).rotated(mRotation).drawAt(mPosition);
		}
		else
		{
			frame->scaled(mScale).mirrored().rotated(mRotation).drawAt(mPosition);
		}
	}
}

void Pufferfish::Launch(double launchScale, Vec2 des)
{
	Fish::Launch(launchScale, des);
	mLaunchedTime = Scene::Time();
	mDashSpeed = launchScale * mDashSpeed;
	if (des.isZero())
	{
		double radian = Random(ToRadians(30.0), ToRadians(150.0));
		mVelocity = Vec2{ Cos(radian), -Sin(radian) } * mDashSpeed;
	}
	else
	{
		mDestination = des;
		mVelocity = (mDestination - mPosition).normalized() * mDashSpeed;
	}

	mTextureAsset = TextureAsset(U"pufferfish2");
	mScale = mManager->mPufferFishLaunchScale;
	SetColliderSize(30 * mManager->mPufferFishLaunchScale, 30 * mManager->mPufferFishLaunchScale);
}

void Pufferfish::Die()
{
	Fish::Die();
	mGame->GetEffect()->add<BubbleEffect>(mPosition, 50, true);
}

void Pufferfish::LaunchedRoutine(double deltaTime)
{
	Fish::LaunchedRoutine(deltaTime);
	mRotation += deltaTime * 2.0;
	mPosition += deltaTime * mVelocity;
	mVelocity += deltaTime * Vec2{ 0, 140.0 }; // gravity
	for (auto bird : mGame->GetBirds())
	{
		if (intersects(bird))
		{
			mBounceTimes += 1;
			if (mBounceTimes >= mManager->mPufferFishMaxBouces)
			{
				AudioAsset(U"audio_puffer").playOneShot();
				Die();
			}
			bird->HittedBy(this);
			Vec2 orientation = bird->GetPosition() - mPosition;
			mVelocity = orientation.normalized() * mDashSpeed;
			if (Abs(orientation.x) > Abs(orientation.y))
			{
				mVelocity.x = -mVelocity.x;
			}
			else
			{
				mVelocity.y = -mVelocity.y;
			}
		}
	}
	if (intersects(mGame->GetShip()) && mVelocity.y > 70.0 && mGame->GetShip()->GetPosition().y - mPosition.y > 14)
	{
		mVelocity.y = -Abs(mVelocity.y);
	}
	if (mPosition.x < 15)
	{
		mVelocity.x = Abs(mVelocity.x);
	}
	if (mPosition.x > 305)
	{
		mVelocity.x = -Abs(mVelocity.x);
	}
}
