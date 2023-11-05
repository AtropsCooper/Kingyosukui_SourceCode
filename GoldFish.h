#pragma once
#include "Fish.h"
class GoldFish :
    public Fish
{
public:
	GoldFish(Game* game, Vec2 pos);

	void Launch(double launchScale, Vec2 des) override;

	void LaunchedRoutine(double deltaTime) override;

};

