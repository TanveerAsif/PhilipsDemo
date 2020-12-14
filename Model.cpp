#include "Model.h"
#include <GL/glew.h>
#include "Texture.h"

#include <fstream>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/compatibility.hpp>

Model::Model(glm::vec3 _vPos, glm::vec3 _vRot, glm::vec3 _vScale)
    :m_vPos(_vPos),
    m_vRot(_vRot),
    m_vScale(_vScale),
    m_vColor(0.0f, 0.0f, 0.0f),
    m_fRotSpeed(1.0f),
    m_fRotRadius(1.0f),
    m_matWorld(glm::mat4(1.0)),
    m_bEnableRotation(false),
    m_bEnableCircleRotation(false),
    m_fCircleAngle(0.1f), //Angle in Degree
    m_fCircleRadius(300.0f),
    m_bVisiblity(true)
{

}


Model::~Model()
{
    //delete 
    
    //1- m_pScene

    //for i to n
    //2- m_vTexture
    //3- m_vMeshes
}


bool Model::LoadMesh(const std::string& Filename)
{   
    std::string sResourceDir = ".\\Resources\\Models\\";
    std::string sModelDir = Filename + "\\";
    std::string sModelName = Filename + ".obj";

    std::string sModelPath = sResourceDir + sModelDir + sModelName;
    //check if file exists
    std::ifstream fin(sModelPath.c_str());
    if (!fin.fail()) 
        fin.close();
    else 
        return false;

    Assimp::Importer Importer;
    m_pScene = Importer.ReadFile(sModelPath.c_str(),
        aiProcess_Triangulate    | 
        aiProcess_GenSmoothNormals | 
        aiProcess_FlipUVs | 
        aiProcess_JoinIdenticalVertices);

    if (m_pScene)
    {   
        printf("\n\nModel Loaded: %s", Filename.c_str());
        //Read Meshes and Create Vertex Buffer for each mesh
        for(unsigned int i = 0; i < m_pScene->mNumMeshes; i++)
        {
            aiMesh* mesh = m_pScene->mMeshes[i];       

            auto pMesh = new stMesh();
            pMesh->sMeshName =  mesh->mName.data;
            pMesh->uMaterialIndex = mesh->mMaterialIndex;
            for (unsigned int j = 0; j < mesh->mNumFaces; j++)
            {
                const aiFace& face = mesh->mFaces[j];
                for (unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++)
                {
                    aiVector3D pos = mesh->mVertices[face.mIndices[k]];
                    aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[k]];
                    aiVector3D normal = mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]] : aiVector3D(1.0f, 1.0f, 1.0f);
                    
                    //auto vertex = new stVertex();
                    stVertex vertex;
                    vertex.pos = glm::vec3(pos[0], pos[1], pos[2]);
                    vertex.tex = glm::vec2(uv[0], uv[1]);
                    //vertex.normal = glm::vec3(normal[0], normal[1], normal[2]);
                    
                    pMesh->vVertexPos.push_back(vertex.pos);
                    pMesh->vVertexUV.push_back(vertex.tex);
                }
            }            
            glGenBuffers(1, &pMesh->vertexPosBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, pMesh->vertexPosBuffer);
            glBufferData(GL_ARRAY_BUFFER, pMesh->vVertexPos.size() * sizeof(stVertex::pos), &pMesh->vVertexPos[0], GL_STATIC_DRAW);

            glGenBuffers(1, &pMesh->textureUVBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, pMesh->textureUVBuffer);
            glBufferData(GL_ARRAY_BUFFER, pMesh->vVertexUV.size() * sizeof(stVertex::tex), &pMesh->vVertexUV[0], GL_STATIC_DRAW);
            m_vMeshes.push_back(pMesh);
        }
        
        //Read Material
        printf("\nNum of Textures Found: %d", m_pScene->mNumMaterials);
        for (unsigned int iMaterialIndex = 0; iMaterialIndex < m_pScene->mNumMaterials; iMaterialIndex++)
        {
            const aiMaterial* material = m_pScene->mMaterials[iMaterialIndex];
            //int a = 5;
            int texIndex = 0;
            aiString path;  // filename

            if (material->GetTexture(aiTextureType_DIFFUSE /*| aiTextureType_AMBIENT*/, texIndex, &path) == AI_SUCCESS)
            {
                std::string sDir = sResourceDir + sModelDir;
                std::string sTextureName = path.data;
                std::string sFullPath = sDir + sTextureName;
                int iTexFound = -1;
                
                //Check :: Is the Texture already loaded
                for (unsigned int iTextureIndex = 0; iTextureIndex < m_vTexture.size(); iTextureIndex++)
                {
                    if (sFullPath == m_vTexture[iTextureIndex]->GetTextureName())
                    {
                        iTexFound = iTextureIndex;
                        break;
                    }
                }

                //Texture Not Found
                if(iTexFound == -1)
                {
                    GLenum dummy = 0;
                    auto texture = new Texture(dummy, sFullPath);
                    if (texture->Load())
                    {
                        printf("\nTexture Loaded: %s", sFullPath.c_str());
                        m_vTexture.push_back(texture);
                    }
                    else
                    {
                        //Texture Failed to Load
                        printf("\nTexture Failed to Load: %s", sFullPath.c_str());
                        delete texture;
                    }
                }
            }            
        }        
        return true;
    }
    else
        printf("\nModel Failed to Load: %s", Filename.c_str());

    return false;
}

