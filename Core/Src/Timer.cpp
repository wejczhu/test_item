#include "Timer.h"
#include <iostream>

Timer::Timer(int minutes, std::function<void()> callback)
: mTimeInterval(minutes)
, mCallback(callback)
{
    mThread = new std::thread(&Timer::Run, this);
    mIsRunning = true;
}

Timer::~Timer()
{
    Terminite();
    delete mThread;
    mThread = nullptr;
}

void Timer::Run()
{
    while(mIsRunning)
    {
        std::this_thread::sleep_for(std::chrono::minutes(mTimeInterval));
        std::cout << "Timer::Run()" << std::endl;
        this->mCallback();
    }
}

void Timer::Terminite()
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