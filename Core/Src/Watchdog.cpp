// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#include "Watchdog.h"
#include <linux/watchdog.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>

Watchdog::Watchdog()
{
    // Open watchdog device
    mWatchdogFd = open(WATCHDOG_DEVICE, O_RDWR);
    if (mWatchdogFd < 0)
    {
        std::cout << "Error opening watchdog device" << std::endl;
    }
    else
    {
        // Set watchdog to fire after 2 second
        int timeout = 2;
        if (ioctl(mWatchdogFd, WDIOC_SETTIMEOUT, &timeout) < 0)
        {
            std::cout << "Error setting watchdog timeout" << std::endl;
        }
    }
}

Watchdog::~Watchdog()
{
    int ret = close(mWatchdogFd);
    if (ret < 0)
    {
        perror("close watchdog");
    }
}


void Watchdog::Start()
{
    int ret = ioctl(mWatchdogFd, WDIOC_SETOPTIONS, WDIOS_ENABLECARD);
    if (ret < 0)
    {
        perror("start watchdog");
    }
}

void Watchdog::Stop()
{
    int ret = ioctl(mWatchdogFd, WDIOC_SETOPTIONS, WDIOS_DISABLECARD);
    if (ret < 0)
    {
        perror("stop watchdog");
    }
}

void Watchdog::Reset()
{
    // Reset watchdog

}

void Watchdog::Feed()
{
    int ret = ioctl(mWatchdogFd, WDIOC_KEEPALIVE, NULL);
    if (ret < 0)
    {
        perror("feed watchdog");
    }
}