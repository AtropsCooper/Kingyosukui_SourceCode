#include "stdafx.h"
#include "UIScreen.h"
#include "Game.h"
#include "Hand.h"
#include "UpgradeManager.h"

UIScreen::UIScreen(Game* game)
	: mGame(game)
	, mState(UIState::EActive)
	, mTexBackground(U"")
	, mBackRect(Rect{})
	, mButtons()
	, mPosition(Vec2::Zero())
{
	mGame->PushUI(this);
}

UIScreen::~UIScreen()
{
	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
}

void UIScreen::Update()
{

	if (!mButtons.empty())
	{
		// Check if there is button selected by mouse
		for (auto b : mButtons)
		{
			if (b->ContainsMouse())
			{
				b->SetIsSelected(true);
			}
			else
			{
				b->SetIsSelected(false);
			}
		}
	}
	// On click
	if (KeySpace.pressed() || KeyEnter.pressed() || MouseL.pressed())
	{
		if (!mButtons.empty())
		{
			for (auto b : mButtons)
			{
				// Click and execute the selected button
				if (b->GetIsSelected())
				{
					b->OnClick();
					break;
				}
			}
		}
	}
}

void UIScreen::Draw()
{
	// Draw shade
	Rect{ Scene::Size() }.draw(ColorF(0, 0.3));

	// Draw background
	if (!mTexBackground.isEmpty())
	{
		mTexBackground.resized(mBackRect.size).drawAt(mBackRect.center());
	}
	DrawButtons();
}

void UIScreen::DrawButtons()
{
	// Draw buttons
	for (auto b : mButtons)
	{
		b->Draw();
	}
}

void UIScreen::AddButton(Button* button)
{
	mButtons << button;
}

void UIScreen::ResetButtonState()
// Reset all the buttons to the state unselected
{
	for (auto b : mButtons)
	{
		b->SetIsSelected(false);
	}
}

void UIScreen::Close()
{
	mState = UIState::EDead;
}

Button::Button(UIScreen* ui)
	: mOnClick(nullptr)
	, mIsSelected(false)
	, mTextureOn(U"")
	, mTextureOff(U"")
	, mButtonRect()
{
	ui->AddButton(this);
}

Button::~Button()
{
}

void Button::SetTexture(String texOff, String texOn)
{
	mTextureOff = TextureAsset(texOff);
	mTextureOn = TextureAsset(texOn);
}

void Button::Draw()
{
	if (mIsSelected)
	{
		mTextureOn.resized(mButtonRect.size).drawAt(mButtonRect.center());
	}
	else
	{
		mTextureOff.resized(mButtonRect.size).drawAt(mButtonRect.center());
	}
}

bool Button::ContainsMouse()
// Check if mouse cursor is in this button
{
	return mButtonRect.mouseOver();
}

void Button::OnClick()
// Process function when clicked
{
	if (mOnClick)
	{
		mOnClick();
	}
}

LevelUpScreen::LevelUpScreen(Game* game, UpgradeManager* um)
	: UIScreen(game)
	, mUpManager(um)
{
	mGame->SetGameState(Game::EPaused);
	mGame->mStopwatch.pause();
	mBackRect = Rect{ 48, 59, 223, 340 };
	mTexBackground = TextureAsset(U"ui_upscreen"); 
	String off = U"ui_upbuttonoff";
	String on = U"ui_upbuttonon";

	for (auto i : step(3))
	{
		mUpgrades << mUpManager->GenerateUpgrade();
	}

	Button* b1 = new Button(this);
	b1->SetTexture(off, on);
	b1->SetWidthHeight(196, 58);
	b1->SetPosition( 61, 138 );
	b1->SetOnClick([this]() { mUpgrades[0]->ApplyUpgrade(); Close(); });

	Button* b2 = new Button(this);
	b2->SetTexture(off, on);
	b2->SetWidthHeight(196, 58);
	b2->SetPosition( 61, 219 );
	b2->SetOnClick([this]() { mUpgrades[1]->ApplyUpgrade(); Close(); });

	Button* b3 = new Button(this);
	b3->SetTexture(off, on);
	b3->SetWidthHeight(196, 58);
	b3->SetPosition( 61, 300 );
	b3->SetOnClick([this]() { mUpgrades[2]->ApplyUpgrade(); Close(); });

}

LevelUpScreen::~LevelUpScreen()
{
	if (mGame->GetHand()->GetHandState() == Hand::EStreching)
	{
		mGame->GetHand()->UpdateHand();
	}
	mGame->SetGameState(Game::ERunning);
	mGame->mStopwatch.pause();
}

void LevelUpScreen::Draw()
{
	UIScreen::Draw();
	FontAsset(U"font_title_upgrade")(U"UPGRADE").drawAt({ 160, 110 });
	if (mUpgrades.size() == 3)
	{
		for (auto i : step(3))
		{
			FontAsset(U"font_12")(mUpgrades[i]->GetTitle()).draw(Arg::leftCenter(104, 146 + i * 81 ));
			FontAsset(U"font_12")(mUpgrades[i]->GetDescription(0)).draw(Arg::leftCenter(104, 169 + i * 81 ));
			FontAsset(U"font_12")(mUpgrades[i]->GetDescription(1)).draw(Arg::leftCenter(104, 183 + i * 81));
			mUpgrades[i]->GetTexture().drawAt(79, 156 + i * 81); 
		}
	}
}

StartScreen::StartScreen(Game* game)
	: UIScreen(game)
{
	mGame->SetGameState(Game::EPaused);
	mGame->mStopwatch.pause();
	mTexBackground = TextureAsset(U"ui_titlescreen");
	mBackRect = Rect{ 0, 0, 320, 480 };
	String off = U"ui_titlebuttonoff";
	String on = U"ui_titlebuttonon";


	Button* b = new Button(this);
	b->SetTexture(off, on);
	b->SetWidthHeight(78, 27);
	b->SetPosition(120, 140);
	b->SetOnClick([this]() { Close(); });
}

StartScreen::~StartScreen()
{
	mGame->SetGameState(Game::ERunning);
	mGame->mStopwatch.resume();
}

SummaryScreen::SummaryScreen(Game* game, bool isVictory)
	: UIScreen(game)
	, mIsVictory(isVictory)
{
	mGame->SetGameState(Game::EPaused);
	mGame->mStopwatch.pause();

	String off = U"ui_sumbuttonoff";
	String on = U"ui_sumbuttonon";

	Button* b = new Button(this);
	b->SetTexture(off, on);
	b->SetWidthHeight(82, 24);
	b->SetPosition(119, 340);
	b->SetOnClick([this]() {  mGame->NewGame(); Close();  });
}

void SummaryScreen::Draw()
{
	UIScreen::Draw();
	if (mIsVictory)
	{
		 FontAsset(U"font_36")(U"Victory").drawAt(160, 60);
	}
	else
	{
		FontAsset(U"font_36")(U"Game Over").drawAt(160, 60);
	}

}
