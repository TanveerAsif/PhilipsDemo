#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




glm::vec3 Camera::GetLeft()
{
	glm::vec3 left = glm::cross(m_vFrwdDir, m_vUp);
	left = glm::normalize(left);	
	return left;
}

glm::vec3 Camera::GetRight()
{
	glm::vec3 right = glm::cross(m_vUp, m_vFrwdDir);
	right = glm::normalize(right);
	return right;
}




Camera::Camera()
	:m_vPos(0.0f, 6.0f, -22.0f),
	m_vFrwdDir(0, 0, 1),
	m_vUp(0, 1, 0),
	m_fSpeed(10.0f),
	m_fRoll(0.0f), 
	m_fPitch(0.0f), 
	m_fYaw(0.0f),
	m_fRotSpeed(1.0f),
	m_matView(glm::mat4(1.0f))
{	
	glm::vec3 vDir;
	vDir.x = cosf(m_fPitch) * sinf(m_fYaw);
	vDir.y = sinf(m_fPitch);
	vDir.z = cosf(m_fPitch) * cosf(m_fYaw);

	glm::vec2  vFrwdDir = glm::vec2(vDir.x, vDir.z);
	vFrwdDir = glm::normalize(vFrwdDir); 
	m_vFrwdDir = glm::vec3(vFrwdDir.x, 0.0f, vFrwdDir.y);
	//m_vFrwdDir = vDir;


	glm::vec3 vTarget = m_vPos + m_vFrwdDir;	
	m_matView = glm::lookAt(m_vPos, vTarget, m_vUp);
}

Camera::~Camera()
{

}

glm::vec3 Camera::GetPos()
{
	return m_vPos;
}

void Camera::SetPos(glm::vec3 _vPos)
{
	m_vPos = _vPos;
}

void Camera::Process(GLFWwindow* _pWindow, double _fTick)
{
	
	glm::vec3 right = GetRight();
	
	//Speed Up with Control Key
	m_fSpeed = 10.0f;
	if ((glfwGetKey(_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
		m_fSpeed = 25.0f;

	// Move Up
	if (glfwGetKey(_pWindow, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		m_vPos += m_vUp * (float)_fTick * m_fSpeed;
	}

	// Move Down
	if (glfwGetKey(_pWindow, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		m_vPos -= m_vUp * (float)_fTick * m_fSpeed;
	}

	// Move forward
	if (glfwGetKey(_pWindow, GLFW_KEY_UP) == GLFW_PRESS) {
		m_vPos += m_vFrwdDir * (float)_fTick * m_fSpeed;
	}

	// Move backward
	if (glfwGetKey(_pWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_vPos -= m_vFrwdDir * (float)_fTick * m_fSpeed;
	}

	// Strafe right
	if ((glfwGetKey(_pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
		&& ((glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) != GLFW_PRESS) || (glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) != GLFW_PRESS))) {
		m_vPos -= right * (float)_fTick * m_fSpeed;
	}

	// Strafe left
	if ((glfwGetKey(_pWindow, GLFW_KEY_LEFT) == GLFW_PRESS) 
		&& ((glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) != GLFW_PRESS) || (glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) != GLFW_PRESS))) {
		m_vPos += right * (float)_fTick * m_fSpeed;
	}

	//CTRL + LEFT : CLOCKWISE ROTATION
	if (((glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) || (glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS))
		&& (glfwGetKey(_pWindow, GLFW_KEY_LEFT) == GLFW_PRESS)) {
		m_fYaw += 0.025f * m_fRotSpeed;
	}

	//CTRL + RIGHT : ANTI-CLOCKWISE ROTATION
	if (((glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) || (glfwGetKey(_pWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) 
		&& (glfwGetKey(_pWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
		m_fYaw -= 0.025f * m_fRotSpeed;
	}

	glm::vec3 vDir;
	vDir.x = cosf(m_fPitch) * sinf(m_fYaw);
	vDir.y = sinf(m_fPitch);
	vDir.z = cosf(m_fPitch) * cosf(m_fYaw);

	glm::vec2  vFrwdDir = glm::vec2(vDir.x, vDir.z);
	vFrwdDir = glm::normalize(vFrwdDir);
	m_vFrwdDir = glm::vec3(vFrwdDir.x, 0.0f, vFrwdDir.y);
	

	glm::vec3 vTarget = m_vPos + m_vFrwdDir;
	m_matView = glm::lookAt(m_vPos, vTarget, m_vUp);
}
