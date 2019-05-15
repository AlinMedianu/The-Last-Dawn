#include "Timer.h"

Timer::Timer(float interval)
	:mInterval(interval), mRemaining(interval)
{

}

Timer::~Timer()
{

}

void Timer::Stop()
{
	mRemaining = -1;
}

void Timer::Reset(float interval)
{
	assert(interval >= 0);
	if (interval)
	{
		mRemaining = interval;
		this->mInterval = interval;
	}
	else
		mRemaining = this->mInterval;
}

void Timer::Tick(float dTime)
{
	if (mRemaining > 0)
		mRemaining -= dTime;
}

bool Timer::HasStopped() const
{
	return mRemaining < 0;
}

float Timer::GetInterval() const
{
	return mInterval;
}

float Timer::GetCurrent() const
{
	return mInterval - mRemaining;
}

float Timer::GetRemaining() const
{
	return mRemaining;
}
