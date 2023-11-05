#pragma once
#include "Actor.h"
#include "Ship.h"

class MasterSpark :
    public Actor
{
public:
	MasterSpark(Game* game, Vec2 pos);
	void UpdateActor(double deltaTime);
	void Draw();
private:
	Stopwatch mStopwatch;
};

