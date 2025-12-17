#include "soTimer.h"

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // Required to stop windows.h messing up std::min
#endif

#include <windows.h>
#include <time.h>


namespace SoulSDK
{
	struct soTimer::Data
	{
		clock_t zeroClock;

		LARGE_INTEGER startTime;
		LARGE_INTEGER frequency;
	};

	soTimer::soTimer()
	{
		m_pData = new Data();
		Reset();
	}


	soTimer::~soTimer()
	{
		delete m_pData;
	}

	void SoulSDK::soTimer::Reset()
	{
		QueryPerformanceFrequency(&m_pData->frequency);
		QueryPerformanceCounter(&m_pData->startTime);
		m_pData->zeroClock = clock();
	}

	unsigned long SoulSDK::soTimer::getMilliseconds()
	{
		LARGE_INTEGER curTime;
		QueryPerformanceCounter(&curTime);

		LONGLONG newTime = curTime.QuadPart - m_pData->startTime.QuadPart;

		//Scale by 1000 for milliseconds
		unsigned long newTicks = static_cast<unsigned long>(1000 * newTime /
															m_pData->frequency.QuadPart);

		return newTicks;
	}

	unsigned long SoulSDK::soTimer::getMicroseconds()
	{
		LARGE_INTEGER curTime;
		QueryPerformanceCounter(&curTime);

		LONGLONG newTime = curTime.QuadPart - m_pData->startTime.QuadPart;

		// Scale by 1000000 for microseconds
		unsigned long newMicro = static_cast<unsigned long>(1000000 * newTime /
															m_pData->frequency.QuadPart);

		return newMicro;
	}

	unsigned long SoulSDK::soTimer::getMillisecondsCPU()
	{
		clock_t newClock = clock();
		return static_cast<unsigned long>(static_cast<float>(newClock - m_pData->zeroClock) /
															(static_cast<float>(CLOCKS_PER_SEC) / 1000.0f));
	}

	unsigned long SoulSDK::soTimer::getMicrosecondsCPU()
	{
		clock_t newClock = clock();
		return static_cast<unsigned long>(static_cast<float>(newClock - m_pData->zeroClock) /
															(static_cast<float>(CLOCKS_PER_SEC) / 1000000.0f));
	}

	unsigned long SoulSDK::soTimer::getStartMs() const
	{
		unsigned long newTicks =
			static_cast<unsigned long>(1000 * m_pData->startTime.QuadPart /
										m_pData->frequency.QuadPart);

		return newTicks;
	}
}
