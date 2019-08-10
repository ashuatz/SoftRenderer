#pragma once
#include "Singleton.h"
#include "performanceCounter.h"

class Time : public Singleton<Time>
{
public:
private:
	__int64 startTime;
	__int64 currentTime;
	__int64 lastSecond;

	int cachedFrameRate;
	float cachedDeltaTime;
	
public:

	Time()
	{
		cachedDeltaTime = 0;
		cachedFrameRate = 0;
	}

	float GetDeltaTime() { return cachedDeltaTime; }
	int GetFrame() { return cachedFrameRate; }

	void CheckCounter()
	{
		auto counter = PerformanceCounter::GetInstance().GetCounter();
		cachedDeltaTime = counter != INFINITY ? counter : 0;
		PerformanceCounter::GetInstance().StartCounter();

		cachedFrameRate = (int)(1000 / cachedDeltaTime);
		currentTime += cachedDeltaTime;

		if (((currentTime - startTime) % 10) != lastSecond)
		{
			lastSecond = (currentTime - startTime) % 10;
		}
	}
private:
protected:
	
};