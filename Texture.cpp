#include <iostream>
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
}

bool Texture::Load()
{	
	bool bFlag = Loadjpg();

	
	return bFlag;
}

void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}

bool Texture::Loadpng()
{
	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_2D, m_textureObj);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(m_fileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		switch (nrChannels)
		{
		case 1:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R, width, height, 0, GL_R, GL_UNSIGNED_BYTE, data);
			break;
		}

		case 3:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		}

		default:
		{
			stbi_image_free(data);
			printf_s("Failed to load texture\n");
			return false;
		}

		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf_s("Failed to load texture\n");
	}
	stbi_image_free(data);

	return true;
}

bool Texture::Loadjpg()
{
	

	jpeg_decompress_struct cinfo;
	ErrorManager errorManager;

	FILE* pFile = nullptr;
	fopen_s(&pFile, m_fileName.c_str(), "rb");
	if (!pFile)
		return false;

	// set our custom error handler
	cinfo.err = jpeg_std_error(&errorManager.defaultErrorManager);	
	errorManager.defaultErrorManager.error_exit = ErrorExit;
	errorManager.defaultErrorManager.output_message = OutputMessage;
	if (setjmp(errorManager.jumpBuffer))
	{
		// We jump here on errorzS		
		jpeg_destroy_decompress(&cinfo);
		fclose(pFile);
		return false;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pFile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	auto m_pImageInfo = new ImageInfo();
	m_pImageInfo->nWidth = cinfo.image_width;
	m_pImageInfo->nHeight = cinfo.image_height;
	m_pImageInfo->nNumComponent = cinfo.num_components;
	m_pImageInfo->pData = new uint8_t[m_pImageInfo->nWidth * m_pImageInfo->nHeight * m_pImageInfo->nNumComponent];

	while (cinfo.output_scanline < cinfo.image_height)
	{
		uint8_t* p = m_pImageInfo->pData + cinfo.output_scanline * cinfo.image_width * cinfo.num_components;
		jpeg_read_scanlines(&cinfo, &p, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(pFile);

	glGenTextures(1, &m_textureObj);
	glBindTexture(GL_TEXTURE_2D, m_textureObj);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Check Jpg image component is always 3
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_pImageInfo->nWidth, m_pImageInfo->nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pImageInfo->pData);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Must be delete m_pImageInfo 
	//delete m_pImageInfo;

	return true;
}

void Texture::ErrorExit(j_common_ptr cinfo)
{
}

void Texture::OutputMessage(j_common_ptr cinfo)
{
}
