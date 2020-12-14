#pragma once
/**
* Class responsible for Camera operation like 
* move(Frwd/Backwrd/Left/Right/Up/Down)
* rotate(Left and Right rotate)  
*/

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{

protected:
	glm::vec3 m_vPos, m_vFrwdDir, m_vUp;
	glm::mat4 m_matView;

	float m_fSpeed, m_fRotSpeed;
	float m_fRoll, m_fPitch, m_fYaw;

	glm::vec3 GetLeft();
	glm::vec3 GetRight();
	
public:
	Camera();
	~Camera();

	glm::vec3 GetPos();
	void SetPos(glm::vec3 _vPos);
	glm::mat4 GetCameraMatrix() { return m_matView; };
	void Process(GLFWwindow* _pWindow, double _fTick);
};

