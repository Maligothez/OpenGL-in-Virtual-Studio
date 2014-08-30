// 2005 - Martin Masek, Edith Cowan University

#include <sstream> 

class GameTimerHighPerformance
{
public:
	GameTimerHighPerformance();
	~GameTimerHighPerformance(){};
	// return time elapsed between now and when getDeltaTime was last called
	double getDeltaTime();
	double getTime();
	double getFrequency();
	double calculateFPS(HWND gamewindow);

private:
	LARGE_INTEGER timerFrequency;
	LARGE_INTEGER startTime;
	int frameCount;
	char framerateText[60];
	double fps;
	LARGE_INTEGER currentTime;
	double secondsElapsed;

};

GameTimerHighPerformance::GameTimerHighPerformance()
{
	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&startTime);
	frameCount = 0;
	fps = 0;
}

double GameTimerHighPerformance::getTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return ((double)currentTime.QuadPart);
}
double GameTimerHighPerformance::getFrequency()
{
	return ((double)timerFrequency.QuadPart);
}

double GameTimerHighPerformance::getDeltaTime()
{
	LARGE_INTEGER currentTime;
	double secondsElapsed;
	QueryPerformanceCounter(&currentTime);
	secondsElapsed = (double)(currentTime.QuadPart - startTime.QuadPart)/(double)timerFrequency.QuadPart;
	startTime = currentTime;
	return(secondsElapsed);
}

double GameTimerHighPerformance::calculateFPS(HWND gamewindow)
{
	//  Increase frame count
	frameCount++;
	QueryPerformanceCounter(&currentTime);
	//  Calculate time passed
	secondsElapsed = (double)(currentTime.QuadPart - startTime.QuadPart)/(double)timerFrequency.QuadPart;
	if(secondsElapsed > 1)
	{
		//  calculate the number of frames per second
		fps = frameCount / secondsElapsed;
		//  Set time
		startTime = currentTime;
		//  Reset frame count
		frameCount = 0;
		sprintf(framerateText, "Bobcat Rampage! - FPS: %0.4f", fps);
		SetWindowText(gamewindow, framerateText);	
	}

	/*if (frameCount == 100){

	sprintf(framerateText, "Bobcat Rampage! - FPS: %0.4f", frameCount/getDeltaTime());
	SetWindowText(gamewindow, framerateText);
	frameCount = 0;
	}*/

	return fps;
}