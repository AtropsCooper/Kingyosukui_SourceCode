#pragma once
#include "Fish.h"
class Tinfish :
	public Fish
{
public:
	Tinfish(Game* game, Vec2 pos);
	void Launch(double launchScale, Vec2 des = Vec2::Zero()) override;
	void Die() override;
private:
	void LaunchedRoutine(double deltaTime) override;

	double mAccelaration;
};

