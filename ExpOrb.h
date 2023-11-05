#pragma once
#include "Actor.h"
class ExpOrb :
    public Actor
{
public:
	ExpOrb(class Game* game, Vec2 pos);
	void UpdateActor(double deltaTime);

private:
	double mSmoothTime;
	Vec2 mVelocity;
};

