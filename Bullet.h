#pragma once
#include "Actor.h"
class Bullet :
    public Actor
{
public:
	Bullet(class Game* game, Vec2 pos);
	void UpdateActor(double deltaTime);
private:
	Vec2 mDirection;
	double mLivingTime;
};

