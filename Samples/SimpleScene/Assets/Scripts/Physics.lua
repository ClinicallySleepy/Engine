local ecs = require "ecs"

local function rand_flt(from, to)
	local maxNumber = 32767
    return from + (math.random(maxNumber) / maxNumber) * (to - from)
end

local function move(it)
    for pos, vel, ent in ecs.each(it) do
        pos.x = pos.x + vel.x * it.delta_time
        pos.y = pos.y + vel.y * it.delta_time
		pos.z = pos.z + vel.z * it.delta_time
    end
end

local function gravity(it)
    for pos, vel, grav, plane, ent in ecs.each(it) do
        local planeEpsilon = 0.1
		
		if plane.x * pos.x + plane.y * pos.y + plane.z * pos.z < plane.w + planeEpsilon then
			do return end
		end
		
		vel.x = vel.x + grav.x * it.delta_time
        vel.y = vel.y + grav.y * it.delta_time
		vel.z = vel.z + grav.z * it.delta_time
    end
end

local function FrictionSystem(it)
    for vel, friction, ent in ecs.each(it) do
        vel.x = vel.x - vel.x * friction.value * it.delta_time
		vel.y = vel.y - vel.y * friction.value * it.delta_time
		vel.z = vel.z - vel.z * friction.value * it.delta_time
    end
end

local function ShiverSystem(it)
    for pos, shiver, ent in ecs.each(it) do
        pos.x = pos.x + rand_flt(-shiver.value, shiver.value)
		pos.y = pos.y + rand_flt(-shiver.value, shiver.value)
		pos.z = pos.z + rand_flt(-shiver.value, shiver.value)
    end
end

local function BounceSystem(it)
    for pos, vel, plane, bounciness, ent in ecs.each(it) do
        local dotPos = plane.x * pos.x + plane.y * pos.y + plane.z * pos.z
		local dotVel = plane.x * vel.x + plane.y * vel.y + plane.z * vel.z
		
		if dotPos < plane.w then
			pos.x = pos.x - (dotPos - plane.w) * plane.x
			pos.y = pos.y - (dotPos - plane.w) * plane.y
			pos.z = pos.z - (dotPos - plane.w) * plane.z

			vel.x = vel.x - (1.0 + bounciness.value) * plane.x * dotVel
			vel.y = vel.y - (1.0 + bounciness.value) * plane.y * dotVel
			vel.z = vel.z - (1.0 + bounciness.value) * plane.z * dotVel
		end
    end
end

local function DyingSystem(it)
    for time, pos, ent in ecs.each(it) do
        if time.value < 0 then
            pos.x = 100000
            ecs.delete(ent)
        else
            time.value = time.value - it.delta_time
        end
    end
end

-- basically same code, doesnt work, crush on query
-- https://github.com/flecs-hub/flecs-lua/blob/b8d95ef4b1e35bf72b5822986a81c858a147f1c9/test/query.lua#L68
local function BulletSystem(it)
    local targets = ecs.query("Target, Position, Velocity")

    for projectile, pos1, vel1, ent1 in ecs.each(it) do
        for target, pos2, vel2, ent2 in ecs.each(targets) do
            if (math.abs(pos1.x - pos2.x) < 1 and math.abs(pos1.y - pos2.y) < 1 and math.abs(pos1.z - pos2.z) < 1) then
                vel2.y = 10;
            end
        end
    end
end

-- same code, doesnt work
-- https://github.com/flecs-hub/flecs-lua/blob/b8d95ef4b1e35bf72b5822986a81c858a147f1c9/test/system.lua#L90
local function CollisionSystem(it)
    local collision, pos, vel = it.columns

    for i = 1, it.count do
        for j = i + 1, it.count do
            if (collision[i] == collision[j]
                and math.abs(pos[i].x - pos[j].x) < 1
                and math.abs(pos[i].y - pos[j].y) < 1
                and math.abs(pos[i].z - pos[j].z) < 1) then
                vel[i].x = 0
                vel[i].y = 0
                vel[i].z = 0
                vel[j].x = 0
                vel[j].y = 0
                vel[j].z = 0
            end
        end
    end
end

ecs.system(move, "Move", ecs.OnUpdate, "Position, Velocity")
ecs.system(gravity, "grav", ecs.OnUpdate, "Position, Velocity, Gravity, BouncePlane")
ecs.system(FrictionSystem, "FrictionSystem", ecs.OnUpdate, "Velocity, FrictionAmount")
ecs.system(ShiverSystem, "ShiverSystem", ecs.OnUpdate, "Position, ShiverAmount")
ecs.system(BounceSystem, "BounceSystem", ecs.OnUpdate, "Position, Velocity, BouncePlane, Bounciness")
ecs.system(DyingSystem, "DyingSystem", ecs.OnUpdate, "TimeToLive, Position")
ecs.system(BulletSystem, "BulletSystem", ecs.OnUpdate, "Projectile, Position, Velocity")
ecs.system(CollisionSystem, "CollisionSystem", ecs.OnUpdate, "Collision, Position, Velocity")

