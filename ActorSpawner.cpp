#include "stdafx.h"
#include "ActorSpawner.h"
#include "Ship.h"
#include "Hand.h"
#include "Bird.h"
#include "UpgradeManager.h"
#include "Pufferfish.h"
#include "Marlin.h"
#include "GoldFish.h"
#include "Crab.h"
#include "Tinfish.h"
#include "FireballBird.h"
#include "LaserBird.h"

ActorSpawner::ActorSpawner(Game* game)
	: Actor(game)
	, mFish(game->mFish)
	, mBirds(game->mBirds)
	, mManager(nullptr)
{
	mManager = mGame->GetUpgradeManager();
}

void ActorSpawner::UpdateActor(double deltaTime)
{
	if (mManager == nullptr) return;

	int fishInsuf = mManager->mMaxFish - mFish.size();
	int birdInsuf = mManager->mMaxBird - mBirds.size();
	if (fishInsuf > 0)
	{
		double fishPerFrame = (1.5 - Pow(fishInsuf / mManager->mMaxFish, 2)) * mManager->mFishPerFrame;
		double chanceInt, chanceFrac;
		chanceFrac = modf(fishPerFrame, &chanceInt);
		for (auto i : step(static_cast<int>(chanceInt)))
		{
			SpawnFish();
		}
		if (RandomBool(fishPerFrame * deltaTime))
		{
			SpawnFish();
		}
	}
	if (birdInsuf > 0)
	{
		double birdPerFrame = (1.5 - Pow(birdInsuf / mManager->mMaxBird, 2)) * mManager->mBirdPerFrame;
		double chanceInt, chanceFrac;
		chanceFrac = modf(birdPerFrame, &chanceInt);
		for (auto i : step(static_cast<int>(chanceInt)))
		{
			SpawnBird();
		}
		if (RandomBool(birdPerFrame * deltaTime))
		{
			SpawnBird();
		}
	}
}

void ActorSpawner::SpawnFish()
{
	Vec2 pos = RandomVec2(FISH_AREA);
	pos.x = RandomBool() ? -20 : 340;

	Array<int> chanceArray;
	chanceArray << mManager->mPufferFishSR;
	chanceArray << mManager->mMarlinSR;
	chanceArray << mManager->mTinFishSR;
	chanceArray << mManager->mGoldFishSR;
	chanceArray << mManager->mCrabSR;

	auto SumUntil = [chanceArray](int index) {
		int sum = 0;
		for (auto i : step(index))
		{
			sum += chanceArray[i];
		}
		return sum;
	};

	int totalChance = SumUntil(5);
	int roll = Random(totalChance);
	if (roll < SumUntil(1))
	{
		new Pufferfish(mGame, pos);
	}
	else if (roll < SumUntil(2))
	{
		new Marlin(mGame, pos);
	}
	else if (roll < SumUntil(3))
	{
		new Tinfish(mGame, pos);
	}
	else if (roll < SumUntil(4))
	{
		new GoldFish(mGame, pos);
	}
	else
	{
		new Crab(mGame, Vec2{ pos.x, 460 });
	}
}

void ActorSpawner::SpawnBird()
{
	Vec2 pos = RandomVec2(BIRD_AREA);
	pos.x = RandomBool() ? -20 : 340;

	int totalChance = mManager->mFireBallSR + mManager->mLaserSR;
	int roll = Random(totalChance);
	if (roll < mManager->mLaserSR)
	{
		new LaserBird(mGame, pos);
	}
	else
	{
		new FireballBird(mGame, pos);
	}
}
