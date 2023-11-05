#include "stdafx.h"
#include "Boss.h"
#include "Pufferfish.h"
#include "Marlin.h"
#include "Tinfish.h"
#include "StarBullet.h"
#include "MasterSpark.h"
#include "UIScreen.h"

const double FRAME_DURATION = 0.05;

Boss::Boss(Game* game, Vec2 pos)
	: Bird(game, pos)
	, mHP(mManager->mBossHP)
	, mNextStarTime(0)
	, mNextSparkTime(Scene::Time() + Random(15.0, 20.0))
	, mAttackEndTime(0)
{
	SetColliderSize(140, 60);
}

void Boss::HittedBy(Actor* actor)
{
	actor->SetState(EDead);
	if (dynamic_cast<const Pufferfish*>(actor) != nullptr)
	{
		mHP -= 1;
	}
	else if (dynamic_cast<const Marlin*>(actor) != nullptr)
	{
		mHP -= 2;
	}
	else if (dynamic_cast<const Tinfish*>(actor) != nullptr)
	{
		mHP -= 4;
	}
	if (mHP <= 0)
	{
		mState = EPaused;
		// Effect
		
		new SummaryScreen(mGame, true);
		//Print << U"You Win";
	}
}

void Boss::UpdateActor(double deltaTime)
{
	Actor::UpdateActor(deltaTime);
	if (mNextStarTime < Scene::Time())
	{
		Vec2 dir = RandomVec2();
		for (int i : step(5))
		{
			new StarBullet(mGame, mPosition, dir.rotate(i * 72));
		}
		mNextStarTime = Scene::Time() + Random(0.6, 1.8);
	}
	if (mBirdState != EAttacking && Scene::Time() > mNextSparkTime)
	{
		mBirdState = EAttacking;
		new MasterSpark(mGame, mPosition);
		mAttackEndTime = Scene::Time() + 9.0;
	}
	if (mBirdState == EAttacking && Scene::Time() > mAttackEndTime)
	{
		mBirdState = EWandering;
		mNextSparkTime = Scene::Time() + Random(15.0, 20.0);
	}
	if (mState == EActive)
	{
		if (mBirdState == EEntering)
		{
			if ((mDestination - mPosition).lengthSq() < 25)
			{
				ChangeState(EWandering);
			}
			else
			{
				mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(3.0, 5.0), mWanderSpeed);
			}
		}
		if (mBirdState == EWandering)
		{
			if ((mDestination - mPosition).lengthSq() < 100)
			{
				mDestination = RandomVec2(Rect{ 40, 50, 240, 170});
			}
			else
			{
				mPosition = Math::SmoothDamp(mPosition, mDestination, mVelocity, Random(3.0, 7.0), mWanderSpeed);
			}
		}
	}

}

void Boss::Draw()
{
	if(mIsFacingRight)
	{
		mGame->AnimBoss[static_cast<int>(Scene::Time() / FRAME_DURATION) % 5].drawAt(mPosition);
	}
	else
	{
		mGame->AnimBoss[static_cast<int>(Scene::Time() / FRAME_DURATION) % 5].mirrored().drawAt(mPosition);
	}
}

