#include "stdafx.h"
#include "Bird.h"
#include "Effects.h"
#include "ExpOrb.h"

Bird::Bird(Game* game, Vec2 pos)
	: Actor(game, pos)
	, mBirdState(EEntering)
	, mIsAttackDone(false)
	, mStateChangedAt(0)
	, mDropExp(0)
	, mWanderSpeed(200.0)
{
	game->GetBirds().emplace_back(this);
	mDrawOrder = 200;
	mDestination = RandomVec2(BIRD_AREA);
	mVelocity = (mDestination - mPosition).normalized() * mWanderSpeed;
}

Bird::~Bird()
{
	auto iter = std::find(GetGame()->GetBirds().begin(),
					  GetGame()->GetBirds().end(),
					  this);
	GetGame()->GetBirds().erase(iter);
}


void Bird::HittedBy(Actor* actor)
{
	mGame->GetEffect()->add<Spark>(mPosition);
	if (mDropExp > 0)
	{
		for (auto i : step(mDropExp))
		{
			new ExpOrb(mGame, mPosition);
		}
	}
	mState = EDead;
	AudioAsset(U"audio_enemyvanish").playOneShot();
}

void Bird::ChangeState(BirdState state)
{
	if (state == EAttacking || mBirdState == EAttacking)
	{
		mStateChangedAt = Scene::Time();
	}
	mBirdState = state;
}
