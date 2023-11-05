#pragma once
#include "Bird.h"
class LaserBird : public Bird
{
public:
	LaserBird(Game* game, Vec2 pos);

	void UpdateActor(double deltaTime) override;

	void Draw() override;

};
