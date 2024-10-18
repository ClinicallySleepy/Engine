#pragma once

#include <array.h>
#include <Geometry.h>
#include <Vector.h>
#include <Constants.h>

namespace GameEngine
{
	namespace RenderCore
	{
		namespace DefaultGeometry
		{
			RENDER_CORE_API Geometry::Ptr Cube();
			RENDER_CORE_API Geometry::Ptr CubeSphere(int subdivisions, float size);
		}
	}
}