#include "stdafx.h"
#include "TreasureBox.h"

TreasureBox::TreasureBox(Game* game, Vec2 pos)
	: Fish(game, pos)
{
	mTextureAsset = TextureAsset(U"goldfish");
	mDrawOrder = 700;
	SetColliderSize(32, 32);
	mExp = mManager->mPufferFishExp;
}
