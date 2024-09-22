#pragma once

#include <RenderEngine/export.h>
#include <Math/Matrix.h>
#include <Math/Vector.h>

namespace GameEngine::Render
{
	namespace HAL
	{
		class Mesh;
		class Material;
	}

	class RENDER_ENGINE_API RenderObject final
	{
	public:
		using Ptr = std::shared_ptr<RenderObject>;

	public:
		RenderObject();

	public:
		// it is small, so no need in pointers i think
		GameEngine::Core::Math::Matrix4x4f matrix = GameEngine::Core::Math::Matrix4x4f::Identity();
		std::shared_ptr<HAL::Mesh> m_mesh;
		std::shared_ptr<HAL::Material> m_material;
	};
}