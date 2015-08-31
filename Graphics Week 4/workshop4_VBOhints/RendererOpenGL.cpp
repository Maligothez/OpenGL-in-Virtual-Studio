// Implementation of RendererOpenGL.h
// Aim: To encapsulate all OpenGL specifics into one class
//      to enable easy replacement of OpenGL with other
//      graphics APIs (and enable placement of the renderer class into other programs)
//
// Created: 31 May 2005
// Modified: 3rd August 2005
// Modified: 29th August 2012 - Added VBO example
// Author: Martin Masek - SCSS, Edith Cowan University

#include <windows.h>
#include <math.h>
#include "RendererOpenGL.h" // include the definition of the class

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


class cube
{
	// draws a triangle without VBOs...
private:
	float mTop, mBottom, mLeft, mRight;

public:
	cube()
	{
		// example of a constructor - this one doesn't do anything useful
		mTop = 1;
		mBottom = -1;
	}
	~cube()
	{
		// example of a destructor
		mTop = 0;
	}

	void draw()
	{
	glBegin(GL_TRIANGLES);
	  glColor3f(1,0,0);
	  glVertex3f(-1.0f, 0.0f, -1.0f);
	  glVertex3f(1.0f, 0.0f, -1.0f);
	  glVertex3f(0.0f, 1.0f, -1.0f);
	glEnd();
	}
};

class cubeVBO
{
	// To use VBOs, you need GLEW libraries - see RendererOpenGL.h for #include and #pragma
	// Important: for this to work, you need to call the function
	// glewInit(); in the setup code (see the initialize method)
private:
	GLuint mBufferVertList;
	int mNumberOfVertices;

public:
	cubeVBO() // constructor
	{
		// This code sets up the VBO with vertices for a triangle
		mNumberOfVertices = 3;
		createBuffer();
	}

	~cubeVBO() // destructor
	{
		// the object is being destroyed - so free its VBO
		glDeleteBuffers(1, &mBufferVertList);
	}

	void createBuffer()
	{
		// get a handle to a vertex buffer object
		glGenBuffers(1, &mBufferVertList);

		// make the buffer object current as an array buffer (used to store vertex data)
		glBindBuffer(GL_ARRAY_BUFFER, mBufferVertList);

		// reserve some space in memory for the vertex data (uses the current buffer - i.e.. the one referred to in the previous glBindBuffer line)
		// 3*sizeof(GLfloat)*(3+3) ??? - this is the amount of memory we are reserving.
		// 3 vertices, each one has 3 Glfloats to represent position and 3 GL gloats to represent colour
		// the sizeof() function gives us the amount of memory taken by a particular type
		// ie. after this, we have room for 3*(3+3) == 18 GLfloats in the buffer.
		glBufferData(GL_ARRAY_BUFFER, 3*sizeof(GLfloat)*(3+3), NULL, GL_STATIC_DRAW);

		// this maps our buffer (video memory) to main memory, through the variable
		//  vertBuff. Modifying vertBuff modifies video memory!
		GLfloat *vertBuff = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);

		// Now, instead of:
		//glVertex3f(-1.0f, 0.0f, -1.0f);
		//glVertex3f(1.0f, 0.0f, -1.0f);
		//glVertex3f(0.0f, 1.0f, -1.0f);
		// we put these vertex coordinates in the buffer:
		
		vertBuff[0] = -1.0f; // first vertex X
		vertBuff[1] = 0.0f; // first vertex Y
		vertBuff[2] = -1.0f; // first vertex Z

		vertBuff[3] = 1.0f; // second vertex X
		vertBuff[4] = 0.0f; // second vertex Y
		vertBuff[5] = -1.0f; // second vertex Z

		vertBuff[6] = 0.0f; // third vertex X
		vertBuff[7] = 1.0f; // third vertex Y
		vertBuff[8] = -1.0f; // third vertex Z

		// next, lets put some colours in:
		
		// vertex 1 colour
		vertBuff[9] = 1;  // red
		vertBuff[10] = 0; // green
		vertBuff[11] = 0; // blue

		// vertex 2 colour
		vertBuff[12] = 1; // red
		vertBuff[13] = 1; // green
		vertBuff[14] = 0; // blue

		// vertex 3 colour
		vertBuff[15] = 0; // red
		vertBuff[16] = 0; // green
		vertBuff[17] = 1; // blue

		// unmap the buffer - this break the connection between vertBuff and video memory
		// you must do this before drawing the buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);

		// note:  the above are just some numbers in memory - we haven't told OpenGL
		// yet what they mean (it doesn't know they are vertex coordinates and colours)
		// we will do that next.

