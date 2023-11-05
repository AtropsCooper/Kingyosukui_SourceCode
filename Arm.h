#pragma once
#include "Actor.h"
class Arm :
    public Actor
{
public:
	Arm(class Game* game);
	void UpdateActor(double deltaTime);
};

