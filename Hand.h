#pragma once
#include "Actor.h"

class Hand :
    public Actor
{
public:
	enum Phase
	{
		EStreching,
		EScooping,
		EReloading
	};

	Hand(class Game* game);
	void UpdateActor(double deltaTime);
	void UpdateHand();
	void Draw();

	Phase GetHandState() const { return mPhase; }

	void AddScoopedFish(class Fish* fish);
private:
	Phase mPhase;
	double mArmLength;
	Vec2 mArmPos;

	double mHandScale;
	double mLaunchScale;
	double mStrechSpeed;
	double mScoopSpeed;
	double mReloadSpeed;

	// Align arm length between scoops
	double mStrechOffset;

	// Scoop -> Reload -> End
	double mScoopBegin;
	double mReloadBegin;
	double mScoopEnd;

	TextureAsset mTexArm;

	Array<class Fish*> mScoopedFish;
};

