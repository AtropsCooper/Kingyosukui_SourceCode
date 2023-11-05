#pragma once
#include "Game.h"
class UpgradeManager;

class Upgrade
{
public:
	Upgrade(UpgradeManager* manager);
	void ApplyUpgrade();

	TextureAsset GetTexture() const { return TextureAsset( mTextureStr ); }
	String GetTitle() const { return mTitle; }
	String GetDescription(int i) const { return mDescription[i]; }
protected:
	UpgradeManager* mManager;
	String mTextureStr;
	String mTitle;
	String mDescription[2];
	std::function<void()> mFunction;
};

class UpgradeManager
{
public:
	UpgradeManager(class Game* game);
	~UpgradeManager();

	class Game* GetGame() const { return mGame; }

	void Reset();

	Upgrade* GenerateUpgrade();

	int mExpNextLevel;
	double mMaxHP;
	double mHPReg;
	// Hand
	double mHandScale;
	double mLaunchScale;
	double mStrechSpeed;
	double mScoopSpeed;
	double mReloadSpeed;

	// Fish Spawn Rate
	int mMaxFish;
	double mFishPerFrame;
	int mPufferFishSR;
	int mMarlinSR;
	int mTinFishSR;
	int mGoldFishSR;
	int mCrabSR;

	// Bird Spawn Rate
	int mMaxBird;
	double mBirdPerFrame;
	int mFireBallSR;
	int mLaserSR;

	////////// Fish
	// Puffer Fish
	int mPufferFishExp;
	int mPufferFishMaxBouces;
	double mPufferFishLaunchScale;

	// Marlin
	int mMarlinExp;
	double mMarlinPiercingLoss;

	// Tin Fish
	int mTinExp;
	double mTinAccelaration;
	double mTinExplosionRadius;

	// Gold Fish
	int mGoldFishExp;

	// Crab
	int mCrabExp;
	double mCrabDigChance;

	////////// Bird
	// Fireball bird
	int mFireballExp;
	double mFireballAtkRate;
	double mFireballDmg;

	// Laser bird
	int mLaserExp;
	double mLaserAtkRate;
	double mLaserDmg;

	//Boss
	int mBossHP;
	double mStarDmg;
	double mSparkDmg;
private:
	class Game* mGame;
};

//////////////// Upgrades

class HandGrowth : public Upgrade
{
public:
	HandGrowth(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_bighand";
		mTitle = U"膨む手";
		mDescription[0] = U"+ 手がすこし大きくなる";
		mDescription[1] = U"- 水の抵抗が増える";
		mFunction = [manager]() { manager->mHandScale += 0.11; manager->mScoopSpeed -= 0.09; };
	}
};

class FastSwimer : public Upgrade
{
public:
	FastSwimer(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_speedup";
		mTitle = U"流線形";
		mDescription[0] = U"+ 水の抵抗を減らす";
		mDescription[1] = U"- 手がすこし小さくなる";
		mFunction = [manager]() { manager->mHandScale = Max(0.1, manager->mHandScale - 0.09); manager->mScoopSpeed += 0.11; };
	}
};

class Bait : public Upgrade
{
public:
	Bait(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_bait";
		mTitle = U"餌付け";
		mDescription[0] = U"+ 魚がしばしば出る";
		mDescription[1] = U"- 鳥に狙われる";
		mFunction = [manager]() { manager->mMaxFish += 2; manager->mFishPerFrame += 0.02; manager->mMaxBird += 2; manager->mBirdPerFrame += 0.02; };
	}
};

class HPReg : public Upgrade
{
public:
	HPReg(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_reg";
		mTitle = U"芽吹く船";
		mDescription[0] = U"+ HPはゆっくりと回復する";
		mDescription[1] = U"- UFOがより多く現われる";
		mFunction = [manager]() { manager->mHPReg += 0.2; manager->mLaserSR += 5; };
	}
};

class Heal : public Upgrade
{
public:
	Heal(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_heal";
		mTitle = U"船の修理";
		mDescription[0] = U"+ HPを半分回復する";
		mDescription[1] = U"デメリットなんかねぇよ";
		mFunction = [manager]() {  manager->GetGame()->TakeDamage(-manager->mMaxHP * 0.5); };
	}
};

class MaxHP : public Upgrade
{
public:
	MaxHP(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_maxhp";
		mTitle = U"船体強化";
		mDescription[0] = U"+ HP上限が増える";
		mDescription[1] = U"- 力がやや弱くなる";
		mFunction = [manager]() {  manager->mMaxHP += 10; manager->mLaunchScale = Max( 0.05, manager->mLaunchScale -0.1); manager->mReloadSpeed = Max(manager->mReloadSpeed-0.1, 0.05); };
	}
};

class PowerUp : public Upgrade
{
public:
	PowerUp(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_powerup";
		mTitle = U"筋トレ";
		mDescription[0] = U"+ 力が強くなる";
		mDescription[1] = U"- 魚に嫌われる";
		mFunction = [manager]() {  manager->mLaunchScale += 0.2; manager->mReloadSpeed += 0.3; manager->mFishPerFrame -= 0.02; };
	}
};

