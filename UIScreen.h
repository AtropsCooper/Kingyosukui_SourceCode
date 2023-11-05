#pragma once

class Button
{
public:
	Button(class UIScreen* ui);
	~Button();

	void SetTexture(String texOff, String texOn);

	virtual void Draw();
	// Check if mouse cursor is in this button
	bool ContainsMouse();
	// Process function when clicked
	void OnClick();

	void SetOnClick(std::function<void()> func) { mOnClick = func; }
	Vec2 GetPosition() const { return mButtonRect.center(); }
	void SetPosition(int x, int y) { mButtonRect.x = x; mButtonRect.y = y; }
	void SetWidthHeight(int w, int h)
	{
		mButtonRect.w = w;
		mButtonRect.h = h;
	}
	bool GetIsSelected() const { return mIsSelected; }
	void SetIsSelected(bool selected) { mIsSelected = selected; }

private:
	std::function<void()> mOnClick;
	TextureAsset mTextureOn;
	TextureAsset mTextureOff;
	Rect mButtonRect;
	bool mIsSelected;
};

class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();

	virtual void Update();
	// Draw background, main text, and buttons
	virtual void Draw();
	void DrawButtons();
	void AddButton(class Button* button);
	// Reset all the buttons to the state unselected
	void ResetButtonState();

	enum class UIState
	{
		EActive,
		EPaused,
		EDead
	};
	// Close and free this UI
	void Close();

	UIState GetState() const { return mState; }

protected:
	class Game* mGame;
	UIState mState;

	TextureAsset mTexBackground;
	Rect mBackRect;


	Array<class Button*> mButtons;

private:
	Vec2 mPosition;
};

class LevelUpScreen : public UIScreen
{
public:
	LevelUpScreen(class Game* game, class UpgradeManager* um);
	~LevelUpScreen();
	void Draw();
private:
	class UpgradeManager* mUpManager;
	Array<class Upgrade*> mUpgrades;
};

class StartScreen : UIScreen
{
public:
	StartScreen(class Game* game);
	~StartScreen();
};

class SummaryScreen : UIScreen
{
public:
	SummaryScreen(class Game* game, bool isVictory);

	void Draw();
private:
	bool mIsVictory;

};
