#include <RenderEngine.h>
#include <RHIHelper.h>
#include <Mesh.h>

namespace GameEngine::Render
{
	RenderEngine::RenderEngine()
	{
		m_rhi = HAL::RHIHelper::CreateRHI("D3D12");
		m_rhi->Init();

		// not able to draw multiple objects somehow
		//RenderObject::Ptr box = std::make_shared<RenderObject>();
		//m_RenderObjects.push_back(box);
		//box->m_mesh = m_rhi->CreateBoxMesh();
		//box->m_material = m_rhi->GetMaterial(box->m_mesh->GetName());

		RenderObject::Ptr piramid = std::make_shared<RenderObject>();
		m_RenderObjects.push_back(piramid);
		piramid->m_mesh = m_rhi->CreatePiramidMesh();
		piramid->m_material = m_rhi->GetMaterial(piramid->m_mesh->GetName());
		
		
		m_rhi->ExecuteCommandLists();
		m_rhi->Flush();
	}

	void RenderEngine::Update(double dt)
	{
		float speed = 0.1f;

		for (int i = 0; i < m_RenderObjects.size(); ++i)
		{
			auto renderObject = m_RenderObjects[i];

			if (i == 0)
			{
				renderObject->matrix.SetElement(renderObject->matrix.GetElement(3, 0) + speed * dt, 3, 0);
				renderObject->matrix.SetElement(renderObject->matrix.GetElement(3, 1) + speed * dt, 3, 1);
			}
			else
			{
				renderObject->matrix.SetElement(renderObject->matrix.GetElement(3, 0) + speed * dt, 3, 0);

			}

			m_rhi->Update(renderObject->m_mesh, renderObject->m_material, renderObject->matrix);
		}

		m_rhi->Flush();
	}

	void RenderEngine::OnResize(uint16_t width, uint16_t height)
	{
		m_swapChainWidth = width;
		m_swapChainHeight = height;
		m_rhi->OnResize();
	}
}