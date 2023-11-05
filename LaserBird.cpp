#include "stdafx.h"
#include "LaserBird.h"
#include "Ship.h"

const double FRAME_DURATION = 0.1;

LaserBird::LaserBird(Game* game, Vec2 pos)
	: Bird(game, pos)
{
	mStateChangedAt = Scene::Time();
	mDrawOrder = 201;
	SetColliderSize(30, 40);
	mWanderSpeed = 400;
}

void LaserBird::UpdateActor(double deltaTime)
{
	Actor::UpdateActor(deltaTime);
	if (mState == EActive)
	{
		if (mBirdState != EAttacking)
		{
			if (mPosition.x > 10 && mPosition.x < 310 && RandomBool(mManager->mLaserAtkRate * deltaTime))
			{
				ChangeState(EAttacking);
				
				AudioAsset(U"audio_get").play();
			}
		}
		else
		{
			if (!mIsAttackDone && Scene::Time() - mStateChangedAt > 10 * FRAME_DURATION)
			{
				AudioAsset(U"audio_lazer").play();
				if (Abs(mGame->GetShip()->GetPosition().x - mPosition.x) < 60)
				{
					mGame->TakeDamage(mManager->mLaserDmg);
					mIsAttackDone = true;
				}
			}
			if (Scene::Time() - mStateChangedAt > 14 * FRAME_DURATION)
			{
				ChangeState(EWandering);
				mIsAttackDone = false;
			}
		}

		if (mBirdState == EEntering)
		{
			if ((mDestination - mPosition).lengthSq() < 36)
			{
				ChangeState(EWandering);
			}
			else
			{
				mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(1.0, 2.0), mWanderSpeed);
			}
		}
		if (mBirdState == EWandering)
		{
			if ((mDestination - mPosition).lengthSq() < 36)
			{
				mDestination = RandomVec2(BIRD_AREA);
			}
			else
			{
				mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(1.0, 2.5), mWanderSpeed);
			}
		}
	}
}

void LaserBird::Draw()
{
	Texture* frame;
	Texture* laserFrame;
	double delay = Scene::Time() - mStateChangedAt;
	if (mState == EActive)
	{
		if (mBirdState == EAttacking)
		{
			mIsFacingRight = (mGame->GetShip()->GetPosition().x - mPosition.x > 0);
			if (delay < 10 * FRAME_DURATION)
			{
				frame = &mGame->AnimLaserBirdCharge[static_cast<int>(delay / FRAME_DURATION) % 10];
				Line{ static_cast<int>( mPosition.x ), 0, static_cast<int>( mPosition.x ), 320 }
					.draw(LineStyle::SquareDot, 2, HSV{ static_cast<int>(delay / FRAME_DURATION * 0.5) % 2 ? 0 : 30, 0.8, 0.8 });
			}
			else
			{
				frame = &mGame->AnimLaserBirdIdle[static_cast<int>(delay / FRAME_DURATION) % 5];
				laserFrame = &mGame->AnimLaser[static_cast<int>(delay / FRAME_DURATION - 10) % 4];
				laserFrame->drawAt(Vec2{ mPosition.x, 160 });
			}
			
			
		}
		else
		{
			frame = &mGame->AnimLaserBirdIdle[static_cast<int>(delay / FRAME_DURATION) % 5];
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
