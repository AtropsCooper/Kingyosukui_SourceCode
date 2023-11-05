#pragma once
#include "Fish.h"
class Crab :
    public Fish
{
public:
	Crab(Game* game, Vec2 pos);
	void Launch(double launchScale, Vec2 des);
	void WanderingRoutine(double deltaTime);
	void LaunchedRoutine(double deltaTime);
private:
	bool mIsDigging;
	double mDigFinishTime;
};

