// Implementation of RendererOpenGL.h
// Aim: To encapsulate all OpenGL specifics into one class
//      to enable easy replacement of OpenGL with other
//      graphics APIs (and enable placement of the renderer class into other programs)
//
// Created: 31 May 2005
// Modified: 3rd August 2005
// Modified: 20th August 2009
// Author: Martin Masek - SCIS, Edith Cowan University

#include <windows.h>
#include <math.h>
#include "RendererOpenGL.h" // include the definition of the class
#include "TextureCreator.h"

#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")


RendererOpenGL* RendererOpenGL::renderer = NULL;  // initialise the pointer to the class to NULL

RendererOpenGL* RendererOpenGL::Instance() // for returning a pointer to the renderer class (also instantiates it if it does not exist)
{
	// the class is implemented as a singleton, so create a new instance only if one does not exist already
	if (renderer==NULL)
	{
		// call the constructor (it was declared 'protected' in the header file (RendererOpenGL.h)
		// so users of the class cant instantiate new objects from this class (constructor can only be called from within the class)
		renderer = new RendererOpenGL; 
	}
	return renderer; // return the pointer to the renderer (to see how this is used, see the message processing function in windowCreator.cpp)
}

RendererOpenGL::RendererOpenGL() // constructor
{	
	hWnd = NULL; // current window handle
	hRC = NULL; // rendering context handle (drawing context for OpenGL
	hDC = NULL; // device context handle (device that is displaying the current window)

	rendererWidth = GLsizei(640);
	rendererHeight = GLsizei(480);
	fieldOfViewAngle = 45.0f;
	nearClippingPlane = 1.0f;
	farClippingPlane = 200.0f;
}

RendererOpenGL::~RendererOpenGL() // destructor
{
}

// outline font code from NeHe - Jeff Molofee: (based on lesson 14)

// set up fonts
GLvoid RendererOpenGL::BuildFont(GLvoid)
{
	HFONT font;
	base = glGenLists(256);
	font = CreateFont(-12, // font height
						0, // width
						0, // escapement angle
						0, // orientation
						FW_BOLD, // weight
						FALSE, // italic
						FALSE, // underline
						FALSE, // strikeout
						ANSI_CHARSET, // character set
						OUT_TT_PRECIS, // output precision
						CLIP_DEFAULT_PRECIS, // clipping precision
						ANTIALIASED_QUALITY, // quality
						FF_DONTCARE|DEFAULT_PITCH, // family and pitch
						"Comic Sans MS"); // font name
	SelectObject(hDC, font);

	wglUseFontOutlines( hDC,
		0, // starting character
		255, // number of characters
		base, // starting display list
		0.0f, // deviation from true outlines
		0.1f, // font Z-depth
		WGL_FONT_POLYGONS, // use polygons
		gmf); // adress of buffer to receive data
}

GLvoid RendererOpenGL::KillFont(GLvoid)
{
	glDeleteLists(base, 256);	
}

GLvoid RendererOpenGL::glPrint(const char *fmt, ...)
{
	float length = 0;
	char text[256];
	va_list ap;

	if (fmt == NULL)
		return;
	va_start(ap, fmt);
	vsprintf_s(text, sizeof(text), fmt, ap);
	// MS UNSAFE: vsprintf(text, fmt, ap);
	va_end(ap);

	for (unsigned int loop=0; loop<(strlen(text));loop++)
	{
		length+=gmf[text[loop]].gmfCellIncX;
	}
	glTranslatef(-length/2,0.0f, 0.0f);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base);

	glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();

}


