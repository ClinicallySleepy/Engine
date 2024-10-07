#include <ecsPlayer.h>
#include <ecsPhys.h>
#include <ecsMesh.h>
#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <RenderThread.h>
#include <RenderObject.h>
#include <DefaultGeometry.h>
#include <Vector.h>
#include <Debug/Console.h>

using namespace GameEngine;

void RegisterEcsPlayerSystems(flecs::world& world)
{
	static const EntitySystem::ECS::RenderThreadPtr* renderThread = world.get<EntitySystem::ECS::RenderThreadPtr>();

	world.system<Velocity, TimeToLiveAfterStop, RenderObjectPtr>()
		.each([&](flecs::entity e, const Velocity& velocity, TimeToLiveAfterStop& ttl, RenderObjectPtr renderObjectComponent)
	{

		if (velocity.value.GetLength() == 0)
		{
			ttl.value -= world.delta_time();
		}

		if (ttl.value <= 0)
		{
			e.destruct();
			// i have no idea how to do this via component's destructor, world variable required on a component
			// also placeholder geometry is required due to the fact that i was unable to overload EnqueueCommand function template
			renderThread->ptr->EnqueueCommand(Render::ERC::RemoveRenderObject, RenderCore::DefaultGeometry::Cube(), renderObjectComponent.ptr);
		}
	});

	world.system<ReloadTimeLeft, MaximumReloadTime, ProjectilesLeft, MaximumProjectiles>()
		.each([&](flecs::entity e, ReloadTimeLeft& reloadLeft, const MaximumReloadTime& maximumReload, ProjectilesLeft& projectilesLeft, const MaximumProjectiles& maximumProjectiles)
	{
		if (projectilesLeft.value == 0.f && reloadLeft.value == 0.f)
		{
			reloadLeft.value = maximumReload.value;
		}

		if (reloadLeft.value != 0.f)
		{
			reloadLeft.value = std::max(reloadLeft.value - world.delta_time(), 0.f);
		}

		if (projectilesLeft.value == 0.f && reloadLeft.value == 0.f)
		{
			projectilesLeft.value = maximumProjectiles.value;
		}
	});

	world.system<const Collision, const Position>()
		.each([&](flecs::entity e1, const Collision& collision1, const Position& position1)
	{
		std::vector<flecs::entity> entities;

		world.each([&](flecs::entity e2, const Collision& collision2, const Position& position2)
		{
			if (e1 == e2)
			{
				return;
			}
			if (collision1.value == 1 && collision2.value == 2 || collision1.value == 2 && collision2.value == 1)
			{
				float distance = (static_cast<Math::Vector3f>(position1.value) - static_cast<Math::Vector3f>(position2.value)).GetLength();

				if (distance < 2.2f)
				{
					entities.push_back(e1);
				}
			}
		});

		for (flecs::entity entity : entities)
		{
			entity.destruct();
			if (entity.has<RenderObjectPtr>())
			{
				renderThread->ptr->EnqueueCommand(Render::ERC::RemoveRenderObject, RenderCore::DefaultGeometry::Cube(), entity.get<RenderObjectPtr>()->ptr);
			}
			if (entity.has<Increase>())
			{
				int currentProjectiles = world.entity("player").get<MaximumProjectiles>()->value;
				world.entity("player").set<MaximumProjectiles>({ currentProjectiles + entity.get<Increase>()->value });
			}
		}
	});
}
