#pragma once
#include "Actor.h"
class Bird :
    public Actor
{
public:
	enum BirdState
	{
		EEntering,
		EWandering,
		EAttacking
	};

	Bird(class Game* game, Vec2 pos);
	~Bird();

	virtual void HittedBy(class Actor* actor);
	void ChangeState(BirdState state);
protected:
	BirdState mBirdState;
	Vec2 mDestination;
	int mDropExp;
	double mWanderSpeed;
	double mStateChangedAt;
	bool mIsAttackDone;
};

