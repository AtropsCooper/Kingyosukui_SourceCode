#include "stdafx.h"
#include "ExpOrb.h"
#include "Ship.h"

ExpOrb::ExpOrb(Game* game, Vec2 pos)
	: Actor(game, pos)
{
	mTextureAsset = TextureAsset(U"exp_orb");
	SetColliderSize(4, 4);
	mVelocity = Vec2{0.0, -100.0} - RandomVec2(100.0, 250.0);
	mSmoothTime = 0.6 * Random() + 0.3;
	mScale = 0.05;
}

void ExpOrb::UpdateActor(double deltaTime)
{
	
	
	mPosition = Math::SmoothDamp(mPosition, mGame->GetShip()->GetPosition(), mVelocity, mSmoothTime);
	if (intersects(mGame->GetShip()))
	{
		mState = EDead;
		mGame->GainExp(1);
	}
	Actor::UpdateActor(deltaTime);
}