void RendererOpenGL::Render(Excavator &bigExcavator, bool thirdPersonCamera, vector<Geometry> &things) // this does the drawing
{	
	float ExcavatorX = bigExcavator.getLocationX();
	float ExcavatorZ = bigExcavator.getLocationZ();
	float angleAroundY = bigExcavator.getZAngleInDegrees();

	float armAngle = bigExcavator.getUpperArmAngle();
	float scoopAngle = bigExcavator.getLowerArmAngle();
	GLuint m_textureObject = TextureCreator::loadTexture("skyboxN.bmp");
	// find the excavator geometry and set its parameters
	// (does this really belong in a render method?)
	
	for(vector<Geometry>::iterator i = things.begin(); i<things.end(); i++)
	{
		if (!i->getName().compare("excavator"))
		{
			Vector3f angle;
			Vector3f position;

			angle.set(0,angleAroundY+180,0);
			i->setAngle(angle,(string)"excavator");
			position.set(ExcavatorX,1,ExcavatorZ);
			i->setPosition(position,(string)"excavator");

			angle.set(armAngle, 0, 0);
			i->setAngle(angle, (string)"arm");

			angle.set(scoopAngle, 0, 0);
			i->setAngle(angle, (string)"scoop");
		}
	}


	glBindTexture(GL_TEXTURE_2D, m_textureObject);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 2.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, 2.0f, 0.0f);
	glEnd();
	

	// enable texture mapping
	glEnable(GL_TEXTURE_2D);


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear buffers from last frame
	glLoadIdentity(); // clear coordinate transformation - (0,0,0) is the screen centre

	
	// **********************************
	// perform the viewing transformation
	// **********************************  
	

	// This will give us a 3rd person fixed camera:
	// set up camera coordinates and point the camera at (0,0,0)
	float cameraDistance = 80;
	static float cameraHeight = 120;
	float cameraAngle = (90-(180/3.1415927f)*atan(cameraDistance/cameraHeight));
	//camera true
	if (thirdPersonCamera)
	{
		
		glTranslatef(0  , 0 , -30 );
		glRotatef(20, 1, 0, 0);
		glRotatef(-angleAroundY, 0, 1, 0);
	
		glTranslatef(-ExcavatorX, -10, -ExcavatorZ);
	}
	//camera false
	else {
		
		glRotatef(cameraAngle, 1, 0, 0);
		glTranslatef(0, -cameraHeight, -cameraDistance);
	}


	// ************************
	// end of viewing transform
	// ************************

	

	// draw the ground - this a triangle strip instead of a large
	// quad - the more vertices the better the lighting will look
	glColor3f(0,1,0);
	int planeSize = 100;
	glNormal3f(0,1,0);
	for (int zCoord = -planeSize; zCoord<planeSize; zCoord++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		  for (int xCoord = -planeSize; xCoord<planeSize; xCoord++)
		  {
			  glVertex3i(xCoord,0,zCoord);
			  glVertex3i(xCoord,0,zCoord+1);
		  }
	    glEnd();
	}

	// draw the Geometrys
	for each (Geometry currentThing in things)
	{
		//currentThing.drawOpenGLImmediate();
		currentThing.drawOpenGLVertexBufferObject();
	}

	SwapBuffers(hDC);
}


