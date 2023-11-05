#pragma once
#include "Bird.h"
class FireballBird :
    public Bird
{
public:
	FireballBird(Game* game, Vec2 pos);
	void UpdateActor(double deltaTime) override;
	void Draw() override;
};

