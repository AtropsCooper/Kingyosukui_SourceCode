#pragma once

#include "Game.h"
#include "UpgradeManager.h"

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};
	Actor(class Game* game, Vec2 pos = Vec2::Zero());
	virtual ~Actor();

	void Update(double deltaTime);
	virtual void UpdateActor(double deltaTime);

	int GetDrawOrder() const { return mDrawOrder; }
	class Game* GetGame() const { return mGame; }
	State GetState() const { return mState; }
	Vec2 GetPosition() const { return mPosition; }
	double GetScale() const { return mScale; }
	double GetRotation() const { return mRotation; }
	const RectF& GetCollider() const { return mCollider; }
	bool IsFacingRight() const { return mIsFacingRight; }

	inline void SetState(State state)   { mState = state; }
	inline void SetPosition(Vec2& pos)  { mPosition = pos; }
	inline void SetRotation(double rot) { mRotation = rot; }
	inline void SetColliderSize(double w, double h) { mCollider.setSize(w, h); }

	inline bool intersects(const class Actor* other) { return mCollider.rotated(mRotation).intersects(other->GetCollider().rotated(other->GetRotation())); }
	virtual void Draw();
	void DrawCollider();

protected:
	bool mIsFacingRight;
	int mDrawOrder;
	State mState;
	Vec2 mPosition;
	Vec2 mVelocity;
	double mScale;
	double mRotation;
	TextureAsset mTextureAsset;
	RectF mCollider;

	const class UpgradeManager* mManager;
	class Game* mGame; 
};

