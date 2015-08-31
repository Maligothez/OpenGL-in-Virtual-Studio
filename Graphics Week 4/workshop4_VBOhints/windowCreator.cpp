/*
windowCreator class implementation
- This takes care of creating a window using the Win32 API and handles messages sent to the window by
the OS

4 June 2005
Martin Masek - SCIS, Edith Cowan University.

*/

#include <windows.h>
#include "windowCreator.h"
#include "RendererOpenGL.h"


/* 
WndProc
This is the callback fuction that the WindowCreator class registers to handle messages for the game
window

Includes handling for the resize message, and thus needs to use the renderer class (RendererOpenGL)
to resize the renderer canvas - same for window creation, sets the window to work with OpenGL in response to WM_CREATE
*/
LRESULT WindowCreator::WndProc(
									  HWND hWnd, // handle to the window
									  UINT message, // message ID
									  WPARAM wParam, // extra information about the message (depends on message)
									  LPARAM lParam  // extra information about the message (depends on message)
								)
{
switch (message)
{
case WM_CREATE: // This message is sent when the window is first created
	{
		// the following calls the Instance method of the renderer class
		// it returns a pointer to the renderer (if the renderer does not exist yet Instance creates it)
		RendererOpenGL::Instance()->bindToWindow(hWnd); //use the pointer to the renderer object and call its bindToWindow method
		break;
	}

case WM_SIZE: // This message is sent when the window re-sizes
	{
		RendererOpenGL::Instance()->ResizeCanvas((long)LOWORD(lParam),(long)HIWORD(lParam)); //re-size the OpenGL View Port
		break; // break and let DefWindowProc take care of the rest of the re-sizing
	}

case WM_DESTROY: // These 3 messages indicate the application is trying to close
case WM_QUIT:
case WM_CLOSE:
	{
		PostQuitMessage(0); // post the quit message to exit - see message pump in WidowCreator::MessagePump
		break; // break and let DefWindowProc take care of shutting the window down
	}
 
default:
	{
		break;
	}

}  
return DefWindowProc(hWnd, message, wParam, lParam); // pass unhandled messages to the default function provided by the OS

}

// Default constructor, set the window handle to NULL (because there is no window yet!)
WindowCreator::WindowCreator()
{
hMainWindow = NULL;
width = 640;
height = 480;
bitsPerPixel = 16;
fullScreen = false;
}

// Default destructor
WindowCreator::~WindowCreator()
{
}

// Returns handle to the window
HWND WindowCreator::getWindowHandle()
{
	return (hMainWindow);
}


// message pump
// process messages in the queue untill none remain.
// returns true if a WM_QUIT message is received, otherwise false.
bool WindowCreator::messagePump()
{
MSG msg; // declare message structure (alternative: to increase game loop speed can make this global or pass it into the messagePump)
if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) // check queue for messages (put them into msg struct) and keep checking untill there a no messages east in the queue.
		{
			if (msg.message==WM_QUIT)
			{
				return (true); // if the message is telling the application to quit, get ready to quit!
			}
			else
			{
				TranslateMessage(&msg); // If we are not quitting, translate and
				DispatchMessage(&msg); // dispatch the message to the message handling function that was registered using RegisterClassEx
			}
		}
		return(false);
}

// WindowCreator::askWindowSettings() - ask user what kind of window they want
void WindowCreator::askWindowSettings()
{
	if (MessageBox(NULL,"Run full screen?","Full Screen?",MB_YESNO)==IDYES)
	{
		fullScreen=true; // set the flag for full-screen mode
	}
	else
	{
		fullScreen=false; // re-set the flag for full-screen mode
	}
}

// WindowCreator::resetWindowSettings() - called at the end of the program to get out of full-screen mode
void WindowCreator::resetWindowSettings()
{
	ChangeDisplaySettings(NULL,0);
	ShowCursor(TRUE);
}



