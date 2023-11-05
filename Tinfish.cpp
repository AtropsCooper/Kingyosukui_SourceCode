#include "stdafx.h"
#include "Tinfish.h"
#include "Effects.h"

Tinfish::Tinfish(Game* game, Vec2 pos)
	: Fish(game, pos)
	, mAccelaration(100)
{
	mTextureAsset = TextureAsset(U"tinfish");
	mDrawOrder = 702;
	SetColliderSize(30, 15);
	mExp = mManager->mTinExp;
}

void Tinfish::Launch(double launchScale, Vec2 des)
{
	Fish::Launch(launchScale, des);
	mAccelaration = mManager->mTinAccelaration * launchScale;
	if (des == Vec2::Zero())
	{
		mVelocity = (RandomVec2(BIRD_AREA) - mPosition).normalized() * mAccelaration * 0.01;
	}
	else
	{
		mVelocity = (des - mPosition).normalized() * mAccelaration * 0.01;
	}
}


void Tinfish::Die()
{
	Fish::Die();
	AudioAsset(U"audio_tnt").playOneShot();
	mGame->GetEffect()->add<Explosion>(mPosition, mManager->mTinExplosionRadius);
	for (auto bird : mGame->GetBirds())
	{
		if (mPosition.distanceFrom(bird->GetPosition()) < mManager->mTinExplosionRadius)
		{
			bird->HittedBy(this);
		}
	}
}

void Tinfish::LaunchedRoutine(double deltaTime)
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
	if (RandomBool(0.6))
	{
		mGame->GetEffect()->add<RocketSmoke>(mPosition - (mVelocity.normalized() * 20), -mVelocity);
	}
	
	mVelocity += mVelocity.normalized() * mAccelaration * deltaTime;
	mPosition += mVelocity * deltaTime;
	for (auto bird : mGame->GetBirds())
	{
		if (intersects(bird))
		{
			bird->HittedBy(this);
			Die();
		}
	}
	if (mPosition.y < 10 || (mPosition.x < 10 && mPosition.y < 200 ) || mPosition.x > 310 )
	{
		Die();
	}
}
