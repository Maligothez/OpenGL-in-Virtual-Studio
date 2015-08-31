/*
CSP2306 - Introduction to 3D game programming
Workshop 1, Version 3

Aim: Create a generic window using the Win32 API(Application Programming Interface)
This will become part of our basecode (code which we re-use to avoid re-writing
it each time we create a new application).

This version:

* WIN32API code for window creation and message pump is in a separate class (windowCreator)
* OpenGL rendering is introduced with the RendererOpenGL class - implemented using the Singleton design pattern


*/

// first, include any necessary files and declare global variables.

#include <windows.h> // need this for the windows code to work (include the win32 API)
#include "windowCreator.h" // this contains the windowCreator class to bring up a win32 API window, as well as message handling function, and message pump.
#include "RendererOpenGL.h" // this is the class that will draw our 3D world to the computer screen
#include "Excavator.h"
#include "Vector3.h"
#include "gameTimerHighPerformance.h"
#include <vector>
#include "Geometry.h"

#include "TextureCreator.h"
#include "GameController.h"

using namespace std;


void generateSkybox(vector<Geometry> &skyboxElements)
{
	// load skybox textures
	//GLuint skyboxTexture[6];

	Geometry currentSquare;
	vector<Vertex3> vertices;
	Vertex3 currentVertex;
	vector<int> indices;
	vector<Vertex3> textureCoordinates;
	vector<int> textureIndices;

	currentSquare.setColour(1,1,1);

	// north square

	currentSquare.setName((string)"north");
	
	// vertices	
	
	vertices.clear();
	currentVertex.set(-1.0f, -1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, -1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, 1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(-1.0f, 1.0f, -1.0f);
	vertices.push_back(currentVertex);
	
	// triangles
	indices.clear();
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	// texture coordinates
	textureCoordinates.clear();
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,1,0);
	textureCoordinates.push_back(currentVertex);

	// texture triangles
	textureIndices.clear();
	textureIndices.push_back(0); textureIndices.push_back(1); textureIndices.push_back(2);
	textureIndices.push_back(0); textureIndices.push_back(2); textureIndices.push_back(3);

	currentSquare.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	currentSquare.setMaterial("skyboxN.bmp");

	skyboxElements.push_back(currentSquare);


	//  south square

		currentSquare.setName((string)"south");
	
	// vertices	
	
	vertices.clear();
	currentVertex.set(-1.0f, -1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, -1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, 1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(-1.0f, 1.0f, 1.0f);
	vertices.push_back(currentVertex);
	
	// triangles
	indices.clear();
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	// texture coordinates
	textureCoordinates.clear();

	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);

	// texture triangles
	textureIndices.clear();
	textureIndices.push_back(0); textureIndices.push_back(1); textureIndices.push_back(2);
	textureIndices.push_back(0); textureIndices.push_back(2); textureIndices.push_back(3);


	currentSquare.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	currentSquare.setMaterial("skyboxS.bmp");

	skyboxElements.push_back(currentSquare);


	//  east square
		
	currentSquare.setName((string)"east");
	
	// vertices	
	
	vertices.clear();
	currentVertex.set(1.0f, -1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, 1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, 1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, -1.0f, 1.0f);
	vertices.push_back(currentVertex);
	
	// triangles
	indices.clear();
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	// texture coordinates
	textureCoordinates.clear();
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);

	// texture triangles
	textureIndices.clear();
	textureIndices.push_back(0); textureIndices.push_back(1); textureIndices.push_back(2);
	textureIndices.push_back(0); textureIndices.push_back(2); textureIndices.push_back(3);


	currentSquare.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	currentSquare.setMaterial("skyboxE.bmp");

	skyboxElements.push_back(currentSquare);


	//  west square


	currentSquare.setName((string)"west");
	
	// vertices	
	
	vertices.clear();
	currentVertex.set(-1.0f, -1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(-1.0f, -1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(-1.0f, 1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(-1.0f, 1.0f, -1.0f);
	vertices.push_back(currentVertex);
	
	// triangles
	indices.clear();
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	// texture coordinates
	textureCoordinates.clear();
	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);

	// texture triangles
	textureIndices.clear();
	textureIndices.push_back(0); textureIndices.push_back(1); textureIndices.push_back(2);
	textureIndices.push_back(0); textureIndices.push_back(2); textureIndices.push_back(3);


	currentSquare.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	currentSquare.setMaterial("skyboxW.bmp");

	skyboxElements.push_back(currentSquare);

	// draw top square

	currentSquare.setName((string)"top");
	
	// vertices	
	
	vertices.clear();
	currentVertex.set(-1.0f, 1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(-1.0f, 1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, 1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, 1.0f, -1.0f);
	vertices.push_back(currentVertex);
	
	// triangles
	indices.clear();
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	// texture coordinates
	textureCoordinates.clear();
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);

	// texture triangles
	textureIndices.clear();
	textureIndices.push_back(0); textureIndices.push_back(1); textureIndices.push_back(2);
	textureIndices.push_back(0); textureIndices.push_back(2); textureIndices.push_back(3);

	currentSquare.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	currentSquare.setMaterial("skyboxT.bmp");

	
	skyboxElements.push_back(currentSquare);

	
	//  bottom square

	currentSquare.setName((string)"bottom");
	
	// vertices	
	
	vertices.clear();
	currentVertex.set(-1.0f, -1.0f, -1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(-1.0f, -1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, -1.0f, 1.0f);
	vertices.push_back(currentVertex);
	currentVertex.set(1.0f, -1.0f, -1.0f);
	vertices.push_back(currentVertex);
	
	// triangles
	indices.clear();
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);

	// texture coordinates
	textureCoordinates.clear();
	currentVertex.set(0,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);

	// texture triangles
	textureIndices.clear();
	textureIndices.push_back(0); textureIndices.push_back(1); textureIndices.push_back(2);
	textureIndices.push_back(0); textureIndices.push_back(2); textureIndices.push_back(3);

	currentSquare.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	currentSquare.setMaterial("skyboxB.bmp");
	
	skyboxElements.push_back(currentSquare);


	// cloud layer 1

	// cloud layer 2
}

void generateMap(vector<Geometry> &worldThings)
{

	// This should really be replaced by a level loader to load
	// and parse a description of the world from a file

	// assemble the excavator
	Geometry testThing;

	testThing.loadGeometry("resources\\excavator\\base.ASE");
	testThing.setName((string)"excavator");
	testThing.setColour(0.5,0.5,0.5);

	Geometry childGeometry;
	childGeometry.loadGeometry("resources\\excavator\\wheel.ASE");
	childGeometry.setName((string)"wheels");
	childGeometry.setColour(0.2f,0.2f,0.2f);

	testThing.addChild(childGeometry);

	childGeometry.loadGeometry("resources\\excavator\\turret.ASE");
	childGeometry.setName((string)"turret");
	childGeometry.setColour(0.3f,0.3f,0.3f);

	testThing.addChild(childGeometry);

	childGeometry.loadGeometry("resources\\excavator\\arm.ASE");
	childGeometry.setName((string)"arm");
	childGeometry.setPosition(Vector3f(0,6.687f,-4.99f),(string)"arm");
	childGeometry.setColour(1,1,0);

	Geometry anotherChild;
	anotherChild.loadGeometry("resources\\excavator\\scoop.ASE");
	anotherChild.setName((string)"scoop");
	anotherChild.setPosition(Vector3f(0, -4.063f, 8.814f), (string)"scoop");
	anotherChild.setColour(0.6f,0.6f,0);

	childGeometry.addChild(anotherChild);

	testThing.addChild(childGeometry);

	worldThings.push_back(testThing);


	// add the houses to the level

	Geometry house;
	house.setName((string)"house");
	house.loadGeometry("resources\\simpleHouse.ASE");
	//house.loadGeometry("resources\\officeBuilding.ASE");
	//house.loadGeometry("resources\\tree.ASE");
	house.setColour(1.0f,1.0f,1.0f);

	house.setPosition(Vector3f(150.0f,0.0f,0.0f),(string)"house");
	worldThings.push_back(house);

	//for (int zCoordinate = -50; zCoordinate<50; zCoordinate+=40)
	//{
	//	for (int xCoordinate = -60; xCoordinate<=60; xCoordinate+=30)
	//	{
	//		house.setPosition(Vector3f((float)xCoordinate,0,(float)zCoordinate),(string)"house");
	//		worldThings.push_back(house);
	//	}
	//}


	// game boundaries
	int xLeft = -200;
	int xRight = 200;
	int zFront = 60;
	int zBack = -60;
	int blockSize = 20; // the 'size' of the boundary blocks (in the model file)

	Geometry boundaryBlock;
	boundaryBlock.setName((string)"boundary");
	boundaryBlock.setColour(0.7f,0.7f,0.7f);
	boundaryBlock.setAngle(Vector3f(0,-90,0),(string)"boundary");
	boundaryBlock.loadGeometry("resources\\barrier.ASE");
	// add horizontal boundaries
	for (int i = xLeft+blockSize; i<xRight+blockSize; i+=blockSize)
	{
		boundaryBlock.setPosition(Vector3f((float)i,0,(float)zFront),(string)"boundary");
		boundaryBlock.setAngle(Vector3f(0,90,0),(string)"boundary");
		worldThings.push_back(boundaryBlock);
		boundaryBlock.setPosition(Vector3f((float)i,0,(float)zBack),(string)"boundary");
		boundaryBlock.setAngle(Vector3f(0,90,0),(string)"boundary");
		worldThings.push_back(boundaryBlock);
	}

	// add vertical boundaries
	for (int i = zBack+blockSize; i<zFront+blockSize; i+=blockSize)
	{
		boundaryBlock.setPosition(Vector3f((float)xLeft,0,(float)i),(string)"boundary");
		boundaryBlock.setAngle(Vector3f(0,0,0),(string)"boundary");
		worldThings.push_back(boundaryBlock);
		boundaryBlock.setPosition(Vector3f((float)xRight,0,(float)i),(string)"boundary");
		boundaryBlock.setAngle(Vector3f(0,0,0),(string)"boundary");
		worldThings.push_back(boundaryBlock);
	}

	// assemble and add police cars

	Geometry policeCar;
	policeCar.setName((string)"sheep");
	policeCar.loadGeometry("resources\\sheep\\sheep.ASE");
	policeCar.setColour(1.0f,1.0f,1.0f);


	for (int count=0; count<20; count++)
	{
	policeCar.setPosition(Vector3f(-50+(float)count,0,-70+(float)count),"sheep");
	policeCar.setID(6+count);
	worldThings.push_back(policeCar);
	}

	// ground

	Geometry groundPlane;
	groundPlane.setName((string)"groundPlane");
	groundPlane.setColour(0.4f, 0.7f,0.4f);
	

	vector<Vertex3> vertices;
	Vertex3 currentVertex;
	vector<int> indices;
	vector<Vertex3> textureCoordinates;
	vector<int> textureIndices;


	vertices.clear();
	indices.clear();
	textureCoordinates.clear();
	textureIndices.clear();

	// specify the ground
	int planeSizeX = 200;
	int planeSizeZ = 60;
	int stepSize = 20;
	int count = 0;
	for (int zCoord = -planeSizeZ; zCoord<planeSizeZ; zCoord+=stepSize)
	{
		  for (int xCoord = -planeSizeX; xCoord<planeSizeX; xCoord+=stepSize)
		  {
			  vertices.push_back(Vertex3((float)xCoord,0,(float)zCoord));
			  vertices.push_back(Vertex3((float)xCoord,0,(float)zCoord+stepSize));
			  vertices.push_back(Vertex3((float)xCoord+stepSize,0,(float)zCoord+stepSize));
			  		

			  vertices.push_back(Vertex3((float)xCoord,0,(float)zCoord));
			  vertices.push_back(Vertex3((float)xCoord+stepSize,0,(float)zCoord+stepSize));
			  vertices.push_back(Vertex3((float)xCoord+stepSize,0,(float)zCoord));
			  	

			  textureCoordinates.push_back(Vertex3((xCoord+planeSizeX)/(float)(2*planeSizeX),0,((zCoord+planeSizeZ)/(float)(2*planeSizeZ))));
			  textureCoordinates.push_back(Vertex3((xCoord+10+planeSizeX)/(float)(2*planeSizeX),0,((zCoord+10+planeSizeZ)/(float)(2*planeSizeZ))));
			  textureCoordinates.push_back(Vertex3((xCoord+planeSizeX)/(float)(2*planeSizeX),0,((zCoord+10+planeSizeZ)/(float)(2*planeSizeZ))));
			  
			  textureCoordinates.push_back(Vertex3((xCoord+planeSizeX)/(float)(2*planeSizeX),0,((zCoord+planeSizeZ)/(float)(2*planeSizeZ))));
			  textureCoordinates.push_back(Vertex3((xCoord+10+planeSizeX)/(float)(2*planeSizeX),0,((zCoord+10+planeSizeZ)/(float)(2*planeSizeZ))));
			  textureCoordinates.push_back(Vertex3((xCoord+planeSizeX)/(float)(2*planeSizeX),0,((zCoord+10+planeSizeZ)/(float)(2*planeSizeZ))));
			  

			  indices.push_back(count);
			  textureIndices.push_back(count);

			  indices.push_back(count+1);
			  textureIndices.push_back(count+1);

			  indices.push_back(count+2);
			  textureIndices.push_back(count+2);

			  indices.push_back(count+3);
			  textureIndices.push_back(count+3);

			  indices.push_back(count+4);
			  textureIndices.push_back(count+4);

			  indices.push_back(count+5);
			  textureIndices.push_back(count+5);

			  count += 6;
		  }
	}

	groundPlane.setGeometry(vertices, indices, textureCoordinates, textureIndices);

	worldThings.push_back(groundPlane);


}



/* 
WinMain function.  This is where the program starts from the programmers point of view.
The 'main' function which you may be used to when programming in a text-based console is written
by Windows and is invisible to us.  The automatically generated 'main' includes a call to WinMain.
We write WinMain, and thus control is transfered from the OS (Operating System) to the programmer.
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpComdLine, int nCmdShow)
{
	/* 
	Execution of our program starts here!  We have to set up the application we are
	writing to handle messages (mouse clicks, key presses, etc.) and show a window.
	THE ABOVE WILL BE DONE USING THE WindowCreator CLASS

	Set up for rendering -	THIS WILL BE DONE BY THE RendererOpenGL CLASS

	Then do whatever we want the application to do
	*/

	/* 
	The program is split into 3 parts.
	1) initialisation
	2) The main game loop
	3) clean up and exit
	*/

	/*
	************** P A R T   1   -   I N I T I A L I S A T I O N   *****************

	Here all components of the application get initialised:
	-The window is created - done by the WindowCreator class
	-The renderer (OpenGL) is created and initialised - done by the RendererOpenGL class
	-Game relevant objects are loaded in or initialised - not implemented in this version
	*/



	// C R E A T E   T H E   W I N D O W
	WindowCreator gameWindow; // get an instance of the WindowCreator class

	//gameWindow.askWindowSettings(); // ask user about full-screen mode, resolution, etc...

	HWND hGameWindow; // create a handle to reference the window with
	hGameWindow = gameWindow.initialise(hInstance); // create the window and return a handle to it
	// NOTE: The initialise member function/method of WindowCreator also creates the renderer
	// and makes it set up for rendering to the window.

	// I N I T I A L I S E   T H E   R E N D E R E R
	RendererOpenGL *renderer = RendererOpenGL::Instance(); // get an instance of the renderer class (the thing that draws to the window)
	// NOTE: the renderer already exists at this point (created during window initialisation) the method RendererOpenGL::Instance
	// merely returns a pointer to the existing renderer.

	renderer->initialise(); // initialise the renderer.

	// L O A D   G A M E   M A P   A N D   O B J E C T S


	Excavator bigExcavator(0.0f,0.0f,0.1f,0.0f,90.0f);

	vector<Geometry> sky;
	generateSkybox(sky);
	
	vector<Geometry> worldThings;
	generateMap(worldThings);



	bool validTex;
	for(vector<Geometry>::iterator skyPiece = sky.begin(); skyPiece<sky.end(); skyPiece++)
	{
		validTex = (bool)glIsTexture(skyPiece->getMaterial());
	}


	/*
	************** P A R T   2   -   G A M E   L O O P   *****************

	Looks after aspects of the game while its running:
	-Handles queued windows messages - dispatching to the handler function
	-Handles input from the user - not implemented in this version
	-Updates game state - not implemented in this version
	-Renders users-view of game to screen - calls the RendererOpenGL::Render() method

	*/

	// declare variables for the timer function
	//long elapsedTime;
	//long sleepTime;

	int gameState = PLAYING;


	GameTimerHighPerformance worldTime;
	double startTime, endTime, deltaTime;
	startTime = worldTime.getTime();
	int frames = 0;
	char framerateText[40];

	bool exit = false;  // used to exit the game loop
	while (exit==false) // while we dont want to exit keep looping between message processing and running our game
	{
		frames++;
		if (frames==100)
		{
			endTime = worldTime.getTime();
			deltaTime = (endTime-startTime)/worldTime.getFrequency();

			sprintf_s(framerateText, "framerate: %0.4f",frames/deltaTime);
			SetWindowText(hGameWindow, framerateText);
			frames = 0;
			startTime = endTime;
		}
		exit = gameWindow.messagePump(); // process messages waiting in queue, returns false if the Quit message was received
		// After processing messages, update our game 'state' and render.


		bool status = bigExcavator.processKeyboardInput();

		// update the game
		GameController::runGameLogic(worldThings);		

		// render the world
		renderer->Render(bigExcavator, worldThings, sky); // call the renderers 'Render' method (from the RendererOpenGL class)
	}

	/* 
	If the program gets to this point it means the Quit message was posted - clean up,
	release any allocated memory, and exit.
	*/

	/*



	************** P A R T   3   -   C L E A N   U P   *****************

	Clean up:
	-Release allocated memory
	-Exit

	*/
	// change screen and resolution back to default (gets out of full-screen mode if that was selected)
	gameWindow.resetWindowSettings();

	// release the OpenGL rendering context
	if (!renderer->releaseFromWindow()) //NOTE: part of the releaseFromWindow method is to delete the renderer class instance
	{
		// if the renderer could not be released - bring up an error message in a message box
		MessageBox (NULL,"Problem releasing renderer context","Error",MB_OK);
		return 0;
	}

	// try to shut down the game window
	if (!gameWindow.destroy())
	{
		MessageBox (NULL,"Destroying window","Error",MB_OK);
		return 0;
	}

	return 0; // exit winMain (quit the program)
}