#include "QuadRenderer.h"
#include "Transform.h"
#include "GUI.h"

namespace GameEngine
{
	QuadRenderer::~QuadRenderer()
	{}
	/*
	* initialize function called from component, creats a quad (two triangles) and attaches the orthographic camera
	*/
	void QuadRenderer::initialize()
	{
		std::shared_ptr<Renderer::Vbo> pos = std::make_shared<Renderer::Vbo>();
		pos->add(glm::vec3(-0.5f, 0.5f, 0.0f));
		pos->add(glm::vec3(-0.5f, -0.5f, 0.0f));
		pos->add(glm::vec3(0.5f, -0.5f, 0.0f));

		pos->add(glm::vec3(0.5f, -0.5f, 0.0f));
		pos->add(glm::vec3(0.5f, 0.5f, 0.0f));
		pos->add(glm::vec3(-0.5f, 0.5f, 0.0f));

		std::shared_ptr<Renderer::Vbo> coords = std::make_shared<Renderer::Vbo>();
		coords->add(glm::vec2(0.0f, 1.0f));
		coords->add(glm::vec2(0.0f, 0.0f));
		coords->add(glm::vec2(1.0f, 0.0f));
		coords->add(glm::vec2(1.0f, 0.0f));
		coords->add(glm::vec2(1.0f, 1.0f));
		coords->add(glm::vec2(0.0f, 1.0f));

		m_quad = std::make_shared<Renderer::Vao>();
		m_quad->addVbo(pos);
		m_quad->addVbo(coords);

		m_shader = std::make_shared<Renderer::Shader>("../Shaders/Orphographic/vertexShader.txt", "../Shaders/Orphographic/fragmentShader.txt");
	
		//Find all camera to attach main 3D cam
		std::vector<std::shared_ptr<Camera> > cameras;
		core().lock()->find<Camera>(cameras);
		for each (std::shared_ptr<Camera> cam in cameras)
		{
			if (cam->m_orthoCam == true)
			{
				m_camera = cam;
			}
		}
	}

	/*
	* renderes a quad based on the texture and size given from a gui component
	*/
	void QuadRenderer::renderQuad(GUI* _gui)
	{
		//Update Matrix
		_gui->transform().lock()->updateGUIMatrix();

		//call shader bind function and pass transform position
		m_shader->bindShader(m_camera.lock()->getProj(), "u_Projection");
		m_shader->bindShader(_gui->transform().lock()->getModelMatrix(), "u_Model");
		
		//render quad
		m_shader->renderQuad(m_quad, _gui->getTexture());
	}

}