void Model::Process(float _fTick)
{     
    
    if (m_bEnableCircleRotation)
    {
        //Update Aeroplane Pos and heading

        //rCos(theta), 0, rSin(theta)
        float fDeltaAngle = 0.5;
        m_fCircleAngle = m_fCircleAngle + fDeltaAngle * _fTick;
        if (m_fCircleAngle > 2 * glm::pi<float>())
            m_fCircleAngle = 0.0f;
        
        glm::vec3 newPos = m_vPos;
        newPos.x = m_vPos.x + m_fCircleRadius * cosf(m_fCircleAngle);
        newPos.z  = m_vPos.z + m_fCircleRadius * sinf(m_fCircleAngle);
        
        glm::vec3 pos1 = glm::normalize(m_vPos);
        glm::vec3 pos2 = glm::normalize(newPos);

        float dx = (static_cast <float> (pos1.x) - static_cast <float> (pos2.x));
        float dz =  (static_cast <float> (pos1.z) - static_cast <float> (pos2.z));
        //float heading = glm::atan2(dx, dz);// glm::degrees();
        float heading = atan2(dx, dz);
        
        glm::mat4 matTranslation = glm::translate(glm::mat4(1.0), newPos);
        glm::mat4 matRotate = glm::eulerAngleYXZ(heading, m_vRot.x, m_vRot.z);
        glm::mat4 matScale = glm::scale(m_vScale);

        m_matWorld = matTranslation * matRotate * matScale;        
    }
    else
    {
        if (m_bEnableRotation)
            m_vRot.y += static_cast <float> (3.14159f / 2.0f * _fTick) * m_fRotSpeed;

        glm::mat4 matTranslation = glm::translate(glm::mat4(1.0), m_vPos);
        glm::mat4 matRotate = glm::eulerAngleYXZ(m_vRot.y, m_vRot.x, m_vRot.z);
        glm::mat4 matScale = glm::scale(m_vScale);

        m_matWorld = matTranslation * matRotate * matScale;
    }

    

  
    
}

Texture* Model::GetTexture(unsigned int _iTextureIndex)
{
    if (_iTextureIndex < m_vTexture.size())
    {
        return m_vTexture[_iTextureIndex];
    }
    return nullptr;
}

void Model::SetColor(glm::vec3 _vColor)
{
    m_vColor = _vColor;
}

void Model::SetRotSpeed(float _fSpeed)
{
    m_fRotSpeed = _fSpeed;
}

void Model::SetRotRadius(float _fRadius)
{
    m_fRotRadius = _fRadius;
}

void Model::EnableRotation(bool _enable)
{
    m_bEnableRotation = _enable;
}

void Model::EnableCircleRotation(bool _enable)
{
    m_bEnableCircleRotation = _enable;
}

void Model::SetVisiblity(bool _bVisiblity)
{
    m_bVisiblity = _bVisiblity;
}
