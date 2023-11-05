#include "stdafx.h"
#include "Game.h"
#include "Actor.h"
#include "ActorSpawner.h"
#include "Ship.h"
#include "Hand.h"
#include "Bird.h"
#include "Fish.h"
#include "UpgradeManager.h"
#include "UIScreen.h"
#include "Effects.h"
#include "Boss.h"
#include "MasterSpark.h"
#include "UIScreen.h"

//#define DEBUG


Game::Game()
	: mUpdatingActors(false)
	, mGameState(ERunning)
	, mActorSpawner(nullptr)
	, mShip(nullptr)
	, mHand(nullptr)
	, mHP(0)
{
	LoadData();
	mUpgradeManager = new UpgradeManager(this);
	NewGame();
}

void Game::UpdateGame()
{
	double deltaTime = Scene::DeltaTime();
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	for (auto pending : mPendingActors)
	{
		AddActor(pending);
	}
	mPendingActors.clear();

	Array<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors << actor;
		}
	}

	for (auto iter : deadActors)
	{
		delete iter;
	}

	mHP += mUpgradeManager->mHPReg * deltaTime;
	if (mStopwatch.sF() > 10.0 * 60.0)
	{
		new Boss(this, { 450, 60 });
		mStopwatch.reset();
	}
}

void Game::UpdateUI()
{
	if (!mUIStack.empty())
	{
		mUIStack.back()->Update();
	}
	// Free all dead UIs
	for (auto iter = mUIStack.begin(); iter != mUIStack.end(); )
	{
		if ((*iter)->GetState() == UIScreen::UIState::EDead)
		{
			delete *iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void Game::GenerateOutput()
{
	DrawBackground();

	for (auto actor : mActors)
	{
#ifdef DEBUG
		actor->DrawCollider();
#endif DEBUG
		actor->Draw();
	}
	// Update Effects
	mEffect.update();

	// HP Bar
	int hp_length = static_cast<int>(Max(Min(25.0, 25.0 * mHP / mUpgradeManager->mMaxHP), 0.0));
	Vector2D<int> hp_pos = static_cast<Vector2D<int>>(mShip->GetPosition()) + Vec2{ 0, -12 };
	Rect{ hp_pos.x - 19, hp_pos.y - 24, 29, 6 }.draw();
	Rect{ hp_pos.x - 18, hp_pos.y - 23, 27, 4 }.draw(Color(88, 83, 97));
	Rect{ hp_pos.x - 17, hp_pos.y - 22, hp_length, 2 }.draw(Palette::Crimson);

	// Exp Bar
	int exp_length = static_cast<int>( Min(316.0, 316.0 * mExp / mUpgradeManager->mExpNextLevel));
	Rect{ 0, 0, 320, 20 }.draw(Palette::White);
	Rect{ 1, 1, 318, 18 }.draw(Color(88,83,97));
	Rect{ 2, 2, exp_length, 16 }.draw(Color(138, 121, 194));
	FontAsset(U"font_12")(U"Exp").draw(Vec2(6, 6));
	FontAsset(U"font_12")(U"Lv.{}"_fmt(mLevel)).draw(Arg::topRight(314, 6));

	// Draw Border
	//TextureAsset(U"fg_border").draw(Arg::topLeft(0, 0));
	for (auto ui : mUIStack)
	{
		ui->Draw();
	}
}

void Game::DrawBackground()
{
	TextureAsset(U"bg_sky").draw(Arg::topLeft(Vec2::Zero()));
	TextureAsset(U"bg_cloud").draw(Arg::bottomLeft(0, 320));
	AnimSeaWave[static_cast<int>(Scene::Time() / 0.15) % 6].draw(Arg::bottomLeft(0, 480));
	TextureAsset(U"bg_sand").draw(Arg::bottomLeft(0, 480));
}

void Game::AddActor(Actor* actor)
{
	// If we're updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors << actor;
	}
	else
	{
		int myOrder = actor->GetDrawOrder();
		auto iter = mActors.begin();
		for (; iter != mActors.end(); iter++)
		{
			if (myOrder < (*iter)->GetDrawOrder())
			{
				break;
			}
		}
		mActors.insert(iter, actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		mPendingActors.erase(iter);
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}
}

void Game::NewGame()
{
	AudioAsset(U"audio_waves").play();
	mUpgradeManager->Reset();
	mHP = mUpgradeManager->mMaxHP;
	mLevel = 0;
	mExp = 0;
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	mActorSpawner = new ActorSpawner(this);
	mShip = new Ship(this);
	mHand = new Hand(this);
	mHand->Update(Scene::DeltaTime());
	mStopwatch.reset();
	mStopwatch.start();
	new StartScreen(this);
}

void Game::GainExp(int value)
{
	mExp += value;
	if (mExp >= mUpgradeManager->mExpNextLevel)
	{
		LevelUp();
	}
}

void Game::LevelUp()
{
	AudioAsset(U"audio_levelup").play();
	mLevel += 1;
	mExp -= mUpgradeManager->mExpNextLevel;
	new LevelUpScreen(this, mUpgradeManager);
}

void Game::TakeDamage(double dmg)
{
	mHP -= dmg;
	if (dmg < 0)
	{
		AudioAsset(U"audio_hurt").playOneShot();
	}
	if (mHP <= 0)
	{
		new SummaryScreen(this, false);
		//Print << U"You Died!";
	}
	if (mHP > mUpgradeManager->mMaxHP)
	{
		mHP = mUpgradeManager->mMaxHP;
	}
}

void Game::LoadData()
{
	TextureAsset::Register(U"bg_sky", Resource(U"resources/BG/Sky.png"));
	TextureAsset::Register(U"bg_cloud", Resource(U"resources/BG/Clouds.png"));
	TextureAsset::Register(U"bg_sand", Resource(U"resources/BG/Sands.png"));
	TextureAsset::Register(U"exp_orb", U"🧿"_emoji);
	TextureAsset::Register(U"ship", Resource(U"resources/Character/Boat.png"));
	TextureAsset::Register(U"sail", Resource(U"resources/Character/Sail.png"));
	TextureAsset::Register(U"sleeve", Resource(U"resources/Character/Cloth.png"));
	TextureAsset::Register(U"girl", Resource(U"resources/Character/Character.png"));
	TextureAsset::Register(U"arm", Resource(U"resources/Character/Arm.png"));
	TextureAsset::Register(U"hand", Resource(U"resources/Character/Hand.png"));

	TextureAsset::Register(U"pufferfish", Resource(U"resources/Fish/PufferFish/PufferFish-Animation1.png"));
	TextureAsset::Register(U"marlin", Resource(U"resources/Fish/Marlin.png"));
	TextureAsset::Register(U"tinfish", Resource(U"resources/Fish/TinFish.png"));
	TextureAsset::Register(U"goldfish", Resource(U"resources/Fish/GoldFish.png"));
	TextureAsset::Register(U"crab", Resource(U"resources/Fish/Crab.png"));

	TextureAsset::Register(U"bird", Resource(U"resources/Bird/Bird-Idle3.png"));
	TextureAsset::Register(U"ufo", Resource(U"resources/Bird2/Bird2-Charge1.png"));
	TextureAsset::Register(U"fireball", Resource(U"resources/FireBall.png"));

	TextureAsset::Register(U"ui_upscreen", Resource(U"resources/UI/Upgrade/NoteBook.png"));
	TextureAsset::Register(U"ui_upbuttonoff", Resource(U"resources/UI/Upgrade/ChoiceButton.png"));
	TextureAsset::Register(U"ui_upbuttonon", Resource(U"resources/UI/Upgrade/ChoiceButton-Selected.png"));

	TextureAsset::Register(U"ui_titlescreen", Resource(U"resources/UI/Title.png"));
	TextureAsset::Register(U"ui_titlebuttonon", Resource(U"resources/UI/TitleButtonOn.png"));
	TextureAsset::Register(U"ui_titlebuttonoff", Resource(U"resources/UI/TitleButtonOff.png"));

	TextureAsset::Register(U"ui_sumbuttonon", Resource(U"resources/UI/EndScene/EndSceneOn.png"));
	TextureAsset::Register(U"ui_sumbuttonoff", Resource(U"resources/UI/EndScene/EndSceneOff.png"));

	TextureAsset::Register(U"icon_bighand", Resource(U"resources/UI/Icon/BiggerHand.png"));
	TextureAsset::Register(U"icon_bait", Resource(U"resources/UI/Icon/FishBait.png"));
	TextureAsset::Register(U"icon_speedup", Resource(U"resources/UI/Icon/HandSpeedUp.png"));
	TextureAsset::Register(U"icon_heal", Resource(U"resources/UI/Icon/HPRecovery.png"));
	TextureAsset::Register(U"icon_reg", Resource(U"resources/UI/Icon/HpRecoverySpeedUp.png"));
	TextureAsset::Register(U"icon_maxhp", Resource(U"resources/UI/Icon/MaxHPUp.png"));
	TextureAsset::Register(U"icon_powerdown", Resource(U"resources/UI/Icon/PowerDown.png"));
	TextureAsset::Register(U"icon_powerup", Resource(U"resources/UI/Icon/PowerUp.png"));

	FontAsset::Register(U"font_title_upgrade", 24, Resource(U"resources/font.ttf"));
	FontAsset::Register(U"font_12", 12, Resource(U"resources/font.ttf"));
	FontAsset::Register(U"font_36", 36, Resource(U"resources/font.ttf"));

	AudioAsset::Register(U"audio_charge", Resource(U"resources/SFX/charge.wav"));
	AudioAsset::Register(U"audio_enemyvanish", Resource(U"resources/SFX/enemy_vanish.wav"));
	AudioAsset::Register(U"audio_fire", Resource(U"resources/SFX/fire.mp3"));
	AudioAsset::Register(U"audio_hurt", Resource(U"resources/SFX/hurt.mp3"));
	AudioAsset::Register(U"audio_kingyo", Resource(U"resources/SFX/kingyo.wav"));
	AudioAsset::Register(U"audio_launch", Resource(U"resources/SFX/launch.mp3"));
	AudioAsset::Register(U"audio_lazer", Resource(U"resources/SFX/lazer01.wav"));
	AudioAsset::Register(U"audio_levelup", Resource(U"resources/SFX/levelup.mp3"));
	AudioAsset::Register(U"audio_nep", Resource(U"resources/SFX/nep00.wav"));
	AudioAsset::Register(U"audio_tnt", Resource(U"resources/SFX/tnt.mp3"));
	AudioAsset::Register(U"audio_waves", Resource(U"resources/SFX/waves.mp3"));
	AudioAsset::Register(U"audio_get", Resource(U"resources/SFX/get.mp3"));
	AudioAsset::Register(U"audio_puffer", Resource(U"resources/SFX/firework_explode4.mp3"));

	AnimSeaWave[0] = Texture(Resource(U"resources/BG/Sea/SeaWave-Animation1.png"));
	AnimSeaWave[1] = Texture(Resource(U"resources/BG/Sea/SeaWave-Animation2.png"));
	AnimSeaWave[2] = Texture(Resource(U"resources/BG/Sea/SeaWave-Animation3.png"));
	AnimSeaWave[3] = Texture(Resource(U"resources/BG/Sea/SeaWave-Animation4.png"));
	AnimSeaWave[4] = Texture(Resource(U"resources/BG/Sea/SeaWave-Animation5.png"));
	AnimSeaWave[5] = Texture(Resource(U"resources/BG/Sea/SeaWave-Animation6.png"));

	AnimSplash[0] = Texture(Resource(U"resources/BG/Splash/Splash-Animation1.png"));
	AnimSplash[1] = Texture(Resource(U"resources/BG/Splash/Splash-Animation2.png"));
	AnimSplash[2] = Texture(Resource(U"resources/BG/Splash/Splash-Animation3.png"));
	AnimSplash[3] = Texture(Resource(U"resources/BG/Splash/Splash-Animation4.png"));

	AnimPufferfish[0] = Texture(Resource(U"resources/Fish/PufferFish/PufferFish-Animation1.png"));
	AnimPufferfish[1] = Texture(Resource(U"resources/Fish/PufferFish/PufferFish-Animation2.png"));
	AnimPufferfish[2] = Texture(Resource(U"resources/Fish/PufferFish/PufferFish-Animation3.png"));
	AnimPufferfish[3] = Texture(Resource(U"resources/Fish/PufferFish/PufferFish-Animation4.png"));
	AnimPufferfish[4] = Texture(Resource(U"resources/Fish/PufferFish/PufferFish-Animation5.png"));

	AnimFireBirdAttack[0] = Texture(Resource(U"resources/Bird/Bird-Attack1.png"));
	AnimFireBirdAttack[1] = Texture(Resource(U"resources/Bird/Bird-Attack2.png"));
	AnimFireBirdAttack[2] = Texture(Resource(U"resources/Bird/Bird-Attack3.png"));
	AnimFireBirdAttack[3] = Texture(Resource(U"resources/Bird/Bird-Attack4.png"));
	AnimFireBirdAttack[4] = Texture(Resource(U"resources/Bird/Bird-Attack5.png"));

	AnimFireBirdIdle[0] = Texture(Resource(U"resources/Bird/Bird-Idle3.png"));
	AnimFireBirdIdle[1] = Texture(Resource(U"resources/Bird/Bird-Idle4.png"));
	AnimFireBirdIdle[2] = Texture(Resource(U"resources/Bird/Bird-Winging1.png"));
	AnimFireBirdIdle[3] = Texture(Resource(U"resources/Bird/Bird-Winging2.png"));
	AnimFireBirdIdle[4] = Texture(Resource(U"resources/Bird/Bird-Winging3.png"));
	AnimFireBirdIdle[5] = Texture(Resource(U"resources/Bird/Bird-Winging4.png"));
	AnimFireBirdIdle[6] = Texture(Resource(U"resources/Bird/Bird-Winging5.png"));

	AnimLaserBirdCharge[0] = Texture(Resource(U"resources/Bird2/Bird2-Charge1.png"));
	AnimLaserBirdCharge[1] = Texture(Resource(U"resources/Bird2/Bird2-Charge2.png"));
	AnimLaserBirdCharge[2] = Texture(Resource(U"resources/Bird2/Bird2-Charge3.png"));
	AnimLaserBirdCharge[3] = Texture(Resource(U"resources/Bird2/Bird2-Charge4.png"));
	AnimLaserBirdCharge[4] = Texture(Resource(U"resources/Bird2/Bird2-Charge5.png"));
	AnimLaserBirdCharge[5] = Texture(Resource(U"resources/Bird2/Bird2-Charge6.png"));
	AnimLaserBirdCharge[6] = Texture(Resource(U"resources/Bird2/Bird2-Charge7.png"));
	AnimLaserBirdCharge[7] = Texture(Resource(U"resources/Bird2/Bird2-Charge8.png"));
	AnimLaserBirdCharge[8] = Texture(Resource(U"resources/Bird2/Bird2-Charge9.png"));
	AnimLaserBirdCharge[9] = Texture(Resource(U"resources/Bird2/Bird2-Charge10.png"));

	AnimLaserBirdIdle[0] = Texture(Resource(U"resources/Bird2/Bird2-Idle1.png"));
	AnimLaserBirdIdle[1] = Texture(Resource(U"resources/Bird2/Bird2-Idle2.png"));
	AnimLaserBirdIdle[2] = Texture(Resource(U"resources/Bird2/Bird2-Idle3.png"));
	AnimLaserBirdIdle[3] = Texture(Resource(U"resources/Bird2/Bird2-Idle4.png"));
	AnimLaserBirdIdle[4] = Texture(Resource(U"resources/Bird2/Bird2-Idle5.png"));

	AnimLaser[0] = Texture(Resource(U"resources/Bird2/Bird2-Laser1.png"));
	AnimLaser[1] = Texture(Resource(U"resources/Bird2/Bird2-Laser2.png"));
	AnimLaser[2] = Texture(Resource(U"resources/Bird2/Bird2-Laser3.png"));
	AnimLaser[3] = Texture(Resource(U"resources/Bird2/Bird2-Laser4.png"));

	AnimBoss[0] = Texture(Resource(U"resources/Boss/BirdBoss-Flying-Animation1.png"));
	AnimBoss[1] = Texture(Resource(U"resources/Boss/BirdBoss-Flying-Animation2.png"));
	AnimBoss[2] = Texture(Resource(U"resources/Boss/BirdBoss-Flying-Animation3.png"));
	AnimBoss[3] = Texture(Resource(U"resources/Boss/BirdBoss-Flying-Animation4.png"));
	AnimBoss[4] = Texture(Resource(U"resources/Boss/BirdBoss-Flying-Animation5.png"));
	AnimBoss[5] = Texture(Resource(U"resources/Boss/BirdBoss-Flying-BirdBoss-Idle.png"));

}

