// RendererOpenGL.h
// Aim: To encapsulate all OpenGL specifics into one class
//      to enable easy replacement of OpenGL with other
//      graphics APIs
//
// Created: 31 May 2005
// Modified: 3rd August 2005
// Author: Martin Masek - SCIS, Edith Cowan University

// This class was made using the Singleton design pattern - where only one instance of the class can exist
// and there is a way to get this instance from multiple points in the program.
// This is done sice both WinMain and the WindowCreator class need to use the renderer - implementing a singleton
// means that we dont need to create two separate objects (one for WinMain and one for the WindowCreator)
//
// Features of the Singleton implementation is highlighted in the code using <SINGLETON>
//
// For more information on the Singleton design pattern - see http://gethelp.devx.com/techtips/cpp_pro/10min/10min0200.asp

#pragma once

// include the headers for the OpenGL and OpenGL utilities libraries
#include <windows.h>
#include "GLEW/glew.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>
#include "Game.h"
#include "Excavator.h"
#include "Geometry.h"


using namespace std;

// import the libraries (alternative to this would have been to add these into the linker input dependencies in visual studio)
#pragma comment(lib, "glew32.lib")

#pragma	comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

class RendererOpenGL // definition of the RendererOpenGL class
{
public: // class member variables and methods following 'public:' are public - can be referenced from outside the class
	static RendererOpenGL* Instance();	// <SINGLETON>: This method returns a pointer to the RendererOpenGL object

	~RendererOpenGL(); // default destructor	
	
	bool bindToWindow(HWND &hWnd); // setup OpenGL to use a window (set widow pixel format, get a rendering context)
	bool releaseFromWindow(); // delete the rendering context, and also delete the renderer instance
	void initialise(); // set up the OpenGL state machine with desired settings (eg. enable texture mapping etc..)
	void ResizeCanvas(long widthRequest, long heightRequest); // re-size glViewport (should be called by the window when it re-sizes) - also calls setupViewingFrustum
	void setUpViewingFrustum(); // set the limits on the viewing volume (which part of the world gets shown on the monitor)
	void Render(Excavator &bigExcavator, bool thirdPersonCamera, vector<Geometry> &things); // do the actual drawing

	void RendererOpenGL::renderBox(GLfloat scale, GLuint northID, GLuint southID
							   ,GLuint eastID, GLuint westID, GLuint topID, GLuint bottomID);
	// the next four methods demonstrate how to get information from the graphics card regarding its OpenGL support
	void getGLvendor();
	void getGLrenderer();
	void getGLversion();
	void getGLextensions();

	GLuint loadTexture(char *fileName);
	// font methods - based heavily on code from NeHe (outline fonts tutorial)
	GLvoid BuildFont(GLvoid);
	GLvoid KillFont(GLvoid);
	GLvoid glPrint(const char* fmt, ...);



protected: // class member variables and methods following 'protected:' are protected - can be referenced from inside this class or any class that is based on it
	// <SINGLETON>: the constructor is declared 'protected' so that users of the class (programmers wanting to render)
	// cant make new objects of the class directly - instead they must call the Instance() method, which will give them
	// a pointer to the only instance (and call this constructor if the object does not exist)
	RendererOpenGL(); // default constructor
	
private:   // class member variables and methods following 'private' are private - can only be referenced from methods belonging to this class
	
	// <SINGLETON>: renderer is a pointer (holds the memory location) of a RendererOpenGL object (object of this class)
	// its function is for the class to keep a reference to itself - so if we need to use the RendererOpenGL object
	// somewhere else we call a method that returns the 'renderer' member variable.  It is declared 'static' meaning
	// a single variable will be shared by all instances of the class (we use it to only keep one instance of the class)
	static RendererOpenGL* renderer; 
	
	HWND hWnd; // handle to renderer's window
	HGLRC hRC; // rendering context
	HDC hDC; // device context

	// window size information
	GLsizei rendererWidth; // window width
	GLsizei rendererHeight; // window height

	// projection settings for viewing frustum
	GLdouble fieldOfViewAngle;
	GLdouble nearClippingPlane;
	GLdouble farClippingPlane;	

	GLuint m_myFirstTexture;
	GLuint m_examsTexture;
	GLuint m_skybox[6];  // using the prefix m_ gives you an
  // easy way to distinguish member 
  // variables from local variables.

	// this is the sphere object used for the 'hills'
	GLUquadricObj *m_mySphere;

	// for fonts:
	GLuint base; // display list ID of first character in the font
	GLYPHMETRICSFLOAT gmf[256];


};