// WindowCreator::Initialise - this does the actual job of creating the window
HWND WindowCreator::initialise(HINSTANCE hInstance)
{
	int ok;
	RECT WindowRect;
	WindowRect.left=(long)0;
	WindowRect.right=(long)width;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)height;
	
	WNDCLASSEX wc = {0}; // create a variable of the windows class structure type(initialised to 0)

	// fill out the member variables of windows class structure
	wc.cbSize = sizeof(WNDCLASSEX); // this holds the size of the structure
	wc.hInstance = hInstance; // the handle to our application (as passed into WinMain by the OS)
	
	wc.lpszClassName = "SuperFunGame"; // gives a name to the application, can be any string of characters
	wc.lpfnWndProc = (WNDPROC)WndProc; // all windows messages will be passed to the function we specify here (so we must write it)
	wc.style = CS_HREDRAW|CS_VREDRAW; // specifies how the application window will be drawn (this setting means the window will be re-drawn if resized hozirontaly 'OR' vertically
	
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // dafault icon
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // dafault 'small' icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // standard arrow for our cursor
	
	// the following two are used for 'normal' windows application but less important for games
	wc.hbrBackground = HBRUSH(GetStockObject(LTGRAY_BRUSH)); // used to define the southground colour, use light grey (in the end our game screen will be on top so this wont show)
	wc.lpszMenuName = NULL; // used to define the menu bar contents (NULL = no menu bar)

	// the following two members of wc are used to request extra memory within the OS
	// we wont ask for any extra.
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	/* 
	That's the whole structure filled out - now we have to tell the OS about it
	*/

	ok = RegisterClassEx(&wc); // register the window class described by wc with the OS, the return value indicates if registration was sucessful or not
	if (ok==0)
	{
		// if registration was unsuccessfull, throw an exception! - to be implemented ;)
	}

	/* 
	Note that registering the window class, as above does not create any windows.  If we want the application
	to have a window we have to create a handle to the window (of type HWND, again this type is provided
	by including windows.h at the top of the program)
	
	The actual window is created with a call to the 'CreateWindowEX' function.  This takes a lot of
	parameters (12)!  No need to memorise them, just look up on MSDN when you need to.
	*/

	DWORD extendedWindowStyle;				// Window Extended Style
	DWORD windowStyle;				// Window Style
		
	// if the fullScreen flag was set in the askWindowSettings method - get the window running full-screen
	if (fullScreen)
	{
		DEVMODE devMode;
		memset(&devMode,0,sizeof(DEVMODE));
		devMode.dmSize=sizeof(DEVMODE);
		devMode.dmPelsWidth	= width;
		devMode.dmPelsHeight = height;
		devMode.dmBitsPerPel = bitsPerPixel;
		devMode.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&devMode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			MessageBox(NULL,"Fullscreen not supported","Error",MB_OK);
			fullScreen=false;
		}
	}
	
	if (fullScreen)
	{
		extendedWindowStyle = WS_EX_APPWINDOW;
		windowStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		extendedWindowStyle = NULL;
		windowStyle = WS_OVERLAPPEDWINDOW|WS_VISIBLE;
	}

	AdjustWindowRectEx(&WindowRect, windowStyle, FALSE, extendedWindowStyle);
	
	hMainWindow = CreateWindowEx(
								extendedWindowStyle, // 'extended style'
								wc.lpszClassName, // name of the windows class that we just registered
								"Excavator Driver", // This text will appear at the top of the window in the title bar
								windowStyle, // Style of the window
								0, // x coordinate of upper east window corner
								0, // y coordinate of upper east window corner
								WindowRect.right - WindowRect.left, // width of window
								WindowRect.bottom - WindowRect.top, // height of window
								NULL, // handle to the window's parent (no parent for our window)
								NULL, // handle to the window's menu (no menu for our window)
								hInstance, // handle to the application (as passed into WinMain)
								NULL // window creation data.
							   );

	if (hMainWindow == NULL)
	{
		// if the window could not be created throw an exception to exit. (add this later)
		MessageBox(NULL,"window handle not created!","Error",MB_OK);
		return (0);
	}

	/*
	If the program got to here, it means the window was successfully created - but is not shown yet.
	To show the window we must call the function 'ShowWindow' with the window handle (hWnd) and
	information on how to show the window (minimised, maximised, etc.)  Here we use SW_SHOW, which
	activates the window and shows it using the location and size we specified in the call to 
	CreateWindowEX.  The ShowWindow function does have a return type to indicate if the window 
	was previously active,since this is not so usefull for us (is not used to indicate error) 
	we ignore it.
	*/

	ShowWindow(hMainWindow, SW_SHOW); // shows the window, but does not update the client area (the content)

	UpdateWindow(hMainWindow); // sends a message for the client area to be re-drawn.

	return (hMainWindow);
}

bool WindowCreator::destroy()
{
	// for this program let windows OS take care of the destroying in the default message handling function, so nothing to do here
	return true;
}


