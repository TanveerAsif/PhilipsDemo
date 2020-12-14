#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct stVertex
{
    glm::vec3 pos;
    glm::vec2 tex;
    //glm::vec3 normal;
};

struct stMesh
{
    std::string sMeshName;
    GLuint vertexPosBuffer;
    GLuint textureUVBuffer;
    GLuint normalBuffer;
    unsigned int uMaterialIndex;
    std::vector<glm::vec3> vVertexPos;
    std::vector<glm::vec2> vVertexUV;
    std::vector<glm::vec3> vVertexNormal;
};