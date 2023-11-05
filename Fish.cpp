#include "stdafx.h"
#include "Fish.h"

Fish::Fish(Game* game, Vec2 pos)
	: Actor(game, pos)
	, mFishState(EEntering)
	, mExp(1)
	, mWanderSpeed(60)
	, mDashSpeed(200)
	, mLife(5.0 + Random(-1.0, 1.0))
{
	game->GetFish() << this;
	mDestination = RandomVec2(FISH_AREA);
	mVelocity = (mDestination - mPosition).normalized() * mWanderSpeed;
}

Fish::~Fish()
{
	auto iter = std::find(GetGame()->GetFish().begin(),
						  GetGame()->GetFish().end(),
						  this);
	GetGame()->GetFish().erase(iter);
}

void Fish::UpdateActor(double deltaTime)
{
	if (mState == EActive)
	{
		if (mFishState == EEntering)
		{
			EnteringRoutine(deltaTime);
		}
		else if (mFishState == EWandering)
		{
			WanderingRoutine(deltaTime);
		}
		else if (mFishState == EScooped)
		{
			ScoopedRoutine(deltaTime);
		}
		else if (mFishState == ELaunched)
		{
			LaunchedRoutine(deltaTime);
		}
	}
	Actor::UpdateActor(deltaTime);
}

void Fish::OnScooped(Hand* hand)
{
	mFishState = EScooped;
	hand->AddScoopedFish(this);
}

void Fish::Launch(double launchScale, Vec2 des)
{
	mFishState = ELaunched;
	for (auto i : step(mExp))
	{
		new ExpOrb(mGame, mPosition);
	}
}

void Fish::Die()
{
	mState = EDead;
}

void Fish::EnteringRoutine(double deltaTime)
{
	if ((mPosition - mDestination).lengthSq() < 9)
	{
		mFishState = EWandering;
	}
	else
	{
		mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(1.5, 2.5), mWanderSpeed);
	}
}

void Fish::WanderingRoutine(double deltaTime)
{
	if ((mPosition - mDestination).lengthSq() < 9)
	{
		mDestination = RandomVec2(FISH_AREA);
	}
	else
	{
		mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(2.0, 4.0), mWanderSpeed);
	}
}

void Fish::ScoopedRoutine(double deltaTime)
{
}

void Fish::LaunchedRoutine(double deltaTime)
{
	mAirTime += deltaTime;
	if (mAirTime >= mLife)
	{
		Die();
	}
}

