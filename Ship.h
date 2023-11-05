#pragma once
#include "Actor.h"
class Ship :
    public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(double deltaTime);
	void Draw();
private:
	double mMoveSpeed;
};

