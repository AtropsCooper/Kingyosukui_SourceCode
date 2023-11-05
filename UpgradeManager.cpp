#include "stdafx.h"
#include "UpgradeManager.h"

Upgrade::Upgrade(UpgradeManager* manager)
	: mManager(manager)
	, mTextureStr()
	, mTitle()
	, mDescription()
{
}

void Upgrade::ApplyUpgrade()
{
	mFunction();
}

UpgradeManager::UpgradeManager(Game* game)
	: mGame(game)
{
	Reset();
}

UpgradeManager::~UpgradeManager()
{
}

void UpgradeManager::Reset()
{
	mExpNextLevel = 100;
	mMaxHP = 50;
	mHPReg = 0;

	// Hand
	mHandScale = 1.0;
	mLaunchScale = 1.0;
	mStrechSpeed = 0.5;
	mScoopSpeed = 2.0;
	mReloadSpeed = 5.0;

	// Fish Spawn Rate
	mMaxFish = 20;
	mFishPerFrame = 0.8;
	mPufferFishSR = 200;
	mMarlinSR = 80;
	mTinFishSR = 20;
	mGoldFishSR = 2;
	mCrabSR = 1;

	// Bird Spawn Rate
	mMaxBird = 30;
	mBirdPerFrame = 0.63;
	mFireBallSR = 100;
	mLaserSR = 5;

	////////// Fish
	// Puffer Fish
	mPufferFishExp = 1;
	mPufferFishMaxBouces = 1;
	mPufferFishLaunchScale = 1.2;

	// Marlin
	mMarlinExp = 1;
	mMarlinPiercingLoss = 100;

	// Tin Fish
	mTinExp = 3;
	mTinAccelaration = 200.0;
	mTinExplosionRadius = 120.0;

	// Gold Fish
	mGoldFishExp = 10;

	// Crab
	mCrabExp = 2;
	mCrabDigChance = 0.1;

	////////// Bird
	// Fireball bird
	mFireballExp = 0;
	mFireballAtkRate = 0.2;
	mFireballDmg = 1;
	
	// Laser bird
	mLaserExp = 0;
	mLaserAtkRate = 0.2;
	mLaserDmg = 3;

	// Boss
	mBossHP = 100;
	mStarDmg = 5;
	mSparkDmg = 10;
}

Upgrade* UpgradeManager::GenerateUpgrade()
{
	int r = Random(18);
	switch (r)
	{
	case 0: return new HandGrowth(this);
	case 1: return new FastSwimer(this);
	case 2: return new Bait(this);
	case 3: return new HPReg(this);
	case 4: return new Heal(this);
	case 5: return new MaxHP(this);
	case 6: return new PowerUp(this);
	case 7: return new PowerDown(this);
	case 8: return new PufferBounce(this);
	case 9: return new PufferExp(this);
	case 10: return new MarlinPiercing(this);
	case 11: return new MarlinExp(this);
	case 12: return new TinFishSR(this);
	case 13: return new TinFishExp(this);
	case 14: return new TinFishRange(this);
	case 15: return new GoldFishSR(this);
	case 16: return new CrabSR(this);
	case 17: return new FireBallExp(this);
	case 18: return new UFOExp(this);
	default: return nullptr;
	}
}


