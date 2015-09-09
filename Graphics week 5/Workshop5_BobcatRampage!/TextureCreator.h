#pragma once

#include <windows.h>
#include "GLEW/glew.h"
#include <GL/glu.h>
#include <GL/gl.h>

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")




class TextureCreator
{
private:

public:
	static GLuint loadTexture(string fileName)
	{
		//ilInit();
		//iluInit();
		ILuint ILImage;
		GLuint textureHandle;
		ilGenImages(1,&ILImage);
		glGenTextures(1,&textureHandle);

		ilBindImage(ILImage);
		
		ILboolean success = ilLoadImage((ILstring)fileName.data());
		if (!success)
		{
			MessageBox(NULL, "could not load texture", "problem", 0);
		}
		
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		// enable automatic mipmap generation
		glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			ilGetInteger(IL_IMAGE_BPP), 
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT), 
			0, 
			ilGetInteger(IL_IMAGE_FORMAT), 
			GL_UNSIGNED_BYTE,
			ilGetData()
			);

		// release IL image
		ilDeleteImages(1, &ILImage);




		return(textureHandle);
	}

};