class PowerDown : public Upgrade
{
public:
	PowerDown(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"icon_powerdown";
		mTitle = U"平和主義";
		mDescription[0] = U"+ 力が弱くなる";
		mDescription[1] = U"- 鳥の攻撃が少なくなる";
		mFunction = [manager]() {  manager->mLaunchScale = Max (manager->mLaunchScale-0.15, 0.05); manager->mReloadSpeed += 0.2; manager->mBirdPerFrame -= 0.03; manager->mMaxBird -= 2; };
	}
};

class PufferBounce : public Upgrade
{
public:
	PufferBounce(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"pufferfish";
		mTitle = U"ぽよってるふぐ";
		mDescription[0] = U"+ ふぐの跳ね返る性能強化";
		mDescription[1] = U"- 鳥が煽られる";
		mFunction = [manager]() {  manager->mPufferFishMaxBouces += 1; manager->mFireballAtkRate += 0.05; manager->mLaserAtkRate += 0.05; };
	}
};

class PufferExp : public Upgrade
{
public:
	PufferExp(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"pufferfish";
		mTitle = U"ふぐ鍋";
		mDescription[0] = U"+ ふぐからのExpが増える";
		mDescription[1] = U"- HP上限が減る";
		mFunction = [manager]() {  manager->mPufferFishExp += 1; manager->mMaxHP -= 10; };
	}
};

class MarlinPiercing : public Upgrade
{
public:
	MarlinPiercing(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"marlin";
		mTitle = U"刃磨き";
		mDescription[0] = U"+ 貫通力向上";
		mDescription[1] = U"- ふぐが少なくなる";
		mFunction = [manager]() {  manager->mMarlinPiercingLoss -= 10; manager->mPufferFishSR = Max(manager->mPufferFishSR - 40, 0); };
	}
};

class MarlinExp : public Upgrade
{
public:
	MarlinExp(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"marlin";
		mTitle = U"カジキ焼き";
		mDescription[0] = U"+ カジキからのExpげ増える";
		mDescription[1] = U"- カジキが少なくなる";
		mFunction = [manager]() {  manager->mMarlinExp += 10; manager->mMarlinSR = Max(manager->mMarlinSR - 5, 0); };
	}
};

class TinFishSR : public Upgrade
{
public:
	TinFishSR(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"tinfish";
		mTitle = U"天下布武";
		mDescription[0] = U"+ 魚雷が増える";
		mDescription[1] = U"- 鳥が挑発される";
		mFunction = [manager]() {  manager->mTinFishSR += 10; manager->mTinAccelaration += 10; manager->mFireballAtkRate += 0.02; manager->mLaserAtkRate += 0.02; };
	}
};

class TinFishExp : public Upgrade
{
public:
	TinFishExp(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"tinfish";
		mTitle = U"...食べるの？";
		mDescription[0] = U"+ 魚雷からのExpが大量増える";
		mDescription[1] = U"- 腹痛い...";
		mFunction = [manager]() {  manager->mTinExp += 3; manager->mHPReg -= 0.2; };
	}
};

class TinFishRange : public Upgrade
{
public:
	TinFishRange(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"tinfish";
		mTitle = U"爆発はロマン";
		mDescription[0] = U"+ 爆発範囲超強化";
		mDescription[1] = U"- 手が小さくなる";
		mFunction = [manager]() {  manager->mTinExplosionRadius += 20; manager->mHandScale = Max(0.1, manager->mHandScale - 0.1); };
	}
};

class GoldFishSR : public Upgrade
{
public:
	GoldFishSR(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"goldfish";
		mTitle = U"ゲームタイトルだよ";
		mDescription[0] = U"+ 金魚出現率上昇";
		mDescription[1] = U"";
		mFunction = [manager]() {  manager->mGoldFishSR += 2; };
	}
};

class CrabSR : public Upgrade
{
public:
	CrabSR(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"crab";
		mTitle = U"誰だお前";
		mDescription[0] = U"+ 蟹出現率上昇";
		mDescription[1] = U"";
		mFunction = [manager]() {  manager->mCrabSR += 1; };
	}
};

class FireBallExp : public Upgrade
{
public:
	FireBallExp(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"bird";
		mTitle = U"カモメへの挑戦状";
		mDescription[0] = U"+ カモメ大強化";
		mDescription[1] = U"+ カモメからのExpが増える";
		mFunction = [manager]() {  manager->mMaxBird += 2; manager->mFireballExp += 1; manager->mFireballAtkRate += 0.04; manager->mFireballDmg += 1; };
	}
};

class UFOExp : public Upgrade
{
public:
	UFOExp(UpgradeManager* manager)
		: Upgrade(manager)
	{
		mTextureStr = U"ufo";
		mTitle = U"UFOへの挑戦状";
		mDescription[0] = U"+ UFO大強化";
		mDescription[1] = U"+ UFOからのExpが増える";
		mFunction = [manager]() {  manager->mMaxBird += 2; manager->mLaserExp += 1; manager->mLaserAtkRate += 0.04; manager->mLaserDmg += 2; };
	}
};
