#pragma once
#include "Singleton.h"
#include <Windows.h>
#include <stdexcept>

class PerformanceCounter : public Singleton<PerformanceCounter>
{
private:
	double PCFreq = 0.0;
	__int64 CounterStart = 0;

public:
	void StartCounter()
	{
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			throw std::bad_exception();

		PCFreq = double(li.QuadPart) * 0.001;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}
	
	double GetCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double(li.QuadPart - CounterStart) / PCFreq;
	}
};