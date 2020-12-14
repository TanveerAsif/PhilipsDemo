#pragma once
/**
* Class responsible for loading 3D Model
* Using Assimp Library for parsing Model File
* 
*/
#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Common.h"

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


class Texture;
class Model
{

private:    
    glm::vec3 m_vPos;
    glm::vec3 m_vRot;
    glm::vec3 m_vScale;

    glm::vec3 m_vColor;

    glm::mat4 m_matWorld;

    // the global Assimp scene object
    const aiScene* m_pScene = NULL;    
    std::vector<stMesh*> m_vMeshes;
    std::vector<Texture *> m_vTexture;

    bool m_bEnableRotation;
    float m_fRotSpeed, m_fRotRadius;

    bool m_bEnableCircleRotation;
    float m_fCircleAngle, m_fCircleRadius;    

    bool m_bVisiblity;
public:
    Model(glm::vec3 _vPos, glm::vec3 _vRot, glm::vec3 _vScale);
    ~Model();
    bool LoadMesh(const std::string& Filename);
    void Process(float _fTick);    
    std::vector<stMesh*> GetMeshList() { return m_vMeshes;}
    Texture* GetTexture(unsigned int _iTextureIndex);

    glm::mat4 GetTransformation() { return m_matWorld; }
    glm::vec3 GetColor() { return m_vColor; }
    void SetColor(glm::vec3 _vColor);
    void SetRotSpeed(float _fSpeed);
    void SetRotRadius(float _fRadius);
    void EnableRotation(bool _enable);
    void EnableCircleRotation(bool _enable);

    bool GetVisiblity() { return m_bVisiblity; }
    void SetVisiblity(bool _bVisiblity);
};
