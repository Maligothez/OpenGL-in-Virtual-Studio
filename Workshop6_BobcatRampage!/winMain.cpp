/*
CSP2306 - Introduction to 3D game programming
Workshop 1, Version 3

Aim: Create a generic window using the Win32 API(Application Programming Interface)
This will become part of our basecode (code which we re-use to avoid re-writing
it each time we create a new application).

This version:

* WIN32API code for window creation and message pump is in a separate class (windowCreator)
* OpenGL rendering is introduced with the RendererOpenGL class - implemented using the Singleton design pattern

Created: 30th May 2005 - Martin Masek - SCIS, Edith Cowan University.
Modified: 31st May 2005 - Martin Masek - SCIS, Edith Cowan University.
Modified: 3rd August 2005 - Martin Masek - SCIS, Edith Cowan University.
Modified: 11th August 2009 - Martin Masek - SCSS, Edith Cowan University
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

using namespace std;


void generateSkybox(vector<Geometry> &skyboxElements)
{
	// load skybox textures
	GLuint skyboxTexture[6];

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
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,1,0);
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
	currentVertex.set(1,1,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(1,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,0,0);
	textureCoordinates.push_back(currentVertex);
	currentVertex.set(0,1,0);
	textureCoordinates.push_back(currentVertex);

	// texture triangles
	textureIndices.clear();
	textureIndices.push_back(0); textureIndices.push_back(1); textureIndices.push_back(2);
	textureIndices.push_back(0); textureIndices.push_back(2); textureIndices.push_back(3);

	currentSquare.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	currentSquare.setMaterial("skyboxT.bmp");

	skyboxElements.push_back(currentSquare);

	// cloud layer 1

	// cloud layer 2
}

void generateMap(vector<Geometry> &worldThings)
{
	vector<Vertex3> vertices;
	Vertex3 currentVertex;
	vector<int> indices;
	vector<Vertex3> textureCoordinates;
	vector<int> textureIndices;
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
	//house.loadGeometry("C:\\Users\\mmasek\\Desktop\\MaxExport\\simpleHouse.ASE");
	house.loadGeometry("resources\\simpleHouse.ASE");
	house.setColour(0.9f,0.9f,0.9f);

	for (int zCoordinate = -50; zCoordinate<50; zCoordinate+=40)
	{
		for (int xCoordinate = -60; xCoordinate<=60; xCoordinate+=30)
		{
			house.setPosition(Vector3f((float)xCoordinate,0,(float)zCoordinate),(string)"house");
			worldThings.push_back(house);
		}
	}




	// assemble and add police cars

	Geometry policeCar;
	policeCar.setName((string)"police");
	policeCar.loadGeometry("resources\\policeCar\\policeCar.ASE");
	policeCar.setColour(0.1f,0.1f,0.8f);
	policeCar.setPosition(Vector3f(-30,0,50),"police");
	policeCar.setID(1);

	Geometry policeSiren;
	policeSiren.setName((string)"siren");
	policeSiren.loadGeometry("resources\\policeCar\\siren.ASE");
	policeSiren.setColour(0.9f,0.1f,0.1f);
	policeCar.addChild(policeSiren);

	Geometry policeWindows;
	policeWindows.setName((string)"window");
	policeWindows.loadGeometry("resources\\policeCar\\window.ASE");
	policeWindows.setColour(0.9f,0.9f,0.1f);
	policeCar.addChild(policeWindows);

	Geometry policeKey;
	policeKey.setName((string)"key");
	policeKey.loadGeometry("resources\\policeCar\\key.ASE");
	policeKey.setColour(0.8f,0.1f,0.1f);
	policeCar.addChild(policeKey);


	worldThings.push_back(policeCar);

	policeCar.setPosition(Vector3f(30,0,50),"police");
	policeCar.setID(2);
	worldThings.push_back(policeCar);

	policeCar.setPosition(Vector3f(35,0,50),"police");
	policeCar.setID(3);
	worldThings.push_back(policeCar);

	policeCar.setPosition(Vector3f(40,0,50),"police");
	policeCar.setID(4);
	worldThings.push_back(policeCar);

	policeCar.setPosition(Vector3f(45,0,50),"police");
	policeCar.setID(5);
	worldThings.push_back(policeCar);

	policeCar.setPosition(Vector3f(50,0,50),"police");
	policeCar.setID(6);
	worldThings.push_back(policeCar);

	policeCar.setPosition(Vector3f(50,0,-70),"police");
	policeCar.setID(6);
	worldThings.push_back(policeCar);

	Geometry boundaryL;
	boundaryL.setName((string)"boundary");
	boundaryL.loadGeometry("resources\\boundary\\barrier.ASE");
	boundaryL.setColour(1.0f,1.0f,1.0f);
	boundaryL.setPosition(Vector3f(-200,0,-40),"boundary");
	boundaryL.setID(1);
	worldThings.push_back(boundaryL);
	//left boundary
	int lower = -20;
	for (int j = 0; j <=4; j++) {
		boundaryL.setPosition(Vector3f(-200,0,lower),"boundary");
		boundaryL.setID(j+1);
		worldThings.push_back(boundaryL);
		lower += 20;
	}

	//rightboundary
	lower = -40;
	for (int j = 0; j <=5; j++) {
		boundaryL.setPosition(Vector3f(203,0,lower),"boundary");
		boundaryL.setID(j+1);
		worldThings.push_back(boundaryL);
		lower += 20;
	}

	lower = -40;

	//boundary Wide
	Geometry boundaryW;
	boundaryW.setName((string)"boundary");
	boundaryW.loadGeometry("resources\\boundary\\barrier.ASE");
	boundaryW.setColour(1.0f,1.0f,1.0f);
	boundaryW.rotateCoordinates();
	boundaryW.setPosition(Vector3f(-200,0,-40),"boundary");
	//boundaryW.setID(1);
	//worldThings.push_back(boundaryW);

	//top boundary
	int side = -200;
	for (int j = 0; j <20; j++) {
		boundaryW.setPosition(Vector3f(side,0,-60),"boundary");
		worldThings.push_back(boundaryW);
		side += 20;
	}
	//bottom boundary
	side = -200;
	for (int j = 0; j <20; j++) {
		boundaryW.setPosition(Vector3f(side,0,60),"boundary");
		worldThings.push_back(boundaryW);
		side += 20;
	}

	Geometry floor;
	floor.setName((string)"floor");
	vertices.clear();
	floor.setColour(0,1,0);
	int indicesIndex = 0;
	indices.clear();
	textureCoordinates.clear();
	textureIndices.clear();
	textureIndices.clear();
	for (float x = -200; x < 200; x += 20) {
		for (float z = -60; z < 60; z +=20) {
			currentVertex.set(x, 0.0f, z);
			vertices.push_back(currentVertex);
			currentVertex.set(x, 0.0f, z+20);
			vertices.push_back(currentVertex);
			currentVertex.set(x+20, 0.0f, z+20);
			vertices.push_back(currentVertex);
			currentVertex.set(x+20, 0.0f, z);
			vertices.push_back(currentVertex);

			currentVertex.set(x,0,z);
			textureCoordinates.push_back(currentVertex);
			currentVertex.set(x,0,z+20);
			textureCoordinates.push_back(currentVertex);
			currentVertex.set(x+20,0,z+20);
			textureCoordinates.push_back(currentVertex);
			currentVertex.set(x+20,0,z);
			textureCoordinates.push_back(currentVertex);

			indices.push_back(indicesIndex); indices.push_back(indicesIndex +1); indices.push_back(indicesIndex +2);
			indices.push_back(indicesIndex); indices.push_back(indicesIndex +2); indices.push_back(indicesIndex +3);

			textureIndices.push_back(indicesIndex); textureIndices.push_back(indicesIndex+1); textureIndices.push_back(indicesIndex+2);
			textureIndices.push_back(indicesIndex); textureIndices.push_back(indicesIndex +2); textureIndices.push_back(indicesIndex +3);

			indicesIndex += 4;
		}
	}
	floor.setGeometry(vertices, indices, textureCoordinates, textureIndices);
	worldThings.push_back(floor);

	//for (int count=0; count<30; count++)
	//{
	//policeCar.setPosition(Vector3f(-50+count,0,-70+count),"police");
	//policeCar.setID(6+count);
	//worldThings.push_back(policeCar);
	//}

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
		validTex = glIsTexture(skyPiece->getMaterial());
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

			sprintf(framerateText, "framerate: %0.4f",frames/deltaTime);
			SetWindowText(hGameWindow, framerateText);
			frames = 0;
			startTime = endTime;
		}
		exit = gameWindow.messagePump(); // process messages waiting in queue, returns false if the Quit message was received
		// After processing messages, update our game 'state' and render.


		bool status = bigExcavator.processKeyboardInput();
		//status = bigExcavator.checkCollisions(worldThings);


		// check collisions between excavator and other objects
		for(vector<Geometry>::iterator i = worldThings.begin(); i<worldThings.end(); i++)
		{
			if (!i->getName().compare("excavator"))

			{
				Vector3f heading(0,0,0);
				Vector3f force(0,0,0);
				// check collisions between the excavator and other things in the world
				for(vector<Geometry>::iterator j = worldThings.begin(); j<worldThings.end(); j++)
				{
					if (!j->getName().compare("house"))
					{
						if (j->isColiding(i->getPosition(),i->getBoundingSphereRadius()))
						{
							// clear VBO
							if (j->hasVBO())
							{
								j->clearVBO();
							}
							j->loadGeometry("resources\\houseRubble.ASE");
							j->setName((string)"rubble");
						}
					}
					else if (!j->getName().compare("police"))
					{
						if (j->isColiding(i->getPosition(),i->getBoundingSphereRadius()))
						{
							// change the excavator colour to red if caught!
							i->setColour(1,0,0);
						}
					}

					else if (!j->getName().compare("boundary"))
					{
						if (i->isColiding(j->getPosition(), (j->getBoundingSphereRadius())))
						{
							//float forceLength = force.length();							
							//force = (1/forceLength)*force; // normalize to get direction only

							// scale force so its greater closer to the house
							//float maxForceLength = 100;
							//forceLength = max(0, (maxForceLength-forceLength));


							//heading += force; // add the force to the sum of forces acting on the car
						}
					}
				}
			}

		}

		// update police car position and orientation

		for(vector<Geometry>::iterator i = worldThings.begin(); i<worldThings.end(); i++)
		{
			// the following code should really be moved to a class (eg. some kind of an 'update'
			// method in the Geometry class

			if (!i->getName().compare("police")) // if the object is a police car
			{
				Vector3f heading(0,0,0);
				Vector3f force(0,0,0);
				// work out the 'force' acting on the police car
				// - the excavator atracts the police car
				// - houses repel the police car if within a certain radius (house rubble has no effect)
				// - the police cars repel each other if too close(to keep their distance)
				//
				for(vector<Geometry>::iterator j = worldThings.begin(); j<worldThings.end(); j++)
				{
					if (!j->getName().compare("police") && i->getID() != j->getID()) // if its a different police car than this one
					{
						// get a vector pointing from the other police car towards this one
						force = i->getPosition() - j->getPosition(); 

						// only apply the repulsive force if the cars are coliding
						if (i->isColiding(j->getPosition(), j->getBoundingSphereRadius()))
						{
							// normalize the force vector to isolate direction only
							float forceLength = force.length();
							force = (1/forceLength)*force; 

							// scale force so its greater closer to the car
							float maxForceLength = 50;
							forceLength = max(0, (maxForceLength-forceLength));
							force = forceLength*force; // rescale the force based on this inverse distance

							heading += force; // add the force to the sum of forces acting on the car
						}					
					}
					else if (!j->getName().compare("house"))
					{
						// houses 'repulse' the police car (to avoid the car colliding with them
						force = i->getPosition() - j->getPosition(); // get a vector pointing from the house towards the car

						if (i->isColiding(j->getPosition(), (j->getBoundingSphereRadius())+10))
						{
							float forceLength = force.length();							
							force = (1/forceLength)*force; // normalize to get direction only

							// scale force so its greater closer to the house
							float maxForceLength = 100;
							forceLength = max(0, (maxForceLength-forceLength));
							force = forceLength*force;

							heading += force; // add the force to the sum of forces acting on the car
						}					
					}
					else if (!j->getName().compare("excavator"))
					{
						// the bobcat atracts police
						force = j->getPosition() - i->getPosition(); // get a vector pointing from the car to the bobcat
						heading += force; // add the force to the sum of forces acting on the car
					}
				}


				//
				// At this point we have have a force vector that acts on the police car
				// this tells us
				//   a) the direction the car should be facing in
				//   b) (a measure of) how fast it should move in that direction
				//
				// As we want this to look realistic (ie. cars cant change their direction instantaneously
				// and only move in the forward direction) we rotate the car in the right direction
				// by some increment, and move it forward.
				//


				// current position of the car
				Vector3f newPosition = i->getPosition();

				// current orientation of he car (only interested in rotation around y-axis)
				Vector3f orientation = i->getAngle(); 
				float angleAroundY = orientation.y();


				// get desired heading angle
				heading = (1/heading.length())*heading; // normalize heading
				float desiredAngleAroundY = (180/(float)pi)*acos(heading.x()); // get the angle in degrees (acos works in radians)

				// adjust for the quadrant we are in (acos is only good between 0 and 180 degrees)				
				if (heading.z()>0)
				{
					desiredAngleAroundY = 360 - desiredAngleAroundY;
				}

				// ************
				// turn the car
				// ************

				// work out distance to turn (in degrees)
				float turningDirection = desiredAngleAroundY - angleAroundY;

				// see if its a shorter to turn the other way
				if (turningDirection>180)
				{
					turningDirection = turningDirection - 360;
				}
				if (turningDirection<-180)
				{
					turningDirection = turningDirection + 360;
				}

				// set the new angle based on a turning speed
				float turningSpeed = 0.1f;
				angleAroundY += turningSpeed*turningDirection;
				orientation.set(orientation.x(), angleAroundY, orientation.z());
				i->setAngle(orientation, (string)"police");

				// ********************
				// move the car forward
				// ********************

				float movementSpeed = 1;
				// the car model actually faces down the wrong axis - so subtract 90 degrees from the angleAroundY to compensate
				float xIncrement = movementSpeed*(float)sin(pi*((-90+angleAroundY)/180.0));
				float zIncrement = movementSpeed*(float)cos(pi*((-90+angleAroundY)/180.0));
				heading.set(-xIncrement,0,-zIncrement);
				newPosition += heading;
				i->setPosition(newPosition,(string)"police");
			}
		}

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