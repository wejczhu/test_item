// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _TIMER_H_
#define _TIMER_H_

#include <thread>
#include <functional>

class Timer
{
public:
    // Create a timer with a given time and callback function
    Timer(int seconds, std::function<void()> callback);
    ~Timer();

    void Terminate();
    //void Start();
    //void Stop();
    //void Reset();
    //void PrintTime();
    void Run();

    int GetTimeInterval();
    void SetTimerInterval(int timeInterval);

    bool GetTimerStatus();

    void StartTimer();
    void StopTimer();

private:
    int mTimeInterval;
    bool mIsTimeout;
    std::thread* mThread;
    // Pointer to callback function
    std::function<void()> mCallback;
    bool mIsRunning;
};

#endif // _TIMER_H_
