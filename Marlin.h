#pragma once
#include "Fish.h"
class Marlin :
    public Fish
{
public:
	Marlin(Game* game, Vec2 pos);
	void Launch(double launchScale, Vec2 des = Vec2::Zero()) override;

private:
	void LaunchedRoutine(double deltaTime) override;
};

