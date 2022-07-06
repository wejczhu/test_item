#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

class Watchdog
{
public:
    Watchdog();
    ~Watchdog();
    void Start();
    void Stop();
    void Reset();
    void Feed();

private:
    static constexpr char* WATCHDOG_DEVICE = "/dev/watchdog";
    int mWatchdogFd;
};

#endif // _WATCHDOG_H_
