#include "Timer.h"
#include <iostream>

Timer::Timer(int minutes, std::function<void()> callback)
: mTimeInterval(minutes)
, mCallback(callback)
{
    mThread = new std::thread(&Timer::Run, this);
    mIsRunning = true;
    mIsTimeout = false;
}

Timer::~Timer()
{
    Terminate();
    delete mThread;
    mThread = nullptr;
}

void Timer::Run()
{
    while(mIsRunning)
    {
        std::this_thread::sleep_for(std::chrono::minutes(mTimeInterval));
        if(mIsRunning)
        {
            mIsTimeout = true;
            mCallback();
        }
        mIsTimeout = false;
    }
}

void Timer::StopTimer()
{
    mIsRunning = false;
}

void Timer::StartTimer()
{
    mIsRunning = true;
}

bool Timer::GetTimerStatus()
{
    return mIsTimeout;
}

void Timer::Terminate()
{
    if (!mIsRunning)
    {
        return;
    }

    mIsRunning = false;
    mThread->join();
}

int Timer::GetTimeInterval()
{
    return mTimeInterval;
}

void Timer::SetTimerInterval(int timeInterval)
{
    mTimeInterval = timeInterval;
}