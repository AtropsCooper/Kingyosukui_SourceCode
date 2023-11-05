#include "stdafx.h"
#include "FireballBird.h"
#include "Bullet.h"
#include "Ship.h"
#include "StarBullet.h"

const double FRAME_DURATION = 0.2;

FireballBird::FireballBird(Game* game, Vec2 pos)
	: Bird(game, pos)
{
	mStateChangedAt = Scene::Time();
	SetColliderSize(20, 20);
}

void FireballBird::UpdateActor(double deltaTime)
{
	Actor::UpdateActor(deltaTime);
	if (mState == EActive)
	{
		if (mBirdState != EAttacking)
		{
			if (mPosition.x > 10 && mPosition.x < 310 && RandomBool(mManager->mFireballAtkRate * deltaTime))
			{
				ChangeState(EAttacking);
			}
		}
		else
		{
			if (!mIsAttackDone && Scene::Time() - mStateChangedAt > 3 * FRAME_DURATION)
			{
				mIsFacingRight = (mGame->GetShip()->GetPosition().x - mPosition.x > 0);
				new Bullet(mGame, mPosition + Vec2{ (mIsFacingRight ? 1 : -1) * 9, -4});
				mIsAttackDone = true;
				AudioAsset(U"audio_fire").play();
			}
			if (Scene::Time() - mStateChangedAt > 4 * FRAME_DURATION)
			{
				ChangeState(EWandering);
				mIsAttackDone = false;
			}
		}

		if (mBirdState == EEntering)
		{
			if ((mDestination - mPosition).lengthSq() < 9)
			{
				ChangeState(EWandering);
			}
			else
			{
				mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(2.0, 3.0), mWanderSpeed);
			}
		}
		if (mBirdState == EWandering)
		{
			if ((mDestination - mPosition).lengthSq() < 9)
			{
				mDestination = RandomVec2(BIRD_AREA);
			}
			else
			{
				mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(2.0, 4.0), mWanderSpeed);
			}
		}
	}
}

void FireballBird::Draw()
{
	Texture* frame;
	double delay = Scene::Time() - mStateChangedAt;
	if (mState == EActive)
	{
		if (mBirdState == EAttacking)
		{
			frame = &mGame->AnimFireBirdAttack[static_cast<int>(delay / FRAME_DURATION) % 5];
			mIsFacingRight = (mGame->GetShip()->GetPosition().x - mPosition.x > 0);
		}
		else
		{
			frame = &mGame->AnimFireBirdIdle[static_cast<int>(delay / FRAME_DURATION) % 7];
		}

		if (mIsFacingRight == true)
		{
			frame->scaled(mScale).rotated(mRotation).drawAt(mPosition);
		}
		else
		{
			frame->scaled(mScale).mirrored().rotated(mRotation).drawAt(mPosition);
		}
	}

}
