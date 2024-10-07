#include <Camera.h>
#include <DefaultGeometry.h>
#include <ecsControl.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsPlayer.h>
#include <GameFramework/GameFramework.h>
#include <Input/Controller.h>
#include <RenderObject.h>

using namespace GameEngine;

void GameFramework::Init()
{
	RegisterEcsMeshSystems(m_World);
	RegisterEcsControlSystems(m_World);
	RegisterEcsPhysSystems(m_World);
	RegisterEcsPlayerSystems(m_World);

	//flecs::entity cubeControl = m_World.entity()
	//	.set(Position{ Math::Vector3f(-2.f, 0.f, 0.f) })
	//	.set(Velocity{ Math::Vector3f(0.f, 0.f, 0.f) })
	//	.set(Speed{ 10.f })
	//	.set(FrictionAmount{ 0.9f })
	//	.set(JumpSpeed{ 10.f })
	//	.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
	//	.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
	//	.set(Bounciness{ 0.3f })
	//	.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
	//	.set(RenderObjectPtr{ new Render::RenderObject() })
	//	.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	//flecs::entity cubeMoving = m_World.entity()
	//	.set(Position{ Math::Vector3f(5.f, 0.f, 0.f) })
	//	.set(Velocity{ Math::Vector3f(0.f, 4.f, 0.f) })
	//	.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
	//	.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
	//	.set(Bounciness{ 1.f })
	//	.set(GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
	//	.set(RenderObjectPtr{ new Render::RenderObject() });

	flecs::entity player = m_World.entity("player")
		.set(ReloadTimeLeft{ 0.f })
		.set(MaximumReloadTime{ 6.f })
		.set(FireCooldownLeft{ 0.f })
		.set(MaximumFireCooldown{ 0.2f })
		.set(ProjectilesLeft{ 6 })
		.set(MaximumProjectiles{ 6 })
		.set(Position{ Math::Vector3f(0.0f, 12.0f, -10.0f) })
		.set(Speed{ 10.f })
		.set(CameraPtr{ Core::g_MainCamera })
		.set(ControllerPtr{ new Core::Controller(Core::g_FileSystem->GetConfigPath("Input_default.ini")) });

	for (int i = 0; i < 10; ++i) {
		flecs::entity target = m_World.entity()
			.set(Position{ Math::Vector3f(i * 4.f - 20.f, 0.f, 10.f) })
			.set(Velocity{ Math::Vector3f(0.f, 6.f, 0.f) })
			.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
			.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
			.set(Bounciness{ 1.f })
			.set(Collision{ 2 })
			.set(GeometryPtr{ RenderCore::DefaultGeometry::CubeSphere(7, 2.f) })
			.set(RenderObjectPtr{ new Render::RenderObject() });
	}

	for (int i = 0; i < 10; ++i) {
		flecs::entity target = m_World.entity()
			.set(Position{ Math::Vector3f(i * 4.f - 20.f, 0.f, 0.f) })
			.set(Velocity{ Math::Vector3f(0.f, 6.f, 0.f) })
			.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
			.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
			.set(Bounciness{ 1.f })
			.set(Collision{ 2 })
			.set(Increase{ 1 })
			.set(GeometryPtr{ RenderCore::DefaultGeometry::CubeSphere(7, 2.f) })
			.set(RenderObjectPtr{ new Render::RenderObject() });
	}
}

void GameFramework::Update(float dt)
{

}