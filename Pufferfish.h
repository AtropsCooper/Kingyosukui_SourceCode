#pragma once
#include "Fish.h"
class Pufferfish :
    public Fish
{
public:
	Pufferfish(Game* game, Vec2 pos);
	void Draw() override;
	void Launch(double launchScale, Vec2 des = Vec2::Zero()) override;
	void Die() override;
private:
	void LaunchedRoutine(double deltaTime) override;
	int mBounceTimes;
	double mLaunchedTime;
};

