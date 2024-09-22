#include <Game.h>

namespace GameEngine
{
	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		m_renderEngine = std::make_unique<Render::RenderEngine>();
		
		ProcessSystemParams();
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		bool quit = false;
		while (!quit)
		{
			start = std::chrono::steady_clock::now();
			quit = !PlatformLoop();

			ProcessSystemParams();

			std::chrono::duration<double> elapsed = start - end;
			m_renderEngine->Update(elapsed.count());
			end = start;
		}
	}

	void Game::ProcessSystemParams()
	{
		if (m_renderEngine->GetSwapChainWidth() != Core::MainWindowsApplication->GetWidth() ||
			m_renderEngine->GetSwapChainHeight() != Core::MainWindowsApplication->GetHeight()) [[unlikely]]
		{
			m_renderEngine->OnResize(Core::MainWindowsApplication->GetWidth(), Core::MainWindowsApplication->GetHeight());
		}
	}
}