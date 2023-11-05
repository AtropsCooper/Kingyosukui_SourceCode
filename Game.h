#pragma once

constexpr Rect BIRD_AREA = { 10, 30, 300, 240 };
constexpr Rect FISH_AREA = { 10, 330, 300, 120 };

class Game
{
	friend class ActorSpawner;
public:
	
	enum GameState {
		ERunning,
		EPaused,
		EQuit
	};
	Game();
	void UpdateGame();
	void UpdateUI();
	void GenerateOutput();

	
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void PushUI(class UIScreen* screen) { mUIStack << screen; };
	bool HasUI() { return !mUIStack.empty(); }

	GameState GetGameState() const { return mGameState; }
	void SetGameState(GameState state) { mGameState = state; }
	int GetExp() const { return mExp; }
	int GetLevel() const { return mLevel; }
	const Effect* GetEffect() const { return &mEffect; }
	const class UpgradeManager* GetUpgradeManager() const { return mUpgradeManager; }
	const class Ship* GetShip() const { return mShip; }
	class Hand* GetHand() const { return mHand; }
	Array<class Fish*>& GetFish() { return mFish; }
	Array<class Bird*>& GetBirds() { return mBirds; }

	void NewGame();
	void GainExp(int value);
	void LevelUp();
	void TakeDamage(double dmg);

	Stopwatch mStopwatch;

	// Animations
	Texture AnimSeaWave[6];
	Texture AnimSplash[4];

	Texture AnimPufferfish[5];

	Texture AnimFireBirdAttack[5];
	Texture AnimFireBirdIdle[7];

	Texture AnimLaserBirdCharge[10];
	Texture AnimLaserBirdIdle[5];
	Texture AnimLaser[4];

	Texture AnimBoss[6];

private:
	void DrawBackground();
	void LoadData();

	GameState mGameState;
	bool mUpdatingActors;
	Effect mEffect;

	Array<class Actor*> mActors;
	Array<class Actor*> mPendingActors;
	Array<class UIScreen*> mUIStack;

	class UpgradeManager* mUpgradeManager;
	class ActorSpawner* mActorSpawner;
	class Ship* mShip; // Player's ship
	class Hand* mHand; // Player's hand

	int mLevel;
	int mExp;
	double mHP;

	Array<class Fish*> mFish;
	Array<class Bird*> mBirds;
};
