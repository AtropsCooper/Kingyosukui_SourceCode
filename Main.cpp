# include <Siv3D.hpp> // Siv3D v0.6.12
# include "Game.h"

void Main()
{
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	Scene::SetTextureFilter(TextureFilter::Nearest);
	Scene::Resize(320, 480);
	Window::Resize(480, 720);
	Window::SetTitle(U"金魚すくい");

	Game game;

	while (System::Update())
	{
		if (game.GetGameState() == Game::ERunning)
		{
			game.UpdateGame();
		}
		game.UpdateUI();
		game.GenerateOutput();
	}
}
