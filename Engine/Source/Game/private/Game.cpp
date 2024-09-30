#include <Camera.h>
#include <DefaultGeometry.h>
#include <Game.h>
#include <GameObject.h>
#include <Input/InputHandler.h>

namespace GameEngine
{

	Game::Game(
		std::function<bool()> PlatformLoopFunc
	) :
		PlatformLoop(PlatformLoopFunc)
	{
		Core::g_MainCamera = new Core::Camera();
		Core::g_MainCamera->SetPosition(Math::Vector3f(0.0f, 6.0f, -6.0f));
		Core::g_MainCamera->SetViewDir(Math::Vector3f(0.0f, -6.0f, 6.0f).Normalized());

		m_renderThread = std::make_unique<Render::RenderThread>();

		Math::Vector2f from(0, 0), to(50, 50);
		Math::Vector3f startingPosition;

		for (int i = 0; i < 100; ++i)
		{
			startingPosition.x = (to.x - from.x) * std::rand() / RAND_MAX;
			startingPosition.z = (to.y - from.y) * std::rand() / RAND_MAX;
			int type = std::rand() % 3;

			m_Objects.push_back(new GameObject());

			switch (type)
			{
			case 0:
				startingPosition.y = 0;
				controllableObjects.push_back(m_Objects.back());
				break;
			case 1:
				startingPosition.y = 5.f;
				movingObjects.push_back(m_Objects.back());
				break;
			case 2:
				startingPosition.y = 10.f;
				physicObjects.push_back(m_Objects.back());
				break;
			default:
				break;
			}


			Render::RenderObject** renderObject = m_Objects.back()->GetRenderObjectRef();
			m_Objects.back()->SetPosition(startingPosition, 0);
			m_renderThread->EnqueueCommand(Render::ERC::CreateRenderObject, RenderCore::DefaultGeometry::Cube(), renderObject);
		}

		Core::g_InputHandler->RegisterCallback("GoForward", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoBack", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetViewDir()); });
		Core::g_InputHandler->RegisterCallback("GoRight", [&]() { Core::g_MainCamera->Move(Core::g_MainCamera->GetRightDir()); });
		Core::g_InputHandler->RegisterCallback("GoLeft", [&]() { Core::g_MainCamera->Move(-Core::g_MainCamera->GetRightDir()); });

		Core::g_InputHandler->RegisterCallback("MoveForward", [&]() { direction.z = 1.f; });
		Core::g_InputHandler->RegisterCallback("MoveBack", [&]() { direction.z = -1.f; });
		Core::g_InputHandler->RegisterCallback("MoveRight", [&]() { direction.x = 1.f; });
		Core::g_InputHandler->RegisterCallback("MoveLeft", [&]() { direction.x = -1.f; });
	}

	void Game::Run()
	{
		assert(PlatformLoop != nullptr);

		m_GameTimer.Reset();

		bool quit = false;
		while (!quit)
		{
			m_GameTimer.Tick();
			float dt = m_GameTimer.GetDeltaTime();

			Core::g_MainWindowsApplication->Update();
			Core::g_InputHandler->Update();
			Core::g_MainCamera->Update(dt);

			Update(dt);

			m_renderThread->OnEndFrame();

			// The most common idea for such a loop is that it returns false when quit is required, or true otherwise
			quit = !PlatformLoop();
		}
	}

	void Game::Update(float dt)
	{
		for (int i = 0; i < m_Objects.size(); ++i)
		{
			Math::Vector3f pos = m_Objects[i]->GetPosition();


			m_Objects[i]->SetPosition(pos, m_renderThread->GetMainFrame());
		}

		for (auto object : controllableObjects) {
			Math::Vector3f positionChange = direction.Normalized() * 5. * dt;
			Math::Vector3f position = object->GetPosition();
			object->SetPosition(position + positionChange, m_renderThread->GetMainFrame());
		}

		for (auto object : movingObjects) {
			//Math::Vector3f positionChange = Math::Vector3f(1.f * std::sinf(m_GameTimer.GetTotalTime()) * dt, 0.f, 1.f * std::cosf(m_GameTimer.GetTotalTime()) * dt);
			Math::Vector3f positionChange = Math::Vector3f(1.f * std::sinf(m_GameTimer.GetTotalTime()) * dt, 0.f, 0.f);
			Math::Vector3f position = object->GetPosition();
			object->SetPosition(position + positionChange, m_renderThread->GetMainFrame());
		}

		float gravitation = 1.f;

		for (auto object : physicObjects) {
			Math::Vector3f velocityChange = Math::Vector3f(0.f, -gravitation, 0.f);
			Math::Vector3f velocity = object->GetVelocity() + velocityChange * dt;
			Math::Vector3f position = object->GetPosition();

			if (position.y <= 10.f) {
				velocity.y = 3.f;
			}
			
			Math::Vector3f positionChange = velocity * dt;
			object->SetPosition(position + positionChange, m_renderThread->GetMainFrame());
			object->SetVelocity(velocity, m_renderThread->GetMainFrame());
		}

		direction = Math::Vector3f::Zero();
	}
}