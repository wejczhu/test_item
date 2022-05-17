#ifndef _TIMER_H_
#define _TIMER_H_

#include <thread>
#include <functional>

class Timer
{
public:
    // Create a timer with a given time and callback function
    Timer(int minutes, std::function<void()> callback);
    ~Timer();

    void Terminite();
    //void Start();
    //void Stop();
    //void Reset();
    //void PrintTime();
    void Run();

    int GetTimeInterval();
    void SetTimerInterval(int timeInterval);

private:
    int mTimeInterval;
    std::thread* mThread;
    // pointer to callback function
    std::function<void()> mCallback;
    bool mIsRunning;
};

#endif // _TIMER_H_
