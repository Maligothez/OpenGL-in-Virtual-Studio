

class GameTimerHighPerformance
{
public:
	GameTimerHighPerformance();
	~GameTimerHighPerformance(){};
	// return time elapsed between now and when getDeltaTime was last called
	double getDeltaTime();
	double getTime();
	double getFrequency();
private:
	LARGE_INTEGER timerFrequency;
	LARGE_INTEGER startTime;
};

GameTimerHighPerformance::GameTimerHighPerformance()
{
	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&startTime);
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