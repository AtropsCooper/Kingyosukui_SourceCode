#include "stdafx.h"
#include "MasterSpark.h"
#include "Effects.h"

const int WIDTH = 85;

MasterSpark::MasterSpark(Game* game, Vec2 pos)
	: Actor(game, pos)
{
	mDrawOrder = 809;
	
	mStopwatch.start();
}

void MasterSpark::UpdateActor(double deltaTime)
{
	// Preparing
	if (mStopwatch.sF() < 3.0)
	{
		if (RandomBool(0.6))
		{
			mGame->GetEffect()->add<Drain>(mPosition);
		}
		if (mStopwatch.sF() > 1.2 )
		{
			AudioAsset(U"audio_charge").play();
		}
	}
	else if (mStopwatch.sF() < 8.0)// Shooting
	{
		if (Abs(mGame->GetShip()->GetPosition().x - mPosition.x) < WIDTH)
		{
			mGame->TakeDamage(mManager->mSparkDmg * deltaTime);
		}
		mGame->GetEffect()->add<Maho>(mPosition);
		AudioAsset(U"audio_nep").play();
	}
	else
	{
		mState = EDead;
	}
}

void MasterSpark::Draw()
{
	// Preparing
	if (mStopwatch.sF() < 3.0)
	{
		const double e = EaseOutCubic(mStopwatch.sF() / 3.0);
		Circle{ mPosition, (1.0 - e) * 350.0 }. drawFrame(30.0 * (1.0 - e), HSV(190, 0.5, 0.9, 0.3));
		Circle{ mPosition, (1.0 - e) * 330.0 }. drawFrame(15.0 * (1.0 - e), ColorF(0.9, 0.3));
		Circle{ mPosition, e * 35.0 }.draw(HSV(254, 0.35, 0.98, 0.3));
		Circle{ mPosition, e * 32.0 }.draw(HSV(250, 0.25, 0.98, 0.4));
		Circle{ mPosition, e * 30.0 }.draw(HSV(240, 0.2, 0.98, 0.6));
		Circle{ mPosition, e * 28.0 }.draw(HSV(233, 0.2, 0.99, 0.7));
	}
	else if (mStopwatch.sF() < 8.0)// Shooting
	{
		Circle{ mPosition, 35.0 }.draw(HSV(254, 0.35, 0.98, 0.3));
		Circle{ mPosition, 32.0 }.draw(HSV(250, 0.25, 0.98, 0.4));
		Circle{ mPosition, 30.0 }.draw(HSV(240, 0.2, 0.98, 0.6));
		Circle{ mPosition, 28.0 }.draw(HSV(233, 0.2, 0.99, 0.7));
		Circle{ mPosition + Vec2::Down(75), 85.0}.draw(HSV(213, 0.07, 0.99, 0.1));
		Circle{ mPosition + Vec2::Down(65), 75.0 }.draw(HSV(213, 0.07, 0.99, 0.15));
		Circle{ mPosition + Vec2::Down(55), 65.0 }.draw(HSV(213, 0.07, 0.99, 0.15));
		RectF{ mPosition.x - 20, mPosition.y, 40, 480 }.draw(HSV(233, 0.2, 0.99, 0.4));
	}
}
