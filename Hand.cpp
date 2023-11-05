#include "stdafx.h"
#include "Hand.h"
#include "Game.h"
#include "Ship.h"
#include "Fish.h"
#include "Bird.h"
#include "UpgradeManager.h"


const double MIN_ARM_LEN = 10;
const double MAX_ARM_LEN = 110.0;
const double BASE_HAND_HEIGHT = 16;
const double BASE_HAND_WIDTH = 25;

Hand::Hand(Game* game)
	: Actor(game)
	, mPhase(EStreching)
	, mTexArm(U"arm")
	, mArmLength(MIN_ARM_LEN)
	, mArmPos(Vec2::Zero())
	, mStrechSpeed(0.5)
	, mStrechOffset(0.0)
	, mScoopSpeed(2.0)
	, mReloadSpeed(6.0)
	, mScoopBegin(0.0)
	, mReloadBegin(0.0)
	, mScoopEnd(0.0)
{
	mTextureAsset = TextureAsset(U"hand");
	mDrawOrder = 600;
	mPosition = mGame->GetShip()->GetPosition();
	mScale = 1.0;
	SetColliderSize(BASE_HAND_WIDTH, BASE_HAND_HEIGHT);
	UpdateHand();
}

void Hand::UpdateActor(double deltaTime)
{
	Vec2 origin = mGame->GetShip()->GetPosition() + Vec2{0, -14};
	double time = Scene::Time();
	if (mPhase == EStreching)
	{
		mArmLength = MIN_ARM_LEN + (MAX_ARM_LEN - MIN_ARM_LEN) * Math::Abs(mStrechSpeed * (time + mStrechOffset) - Math::Floor(mStrechSpeed * (time + mStrechOffset) + 0.5));
		if (MouseL.down() || KeyS.down() || KeySpace.down())
		{
			mPhase = EScooping;
			mScoopBegin = time;
			mReloadBegin = mScoopBegin + (Math::Pi / mScoopSpeed);
			mScoopEnd = mReloadBegin + (Math::Pi / mReloadSpeed);
		}
	}
	else if (mPhase == EScooping)
	{
		mRotation = (time - mScoopBegin) * mScoopSpeed;
		for (auto fish : mGame->GetFish())
		{
			if (fish->GetPosition().x <= 320 &&
				fish->GetPosition().x >= 0 &&
				intersects(fish))
			{
				if (fish->GetFishState() == Fish::EWandering ||
					fish->GetFishState() == Fish::EEntering)
				{
					fish->OnScooped(this);
				}

			}
		}
		for (auto fish : mScoopedFish)
		{
			fish->SetRotation(fish->GetRotation() + deltaTime * mScoopSpeed);
			fish->SetPosition(mPosition);
		}
		if (time >= mReloadBegin)
		{
			mPhase = EReloading;
			mRotation = Math::Pi;
			auto targets = mGame->GetBirds().choice(Min(mScoopedFish.size(), mGame->GetBirds().size()));
			for (auto fish : mScoopedFish)
			{
				if (targets.size() > 0)
				{
					fish->Launch(mLaunchScale, targets.back()->GetPosition());
					targets.pop_back();
				}
				else
				{
					fish->Launch(mLaunchScale);
				}
			}
			mScoopedFish.clear();
		}
	}
	else // Reloading
	{
		AudioAsset(U"audio_launch").play();
		mRotation = Math::Pi + (time - mReloadBegin) * mReloadSpeed;
		if (time >= mScoopEnd)
		{
			mPhase = EStreching;
			mRotation = 0.0;
			double p = 1 / mStrechSpeed;
			mStrechOffset = p - Math::Fmod(mScoopEnd - mScoopBegin, p);
			UpdateHand();
		 }
	}
	mArmPos = origin + Vec2{ (mArmLength + 5) * Cos(mRotation), (mArmLength + 5) * Sin(mRotation) };
	mPosition = origin + Vec2{ (2 * mArmLength + 0.5 * BASE_HAND_WIDTH * mHandScale) * Cos(mRotation), (2 * mArmLength + 0.5 * BASE_HAND_WIDTH * mHandScale) * Sin(mRotation) };
	Actor::UpdateActor(deltaTime);
}

void Hand::UpdateHand()
{
	auto manager = mGame->GetUpgradeManager();
	if (manager != nullptr)
	{
		mHandScale = manager->mHandScale;
		SetColliderSize(BASE_HAND_WIDTH * mHandScale, BASE_HAND_HEIGHT);
		mLaunchScale = manager->mLaunchScale;
		mStrechSpeed = manager->mStrechSpeed;
		mScoopSpeed = manager->mScoopSpeed;
		mReloadSpeed = manager->mReloadSpeed;
	}
}

void Hand::Draw()
{
	mTexArm.resized(Vec2{ 2 * mArmLength, 16 }).rotated(mRotation).drawAt(mArmPos);
	TextureAsset(U"hand").resized(Vec2{ BASE_HAND_WIDTH * mHandScale, BASE_HAND_HEIGHT }).rotated(mRotation).drawAt(mPosition);
	TextureAsset(U"sleeve").rotatedAt(Vec2{ 0, 9 }, mRotation).drawAt(mGame->GetShip()->GetPosition() + Vec2{ 0, -14 });
}

void Hand::AddScoopedFish(Fish* fish)
{
	mScoopedFish.emplace_back(fish);
}
