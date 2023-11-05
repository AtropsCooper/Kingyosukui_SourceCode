#pragma once
#include "Actor.h"
class StarBullet :
    public Actor
{
public:
	StarBullet(class Game* game, Vec2 pos, Vec2 dir);
	void UpdateActor(double deltaTime);
	void Draw();
private:
	Vec2 mDirection;
	Texture mTexture;
	int mVariant;
	double mRotateSpeed;
};

