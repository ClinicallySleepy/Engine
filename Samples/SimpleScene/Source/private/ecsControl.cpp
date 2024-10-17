#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsPhys.h>
#include <flecs.h>
#include <Input/Controller.h>
#include <Input/InputHandler.h>
#include <Vector.h>
#include <DefaultGeometry.h>
#include <RenderObject.h>

using namespace GameEngine;

void RegisterEcsControlSystems(flecs::world& world)
{
	world.system<Position, CameraPtr, const Speed, const ControllerPtr>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller)
	{
		Math::Vector3f currentMoveDir = Math::Vector3f::Zero();
		if (controller.ptr->IsPressed("GoLeft"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoRight"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetRightDir();
		}
		if (controller.ptr->IsPressed("GoBack"))
		{
			currentMoveDir = currentMoveDir - camera.ptr->GetViewDir();
		}
		if (controller.ptr->IsPressed("GoForward"))
		{
			currentMoveDir = currentMoveDir + camera.ptr->GetViewDir();
		}
		position.x = position.x + currentMoveDir.Normalized().x * speed.value * world.delta_time();
		position.y = position.y + currentMoveDir.Normalized().y * speed.value * world.delta_time();
		position.z = position.z + currentMoveDir.Normalized().z * speed.value * world.delta_time();
		camera.ptr->SetPosition(Math::Vector3f(position.x, position.y, position.z));
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon)
		{
			if (controller.ptr->IsPressed("Jump"))
			{
				vel.y = jump.value;
			}
		}
	});

	world.system<Position, CameraPtr, const Speed, const ControllerPtr, Cooldown>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller, Cooldown& cooldown)
	{
		if (cooldown.value < 0.f && controller.ptr->IsPressed("Jump"))
		{
			float initialSpeed = 20.f;
			cooldown.value = 0.5f;

			flecs::entity projectile = world.entity()
				.set(Position{ position })
				.set(Velocity{ camera.ptr->GetViewDir().x * initialSpeed, camera.ptr->GetViewDir().y * initialSpeed, camera.ptr->GetViewDir().z * initialSpeed })
				.set(Gravity{ 0.f, -9.8065f, 0.f })
				.set(BouncePlane{ 0.f, 1.f, 0.f, 5.f })
				.set(Bounciness{ 0.3f })
				.set(TimeToLive{ 5.f })
				.set(Projectile{ true })
				.set(EntitySystem::ECS::GeometryPtr{ RenderCore::DefaultGeometry::Cube() })
				.set(EntitySystem::ECS::RenderObjectPtr{ new Render::RenderObject() });
		}
		cooldown.value -= world.delta_time();
	});
}

