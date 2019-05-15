#ifndef TimerH
#define TimerH

#include <cassert>

class Timer
{
	public:
		Timer() = default;
		Timer(float interval);
		~Timer();
		void Stop();
		void Reset(float interval = 0.0f);
		void Tick(float dTime);
		bool HasStopped() const;
		float GetInterval() const;
		float GetCurrent() const;
		float GetRemaining() const;
	private: 
		float mInterval;
		float mRemaining;
};
#endif 