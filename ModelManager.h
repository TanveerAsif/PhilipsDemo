#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Model;
class ModelManager
{

private:
	bool InitShader();
	std::vector<Model* > m_vModels;

protected:
	GLuint m_ProgramID, m_WorldID, m_ViewID, m_ProjectionID, m_LightSrcID, m_TextureID;
	GLuint m_ModelColorID;

public:
	ModelManager();
	~ModelManager();

	bool Initialize();
	void Process(GLFWwindow* _pWindow, float _fTick);
	void Render(glm::mat4 _ViewMatrix, glm::mat4 _ProjMatrix, glm::vec3 _vCameraPos);
};

