/*
windowCreator.h
- This takes care of creating a window using the Win32 API and handles messages sent to the window by
the OS
- this version also takes care of the renderer initialisation (setting pixel format, creating a rendering context...)
  all done in the WndProc function (the message handling callback) acting on the RendererOpenGL object in response to the WM_CREATE message
- re-sizing of the OpenGL viewport is also done through WndProc using the RendererOpenGL object in response to to WM_SIZE message

4 June 2005
Martin Masek - SCIS, Edith Cowan University.

*/


/*
class WidowCreator
-encapsulates win32API code for creating and displaying a window
*/

class WindowCreator
{

public:
	WindowCreator(); // default constructor
	~WindowCreator(); // default destructor
	void askWindowSettings(void); // asks about full-screen mode and resolution
	void resetWindowSettings(void); // re-sets resolution and full-screen mode
	HWND initialise(HINSTANCE hInstance); // register, create, and show the window, registers WndProc as the message handler callback, returns the window handle (hMainWindow)
	bool destroy(); // destroys the window
	bool messagePump(); // take queued windows messages from the queue and send them to the message handler callsouth, returns true if exiting, otherwise false
	HWND getWindowHandle(); // returns the window handle (hMainWindow)

	// message handling function registered with OS in WindowCreator::initialise
	static LRESULT WndProc(
									  HWND, // handle to the window
									  UINT, // message ID
									  WPARAM, // extra information about the message (depends on message)
									  LPARAM  // extra information about the message (depends on message)
									  );

	
private:
	HWND hMainWindow; // Handle to the game window.
	bool fullScreen; // will it run full screen or not
	int width; // width of window
	int height; // height of window
	int bitsPerPixel; // bits per pixel of window
};




