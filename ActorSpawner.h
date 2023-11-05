#pragma once
#include "Actor.h"
class ActorSpawner :
    public Actor
{
public:
	ActorSpawner(class Game* game);
	void UpdateActor(double deltaTime);
private:
	void SpawnFish();
	void SpawnBird();
	const class UpgradeManager* mManager;

	Array<class Fish*>& mFish;
	Array<class Bird*>& mBirds;
};

