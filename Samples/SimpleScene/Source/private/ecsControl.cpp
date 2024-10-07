#include <Camera.h>
#include <ecsControl.h>
#include <ECS/ecsSystems.h>
#include <ecsMesh.h>
#include <ecsPhys.h>
#include <ecsPlayer.h>
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
		float rotationY = 0;
		if (controller.ptr->IsPressed("TurnLeft"))
		{
			rotationY = rotationY - 1.f;
		}
		if (controller.ptr->IsPressed("TurnRight"))
		{
			rotationY = rotationY + 1.f;
		}
		// there is not matrix/rotation camera component, therefore no need to update it
		camera.ptr->Rotate(rotationY * world.delta_time(), 0);

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
		if (controller.ptr->IsPressed("GoUp"))
		{
			currentMoveDir = currentMoveDir + Math::Vector3f(0.0f, 1.0f, 0.0f);
		}
		if (controller.ptr->IsPressed("GoDown"))
		{
			currentMoveDir = currentMoveDir - Math::Vector3f(0.0f, 1.0f, 0.0f);
		}
		position.value = position.value + currentMoveDir.Normalized() * speed * world.delta_time();
		camera.ptr->SetPosition(position.value);
	});

	world.system<Position, CameraPtr, const Speed, const ControllerPtr, 
		const ReloadTimeLeft, FireCooldownLeft, const MaximumFireCooldown, ProjectilesLeft>()
		.each([&](flecs::entity e, Position& position, CameraPtr& camera, const Speed& speed, const ControllerPtr& controller,
			const ReloadTimeLeft& reloadLeft, FireCooldownLeft& cooldownLeft, const MaximumFireCooldown& cooldown, ProjectilesLeft& projectiles)
	{
		if (controller.ptr->IsPressed("Jump"))
		{
			if (projectiles.value > 0 && cooldownLeft.value == 0.f && reloadLeft.value == 0.f) {
				cooldownLeft.value = cooldown.value;
				projectiles.value -= 1;

				// tired of passing so many things to system
				float initialSpeed = 20.f;

				flecs::entity projectile = world.entity()
					.set(Position{ position })
					.set(Velocity{ camera.ptr->GetViewDir() * initialSpeed })
					.set(Gravity{ Math::Vector3f(0.f, -9.8065f, 0.f) })
					.set(BouncePlane{ Math::Vector4f(0.f, 1.f, 0.f, 5.f) })
					.set(Bounciness{ 0.3f })
					.set(Collision{ 1 })
					.set(FrictionAmount{ 0.9f })
					.set(TimeToLiveAfterStop{ 1.f })
					.set(GeometryPtr{ RenderCore::DefaultGeometry::CubeSphere(7, 0.2) })
					.set(RenderObjectPtr{ new Render::RenderObject() });
			}
		}

		cooldownLeft.value -= world.delta_time();

		if (cooldownLeft.value < 0.f)
		{
			cooldownLeft.value = 0;
		}
	});

	world.system<const Position, Velocity, const ControllerPtr, const BouncePlane, const JumpSpeed>()
		.each([&](const Position& pos, Velocity& vel, const ControllerPtr& controller, const BouncePlane& plane, const JumpSpeed& jump)
	{
		constexpr float planeEpsilon = 0.1f;
		if (plane.value.x * pos.value.x + plane.value.y * pos.value.y + plane.value.z * pos.value.z < plane.value.w + planeEpsilon)
		{
			//if (controller.ptr->IsPressed("Jump"))
			//{
			//	vel.value.y = jump.value;
			//}
		}
	});
}