		// Each vertex has 3 components, is a GLfloat, stride is 0 (dont have to jump over memory to get to the next vertex)
		// and the vertices begin at the beginning of the buffer - (float *)NULL
		glVertexPointer(3, GL_FLOAT, 0, (float *)NULL);

		// the colours begin after the vertices, so from the start of the buffer
		// jump forward by numberOfVertices*3 (because each vertex has 3 components)
		glColorPointer(3, GL_FLOAT, 0,(float *)NULL + 3*mNumberOfVertices);
	}

	void draw()
	{
		// this method draws the things stored in the VBO
		// to see how the object is created and the draw method called - have a look in the RendererOpenGL::Render method
		// further along in this file.

		// make sure that the buffer hasn't become invalid for some reason, if it has - recreate it.
		if (!glIsBuffer(mBufferVertList))
		{
			createBuffer();
		}

		// make the buffer current
		glBindBuffer(GL_ARRAY_BUFFER, mBufferVertList);

		// enable vertex coordinates
		glEnableClientState(GL_VERTEX_ARRAY);
		// enable colours
		glEnableClientState(GL_COLOR_ARRAY);

		
		// Each vertex has 3 components, is a GLfloat, stride is 0 (dont have to jump over memory to get to the next vertex)
		// and the vertices begin at the beginning of the buffer - (float *)NULL
		glVertexPointer(3, GL_FLOAT, 0, (float *)NULL);

		// the colours begin after the vertices, so from the start of the buffer
		// jump forward by numberOfVertices*3 (because each vertex has 3 components)
		glColorPointer(3, GL_FLOAT, 0,(float *)NULL + 3*mNumberOfVertices);
		
		
		// draw all the vertices, connected up as triangles
		glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);
	}
};


class cubeIndexedVBO
{

	// Exercise: create me!

};


void RendererOpenGL::Render(Excavator bigExcavator) // this does the drawing
{	
	float ExcavatorX = bigExcavator.getLocationX();
	float ExcavatorZ = bigExcavator.getLocationZ();
	float angleAroundY = bigExcavator.getZAngleInDegrees();

	float turretAngle = bigExcavator.getTurretAngle();
	float upperArmAngle = bigExcavator.getUpperArmAngle();
	float lowerArmAngle = bigExcavator.getLowerArmAngle();
	
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear buffers from last frame
	glLoadIdentity(); // clear coordinate transformation - (0,0,0) is the screen centre

	static float angle = 0;
   
	// set up camera coordinates and point the camera at (0,0,0)
	float cameraDistance = 20;
	static float cameraHeight = 2;
	float cameraAngle = (90-(180/3.1415927)*atan(cameraDistance/cameraHeight));
	// perform the viewing transformation
	glRotatef(cameraAngle,1,0,0);
	glTranslatef(0,-cameraHeight,-cameraDistance);
	

	// draw the ground
	glColor3f(0,1,0);
	float planeSize = 100;
	glBegin(GL_QUADS);
	  glVertex3f(planeSize,0,planeSize);
	  glVertex3f(planeSize,0,-planeSize);
	  glVertex3f(-planeSize,0,-planeSize);
	  glVertex3f(-planeSize,0,planeSize);
	glEnd();
    
	// Insert excavator drawing code
	
	// create a cube object (our VBO triangle)
	cubeVBO myVBOCube; 
	// note: Its very naughty (yes, you heard right) to create VBOs here. It means you are creating a new VBO each frame,
	// copying data into it, drawing it, then deleting it - not good if you want your program to run fast!
	// Where would you create VBOs? (hint: you don't see the excavator object being created in the render method, do you?)
	// Yeah, I know - code comments are meant to explain things, rather than ask you questions. Doesn't matter, nobody will ready this anyway :)

	glPushMatrix();
	
	// excavator transformation
	glTranslatef(ExcavatorX, 0.0f, ExcavatorZ);
	glRotatef(angleAroundY, 0,1,0);

	// draw the triangle
	myVBOCube.draw();

	glPopMatrix();



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
	glewInit();
	glClearColor(0.0,0.0,0.0,0.0);						// select what colour the background is (here set to black)
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing (using Z-buffer)
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// give OpenGL a hint on which perspective calculation to use (here suggesting the nicest)
	SwapBuffers(hDC);	// draw the screen (will draw a black blank screen as no drawing has been done yet)
	glLineWidth(4);
  
	
	// examples of other operations that could be good here in the future:
	//glEnable(GL_TEXTURE_2D);	// enable 2D texture mapping
	//glFrontFace(GL_CW); // indicate which side of a polygon is the front (defined in terms of the order vertices are given)
	//glEnable(GL_CULL_FACE); // enable back-face culling
}

bool RendererOpenGL::releaseFromWindow()
{
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



