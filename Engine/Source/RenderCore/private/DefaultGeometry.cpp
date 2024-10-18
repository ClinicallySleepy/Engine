#include <DefaultGeometry.h>

namespace GameEngine
{
	namespace RenderCore
	{
		namespace DefaultGeometry
		{
			Geometry::Ptr Cube()
			{
				constexpr Core::array<Geometry::VertexType, 8> vertices =
				{
					Math::Vector3f(-1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, +1.0f, -1.0f),
					Math::Vector3f(+1.0f, -1.0f, -1.0f),
					Math::Vector3f(-1.0f, -1.0f, +1.0f),
					Math::Vector3f(-1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, +1.0f, +1.0f),
					Math::Vector3f(+1.0f, -1.0f, +1.0f)
				};

				constexpr Core::array<Geometry::IndexType, 36> indices =
				{
					// front face
					0, 1, 2,
					0, 2, 3,

					// back face
					4, 6, 5,
					4, 7, 6,

					// left face
					4, 5, 1,
					4, 1, 0,

					// right face
					3, 2, 6,
					3, 6, 7,

					// top face
					1, 5, 6,
					1, 6, 2,

					// bottom face
					4, 0, 3,
					4, 3, 7
				};

				return std::make_shared<Geometry>((Geometry::VertexType*)vertices.begin(), vertices.size(), (Geometry::IndexType*)indices.begin(), indices.size());
			}

			Geometry::Ptr CubeSphere(int subdivisions, float size)
			{
				using Vertex = Geometry::VertexType;

				std::vector<Vertex> vertices;
				std::vector<Geometry::IndexType> indices;

				constexpr Core::array<Vertex, 8> corners =
				{
					Vertex(-1.0f, -1.0f, -1.0f),
					Vertex(-1.0f, +1.0f, -1.0f),
					Vertex(+1.0f, +1.0f, -1.0f),
					Vertex(+1.0f, -1.0f, -1.0f),
					Vertex(-1.0f, -1.0f, +1.0f),
					Vertex(-1.0f, +1.0f, +1.0f),
					Vertex(+1.0f, +1.0f, +1.0f),
					Vertex(+1.0f, -1.0f, +1.0f)
				};

				constexpr Core::array<Core::array<int, 4>, 6> faces =
				{
					Core::array<int, 4>{ 0, 1, 2, 3 }, // Front face
					Core::array<int, 4>{ 7, 6, 5, 4 }, // Back face
					Core::array<int, 4>{ 4, 5, 1, 0 }, // Left face
					Core::array<int, 4>{ 3, 2, 6, 7 }, // Right face
					Core::array<int, 4>{ 1, 5, 6, 2 }, // Top face
					Core::array<int, 4>{ 4, 0, 3, 7 }  // Bottom face
				};

				auto Interpolate = [&](Vertex v0, Vertex v1, float t)
				{
					return v0 * (1.f - t) + v1 * t;
				};

				auto AddSubface = [&](int v0, int v1, int v2, int v3)
				{
					// First triangle
					indices.push_back(v2);
					indices.push_back(v1);
					indices.push_back(v0);

					// Second triangle
					indices.push_back(v3);
					indices.push_back(v2);
					indices.push_back(v0);
				};

				for (int face = 0; face < 6; ++face)
				{
					for (int row = 0; row <= subdivisions; ++row)
					{
						for (int column = 0; column <= subdivisions; ++column)
						{
							float u = row / static_cast<float>(subdivisions);
							float v = column / static_cast<float>(subdivisions);

							Vertex v0 = Interpolate(corners[faces[face][0]], corners[faces[face][1]], v);
							Vertex v1 = Interpolate(corners[faces[face][3]], corners[faces[face][2]], v);
							Vertex position = Interpolate(v0, v1, u);

							position = position.Normalized() * size;
							vertices.push_back(position);

							if (row < subdivisions && column < subdivisions)
							{
								int faceVertexCount = (subdivisions + 1) * (subdivisions + 1);
								int baseIndex = face * faceVertexCount;

								int current = baseIndex + row * (subdivisions + 1) + column;
								int next = current + 1;
								int below = current + (subdivisions + 1);
								int belowNext = below + 1;

								AddSubface(current, below, belowNext, next);
							}

						}
					}
				}

				return std::make_shared<Geometry>(
					static_cast<Vertex*>(vertices.data()), vertices.size(), static_cast<Geometry::IndexType*>(indices.data()), indices.size());

			}
		}
	}
}
