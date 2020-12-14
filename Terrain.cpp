#include "Terrain.h"
#include "Shader.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>


bool Terrain::InitShader()
{
	//Init Shader	
	m_ProgramID = LoadShaders(".\\Resources\\Shaders\\TerrainVS.glsl", ".\\Resources\\Shaders\\TerrainPS.glsl");
	m_WorldID = glGetUniformLocation(m_ProgramID, "World");
	m_ViewID = glGetUniformLocation(m_ProgramID, "View");
	m_ProjectionID = glGetUniformLocation(m_ProgramID, "Projection");
	m_TextureID = glGetUniformLocation(m_ProgramID, "TerrainTexture");
	//m_LightSrcID = glGetUniformLocation(m_ProgramID, "LightPosition");
	//m_ModelColorID = glGetUniformLocation(m_ProgramID, "TerrainColor");

	return true;
}

bool Terrain::InitBuffer()
{
	const float pVertex[] = {
		 0.0f,  0.0f, 0.0f,
		 1.0f,  0.0f, 0.0f,
		 1.0f,  0.0f, 1.0f,

		 1.0f,  0.0f, 1.0f,
		 0.0f,  0.0f, 1.0f,
		 0.0f,  0.0f, 0.0f,

	};

	glGenBuffers(1, &m_pVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pVertex), pVertex, GL_STATIC_DRAW);

	const GLfloat pTextureUV[] = {
		 0.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f,  0.0f,

		 1.0f,  0.0f,
		 0.0f,  0.0f,
		 0.0f,  1.0f
	};
	glGenBuffers(1, &m_pTextureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_pTextureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pTextureUV), pTextureUV, GL_STATIC_DRAW);

	return true;
}

Terrain::Terrain(unsigned int _w, unsigned int _h)
	:m_uiWidth(_w),
	m_uiHeight(_h),
	m_ProgramID(0),
	m_WorldID(0),
	m_ViewID(0),
	m_ProjectionID(0),
	m_LightSrcID(0),
	m_TextureID(0),
	m_pVertexBuffer(0),
	m_pTextureBuffer(0),
	m_vPos(glm::vec3(-500.0f, 0.0f, -500.0f)),
	m_vRot(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_vScale(glm::vec3(1000.0f, 1000.0f, 1000.0f)),
	m_matWorld(glm::mat4(1.0))
{

}

Terrain::~Terrain()
{
}

bool Terrain::InitTerrain()
{

	/*for (unsigned int iRow = 0; iRow < m_uiWidth; iRow++)
	{
		for (unsigned int iCol = 0; iCol < m_uiHeight; iCol++)
		{

		}
	}*/
	bool flag = this->InitShader();
	flag = flag && this->InitBuffer();

	return flag;
}

void Terrain::Process(float _fTick)
{
	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0), m_vPos);
	glm::mat4 matRotate = glm::eulerAngleYXZ(m_vRot.y, m_vRot.x, m_vRot.z);
	glm::mat4 matScale = glm::scale(m_vScale);

	m_matWorld = matTranslation * matRotate * matScale;
}

void Terrain::Render(glm::mat4 _ViewMatrix, glm::mat4 _ProjMatrix, glm::vec3 _vCameraPos)
{
	glUseProgram(m_ProgramID);
	
	glUniformMatrix4fv(m_ViewID, 1, GL_FALSE, &_ViewMatrix[0][0]);
	glUniformMatrix4fv(m_ProjectionID, 1, GL_FALSE, &_ProjMatrix[0][0]);
	
	glm::mat4 matWorld = glm::mat4(1.0f);
	//glUniformMatrix4fv(m_WorldID, 1, GL_FALSE, &matWorld[0][0]);
	glUniformMatrix4fv(m_WorldID, 1, GL_FALSE, &m_matWorld[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_pTextureBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
}
