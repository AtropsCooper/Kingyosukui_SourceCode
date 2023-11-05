#pragma once
#include "Bird.h"
class Boss :
    public Bird
{
public:
	Boss(Game* game, Vec2 pos);

	void HittedBy(Actor* actor) override;

	void UpdateActor(double deltaTime);

	void Draw();

private:
	int mHP;
	double mAttackEndTime;
	double mNextStarTime;
	double mNextSparkTime;
};
