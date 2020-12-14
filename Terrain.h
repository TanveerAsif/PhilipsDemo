#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


class Terrain
{

private:
    unsigned int            m_uiWidth, m_uiHeight;
    unsigned int			m_pVertexBuffer;
    unsigned int			m_pTextureBuffer;

    glm::vec3 m_vPos;
    glm::vec3 m_vRot;
    glm::vec3 m_vScale;

    glm::mat4 m_matWorld;

    GLuint m_ProgramID, m_WorldID, m_ViewID, m_ProjectionID, m_LightSrcID, m_TextureID;

    bool InitShader();
    bool InitBuffer();

public:
    Terrain(unsigned int _w, unsigned int _h);
    ~Terrain();
    
    bool InitTerrain();
    void Process(float _fTick);
    void Render(glm::mat4 _ViewMatrix, glm::mat4 _ProjMatrix, glm::vec3 _vCameraPos);
};

