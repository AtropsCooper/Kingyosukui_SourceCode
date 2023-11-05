#pragma once
#include "Actor.h"
#include "ExpOrb.h"
#include "Bird.h"
#include "Game.h"
#include "Hand.h"
#include "Effects.h"

class Fish :
    public Actor
{
public:
	enum FishState
	{
		EEntering,
		EWandering,
		EScooped,
		ELaunched
	};
	Fish(class Game* game, Vec2 pos);
	~Fish();
	void UpdateActor(double deltaTime) override;

	void OnScooped(class Hand* hand);
	virtual void Launch(double launchScale, Vec2 des = Vec2::Zero());
	virtual void Die();

	FishState GetFishState() const { return mFishState; }
protected:
	virtual void EnteringRoutine(double deltaTime);
	virtual void WanderingRoutine(double deltaTime);
	virtual void ScoopedRoutine(double deltaTime);
	virtual void LaunchedRoutine(double deltaTime);

	FishState mFishState;

	double mWanderSpeed;
	double mDashSpeed;
	double mAirTime;
	double mLife;
	Vec2 mDestination;

	int mExp;
};

