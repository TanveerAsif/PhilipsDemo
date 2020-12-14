#include "ModelManager.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include <glm/gtx/transform.hpp>

ModelManager::ModelManager()
    :m_ProgramID(0), 
    m_WorldID(0), 
    m_ViewID(0), 
    m_ProjectionID(0), 
    m_LightSrcID(0),
    m_TextureID(0),
    m_ModelColorID(0)
{
   
}

ModelManager::~ModelManager()
{ 
    glDeleteProgram(m_ProgramID);

    //delete Model
    for (int i = 0; i < m_vModels.size(); i++)
    {
        auto model = m_vModels[i];
        delete model;
    }
}

bool ModelManager::InitShader()
{
    //Init Shader	
    m_ProgramID = LoadShaders(".\\Resources\\Shaders\\ModelVS.glsl", ".\\Resources\\Shaders\\ModelPS.glsl");
    m_WorldID = glGetUniformLocation(m_ProgramID, "World");
    m_ViewID = glGetUniformLocation(m_ProgramID, "View");
    m_ProjectionID = glGetUniformLocation(m_ProgramID, "Projection");
    m_TextureID = glGetUniformLocation(m_ProgramID, "ModelTexture");
    //m_LightSrcID = glGetUniformLocation(m_ProgramID, "LightPosition");
    m_ModelColorID = glGetUniformLocation(m_ProgramID, "ModelColor");

    return true;
}


bool ModelManager::Initialize()
{
    if (InitShader())
    {        
       {
            glm::vec3 pos = glm::vec3(0.0f, 500.0f, 0.0f);
            glm::vec3 rot = glm::vec3(-1.0 * (3.14159f / 2.0), 0.0f, 0.0f);
            glm::vec3 scale = glm::vec3(0.0312500000f, 0.0312500000f, 0.0312500000f);
            auto model1 = new Model(pos, rot, scale);
            if (model1->LoadMesh("Airplane"))
            {
                model1->EnableRotation(true);
                model1->EnableCircleRotation(true);
                model1->SetRotRadius(2.0);
                model1->SetRotSpeed(0.5);
                m_vModels.push_back(model1);
            }
        }
        
        {
            glm::vec3 pos = glm::vec3(300.0f, 0.0f, 500.0f);
            glm::vec3 rot = glm::vec3(0.0, -1.0 * (3.14159f), 0.0f);
            glm::vec3 scale = glm::vec3(75.0f, 75.0f, 75.0f);
            auto model2 = new Model(pos, rot, scale);
            if (model2->LoadMesh("house"))
            {
                model2->EnableRotation(false);
                m_vModels.push_back(model2);
            }
        }


        {
            glm::vec3 pos = glm::vec3(75.0f, -10.0f, 0.0f);
            glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 scale = glm::vec3(0.25f, 0.25f, 0.25f);
            auto model3 = new Model(pos, rot, scale);
            if (model3->LoadMesh("Spaceship"))
            {
                model3->EnableRotation(true);
                m_vModels.push_back(model3);
            }
                
        }
        

        {
            glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 rot = glm::vec3(-1.0 * (3.14159f / 2.0), 0.0f, 0.0f);
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
            auto model6 = new Model(pos, rot, scale);
            if (model6->LoadMesh("Penguin")) 
            {
                model6->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
                model6->EnableRotation(true);
                m_vModels.push_back(model6);
            }
        }
        
        {
            glm::vec3 pos = glm::vec3(50.0f, 0.0f, 0.0f);
            glm::vec3 rot = glm::vec3(-1.0 * (3.14159f / 2.0), 0.0f, 0.0f);
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
            auto model7 = new Model(pos, rot, scale);
            if (model7->LoadMesh("Penguin"))
            {
                model7->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
                model7->EnableRotation(true);
                model7->SetRotSpeed(0.5);
                m_vModels.push_back(model7);
            }
        }

        {
            glm::vec3 pos = glm::vec3(-50.0f, 0.0f, 0.0f);
            glm::vec3 rot = glm::vec3(-1.0 * (3.14159f / 2.0), 0.0f, 0.0f);
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
            auto model8 = new Model(pos, rot, scale);
            if (model8->LoadMesh("Penguin"))
            {
                model8->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
                model8->EnableRotation(true);
                model8->SetRotSpeed(1.5);
                m_vModels.push_back(model8);
            }
        }

        return true;
    }

    return false;
}

void ModelManager::Process(GLFWwindow* _pWindow, float _fTick)
{
    //Show/Hide of Model
    if (glfwGetKey(_pWindow, GLFW_KEY_A) == GLFW_PRESS) 
    {   //Aeroplane
        m_vModels[0]->SetVisiblity(!m_vModels[0]->GetVisiblity());
    }

    if (glfwGetKey(_pWindow, GLFW_KEY_H) == GLFW_PRESS)
    {   //House
        m_vModels[1]->SetVisiblity(!m_vModels[1]->GetVisiblity());
    }

    if (glfwGetKey(_pWindow, GLFW_KEY_P) == GLFW_PRESS)
    {   //Penguin
        m_vModels[2]->SetVisiblity(!m_vModels[2]->GetVisiblity());
        m_vModels[3]->SetVisiblity(!m_vModels[3]->GetVisiblity());
        m_vModels[4]->SetVisiblity(!m_vModels[4]->GetVisiblity());
    }

    for (unsigned int modelIndex = 0; modelIndex < m_vModels.size(); modelIndex++)
    {
        if(m_vModels[modelIndex]->GetVisiblity())
            m_vModels[modelIndex]->Process(_fTick);
    }
        
}

void ModelManager::Render(glm::mat4 _ViewMatrix, glm::mat4 _ProjMatrix, glm::vec3 _vCameraPos)
{
    glUseProgram(m_ProgramID);

    

    glUniformMatrix4fv(m_ViewID, 1, GL_FALSE, &_ViewMatrix[0][0]);
    glUniformMatrix4fv(m_ProjectionID, 1, GL_FALSE, &_ProjMatrix[0][0]);
    
    
    for (unsigned int modelIndex = 0; modelIndex < m_vModels.size(); modelIndex++)
    {
        if (m_vModels[modelIndex]->GetVisiblity())
        {
            glm::mat4 matWorld = glm::mat4(1.0f);
            std::vector<stMesh*> meshes = m_vModels[modelIndex]->GetMeshList();
            glm::mat4 matLocalTransformation = m_vModels[modelIndex]->GetTransformation();
            matWorld = matLocalTransformation * matWorld;

            glm::vec3 modelColor = m_vModels[modelIndex]->GetColor();
            for (unsigned int meshIndex = 0; meshIndex < meshes.size(); meshIndex++)
            {

                glUniformMatrix4fv(m_WorldID, 1, GL_FALSE, &matWorld[0][0]);
                //glUniformMatrix4fv(m_WorldID, 1, GL_FALSE, &matTranslate[0][0]);


                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, meshes[meshIndex]->vertexPosBuffer);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, meshes[meshIndex]->textureUVBuffer);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

                glUniform4f(m_ModelColorID, modelColor.r, modelColor.g, modelColor.b, 1.0f);

                auto texture = m_vModels[modelIndex]->GetTexture(meshes[meshIndex]->uMaterialIndex - 1); //Index One Ahead
                if (texture != nullptr)
                {
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, texture->GetTextureBuffer());
                    glUniform1i(m_TextureID, 0);
                }

                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(meshes[meshIndex]->vVertexPos.size()));
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
            }
        }
    }    
}