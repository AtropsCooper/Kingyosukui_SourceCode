#include "stdafx.h"
#include "Actor.h"
#include "Game.h"

Actor::Actor(Game* game, Vec2 pos)
	:mState(EActive)
	, mDrawOrder(500)
	, mIsFacingRight(true)
	, mPosition(pos)
	, mVelocity(Vec2::Zero())
	, mScale(1.0)
	, mRotation(0.0)
	, mTextureAsset(U"")
	, mCollider({})
	, mGame(game)
	, mManager(nullptr)
{
	game->AddActor(this);
	mManager = mGame->GetUpgradeManager();
}


Actor::~Actor()
{
	mGame->RemoveActor(this);
}


void Actor::Update(double deltaTime)
{
	if (mState != EDead)
	{
		UpdateActor(deltaTime);
	}
}


void Actor::UpdateActor(double deltaTime)
{
	if (mVelocity.x < -0.01)
	{
		mIsFacingRight = false;
	}
	else if (mVelocity.x > 0.01)
	{
		mIsFacingRight = true;
	}
	mCollider.setCenter(mPosition);
}

void Actor::Draw()
{
	if (mState == EActive && !mTextureAsset.isEmpty())
	{
		if (mIsFacingRight == true)
		{
			mTextureAsset.scaled(mScale).rotated(mRotation).drawAt(mPosition);
		}
		else
		{
			mTextureAsset.scaled(mScale).mirrored().rotated(mRotation).drawAt(mPosition);
		}
	}

}

void Actor::DrawCollider()
{
	mCollider.rotated(mRotation).drawFrame(2, 0, Palette::Crimson);
}



