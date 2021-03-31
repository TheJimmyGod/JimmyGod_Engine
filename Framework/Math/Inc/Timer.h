#pragma once

namespace JimmyGod::Math
{
	class Timer
	{
	public:
		Timer();

		void Initialize();
		void Update();

		float GetElapsedTime() const;
		float GetTotalTime() const;
		float GetFramesPerSecond() const;

	private:
		// http://msdn2.microsoft.com/en-us/library/aa383713.aspx
		LARGE_INTEGER mTicksPerSecond;
		LARGE_INTEGER mLastTick;
		LARGE_INTEGER mCurrentTick;

		float mElapsedTime;
		float mTotalTime;

		float mLastUpdateTime;
		float mFrameSinceLastSecond;
		float mFramesPerSecond;
	};
}