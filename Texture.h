#pragma once

#include <GL/glew.h>

#include <stdio.h>
#include <setjmp.h>
#include <stdint.h>


#include "libjpg/jpeglib.h"


class Texture
{
public:
    Texture(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);
    std::string GetTextureName() { return m_fileName; }
    GLuint GetTextureBuffer() { return m_textureObj; }

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;    

    bool Loadpng();


    
    struct ImageInfo
    {
        uint32_t nWidth;
        uint32_t nHeight;
        uint8_t nNumComponent;
        uint8_t* pData;
    };

    struct ErrorManager
    {
        jpeg_error_mgr defaultErrorManager;
        jmp_buf jumpBuffer;
    };
    bool Loadjpg();
    static void ErrorExit(j_common_ptr cinfo);
    static void OutputMessage(j_common_ptr cinfo);
};
