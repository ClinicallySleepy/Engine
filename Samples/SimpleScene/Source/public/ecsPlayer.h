#pragma once

#include <flecs.h>

struct ReloadTimeLeft
{
	float value;
};

struct MaximumReloadTime
{
	float value;
};

struct FireCooldownLeft
{
	float value;
};

struct MaximumFireCooldown
{
	float value;
};

struct ProjectilesLeft
{
	int value;
};

struct MaximumProjectiles
{
	int value;
};

struct TimeToLiveAfterStop
{
	float value;
};

struct Increase
{
	int value;
};

void RegisterEcsPlayerSystems(flecs::world& world);