void RendererOpenGL::setUpViewingFrustum()  // set up the viewing volume
{
	// Select projection matrix and reset it to identity, subsequent operations are then performed on this matrix (gluPerspective)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// set up the perspective of the window
	GLdouble aspectRatio = (GLfloat)rendererWidth/(GLfloat)rendererHeight;
	gluPerspective(fieldOfViewAngle, aspectRatio, nearClippingPlane, farClippingPlane);

	// select the model-view matrix (to de-select the projection matrix) and reset it to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



bool RendererOpenGL::bindToWindow(HWND &windowHandle)
{
	//set up pixel format, rendering context
	// NOTE: this method uses 'wgl' commands - the MS Windows Operating system binding for OpenGL.
	// it must be over-written when porting this renderer to another OS.

	// Need to do 5 things before we can use OpenGL
	// First - get the device context of the game window (ie. what is the window being shown on eg. graphics adapter)
	// Second - set that device to some desired pixel format
	// Third - create a rendering context for OpenGL (something OpenGL draws to and maps to the device)
	// Fourth - make the rendering context 'current'
	// Fifth - Set the size of the OpenGL window.
	
	// First - get the device context of the game window
	hWnd = windowHandle;
	hDC = GetDC(hWnd); // get the device context of the window
	

	// Second - set the device to some desired pixel format
	// This is done be filling out a pixel format descriptor structure

	static PIXELFORMATDESCRIPTOR pfd; // pixel format descriptor

	// The pixel format discriptor has a lot of memembers (26) !
	// luckily we dont need most of them and set them to zero
	// we could go through the structure member by member and set them to zero
	// but a shortcut way is to use memset to initialise everything to zero

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // sets all memmbers of pfd to 0

	// now we change only the relevant pfd members
	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits	= 16;
	pfd.cDepthBits	= 16;

	// based on the descriptor, choose the closest supported pixel format.
	int PixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (PixelFormat==0)
	{
		// error
		MessageBox (NULL,"Could not choose pixel format","Error",MB_OK);
		return (false);
	}

	// set the display device (device context) to the pixel format
	if (SetPixelFormat(hDC, PixelFormat, &pfd)==0)
	{
		// error
		MessageBox (NULL,"Could not set pixel format","Error",MB_OK);
		return (false);
	}

	

	// Third - create rendering context
 
	 hRC = wglCreateContext(hDC); // windows dependent OpenGL function (wgl)
	 if (hRC==NULL)
	 {
		 MessageBox (NULL,"Could not create GL rendering context","Error",MB_OK);
		 return (false);
	 }

	 // Fourth - Make the rendering context current	 
	 if (!wglMakeCurrent(hDC, hRC))
	 {
		 MessageBox (NULL,"Could not make rendering context current","Error",MB_OK);
		 return (false);
	 }

	 // Fifth - set the size of the OpenGL window

	 /*
	 ***** Note: this step is important, not setting an initial size
	 can cause the whole OS to crash (computer is re-set)
	 */

	 RECT rect; // structure to store the coordinates of the 4 corners of the window
	 GetClientRect (hWnd, &rect); // put the window coordinates in the structure
	 ResizeCanvas(long(rect.right-rect.left), long(rect.bottom-rect.top));
	 
	return (true);
}

void RendererOpenGL::getGLvendor()
{
	char *info;
	info = (char *)glGetString(GL_VENDOR);
	MessageBox (NULL,info,"Vendor",MB_OK);
}
void RendererOpenGL::getGLrenderer()
{
	char *info;
	info = (char *)glGetString(GL_RENDERER);
	MessageBox (NULL,info,"Renderer",MB_OK);
}
void RendererOpenGL::getGLversion()
{
	char *info;
	info = (char *)glGetString(GL_VERSION);
	MessageBox (NULL,info,"Version",MB_OK);
}
void RendererOpenGL::getGLextensions()
{
	char *info;
	info = (char *)glGetString(GL_EXTENSIONS);
	MessageBox (NULL,info,"Extensions",MB_OK);
}

void RendererOpenGL::initialise()
{
	// initialise the extension wrangler library (this enables GL functionality newer than in the
	// files provided with visual studio
	glewInit();

    glClearColor(0.0,0.0,0.0,0.0);						// select what colour the background is (here set to black)
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing (using Z-buffer)
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// give OpenGL a hint on which perspective calculation to use (here suggesting the nicest)
	SwapBuffers(hDC);	// draw the screen (will draw a black blank screen as no drawing has been done yet)
	glLineWidth(4);

}

bool RendererOpenGL::releaseFromWindow()
{
	KillFont();
	if (hDC == NULL || hRC == NULL) 
	{
		MessageBox (NULL,"hDC or hRC already NULL!","Error",MB_OK);
		return (false);
	}

	if (wglMakeCurrent(NULL, NULL) == false)					
	{
		// error
		MessageBox (NULL,"Could not set hDC to NULL","Error",MB_OK);
		return (false);
	}

	if (wglDeleteContext(hRC) == false)						
	{
		// error deleting rendering context
		MessageBox (NULL,"Could not delete rendering context","Error",MB_OK);
		return (false);
	}

	// Windows releases the DC in the default message handler, if not passing all messages through it,
	// the releaseDC function must be called (we are, so its commented out - otherwise allways get the message box error)
	//if (releaseDC(hWnd,hDC)==false)
	//{
	//	MessageBox (NULL,"Could not release device context","Error",MB_OK);
	//	return (false);
	//}
	
	hRC	= NULL;
    hDC	= NULL;	

	// delete the renderer and set it to NULL (this is how the singleton gets deleted)
	delete renderer;
	renderer = NULL;

	return (true);

}

// re-size the viewport
void RendererOpenGL::ResizeCanvas(long widthRequest, long heightRequest)
{
	rendererWidth = (GLsizei)widthRequest;
	rendererHeight = (GLsizei)heightRequest;
    glViewport(0, 0, rendererWidth, rendererHeight);
	setUpViewingFrustum();	
}



