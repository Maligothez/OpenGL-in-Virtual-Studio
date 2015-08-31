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
*/

// first, include any necessary files and declare global variables.

#include <windows.h> // need this for the windows code to work (include the win32 API)
#include "windowCreator.h" // this contains the windowCreator class to bring up a win32 API window, as well as message handling function, and message pump.
#include "RendererOpenGL.h" // this is the class that will draw our 3D world to the computer screen
#include "Excavator.h"


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

	/* to be done */


	Excavator bigExcavator(0,0,0.1,-30,90);



	/*
	************** P A R T   2   -   G A M E   L O O P   *****************

	Looks after aspects of the game while its running:
	-Handles queued windows messages - dispatching to the handler function
	-Handles input from the user - not implemented in this version
	-Updates game state - not implemented in this version
	-Renders users-view of game to screen - calls the RendererOpenGL::Render() method

	*/
	
	bool exit = false;  // used to exit the game loop
	while (exit==false) // while we dont want to exit keep looping between message processing and running our game
	{
		exit = gameWindow.messagePump(); // process messages waiting in queue, returns false if the Quit message was received
		// After processing messages, update our game 'state' and render.

		// (insert code here)
		bigExcavator.processKeyboardInput();

		// run AI

		// (insert code here)

		// apply physics

		// (insert code here)

		// render

		renderer->Render(bigExcavator); // call the renderers 'Render' method (from the RendererOpenGL class)

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