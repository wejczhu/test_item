#include "Ethernet.h"
#include <iostream>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>

Ethernet::Ethernet()
{
    bool ret = Initialize();
    if(ret == false)
    {
        std::cout << "Ethernet Initialize failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    mThread = new std::thread(&Ethernet::Run, this);
    mIsRunning = true;
}

Ethernet::~Ethernet()
{
    Terminate();

    delete mThread;
    mThread = nullptr;
}

// Initialize socket and return true if success
bool Ethernet::Initialize()
{
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketFd == -1)
    {
        perror("socket");
        return false;
    }

    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    mServerAddr.sin_port = htons(mServerPort);
    //mServerAddr.sin_addr.s_addr = GetLocalIP();

    if(bind(mSocketFd, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr)) == -1)
    {
        perror("bind");
        return false;
    }

    if(listen(mSocketFd, 5) == -1)
    {
        perror("listen");
        return false;
    }

    mClientAddrLen = sizeof(mClientAddr);
    mClientFd = accept(mSocketFd, (struct sockaddr*)&mClientAddr, &mClientAddrLen);

    if(mClientFd == -1)
    {
        perror("accept");
        return false;
    }

    return true;
}

in_addr_t Ethernet::GetLocalIP()
{
    struct ifaddrs *ifAddrStruct = NULL;
    struct ifaddrs *ifa = NULL;
    void *tmpAddrPtr = NULL;

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if (strcmp(ifa->ifa_name, "eth1") == 0)
            {
                in_addr_t localip = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr.s_addr;
                std::cout << "get local ip address : " << localip << std::endl;
                return localip;
            }
        }
    }
    if (ifAddrStruct != NULL)
        freeifaddrs(ifAddrStruct);
    return 0;
}

void Ethernet::Run()
{
    while (mIsRunning)
    {
        // read(mClientFd, mBuffer, 1024);
        // std::cout << mBuffer << std::endl;

        // // Send back to client
        // int number = atoi(mBuffer);
        // ComCommandE content = static_cast<ComCommandE>(number);
        // switch (content)
        // {
        //     case ComCommandE::READDATA:
        //         std::cout <<"Read data from board" << std::endl;
        //         //Send("Read data", 10);
        //         break;
        //     case ComCommandE::SETCOM:
        //         //Send("Set com", 10);
        //         break;
        //     case ComCommandE::SETCOMWAY:
        //         //Send("Set com way", 13);
        //         break;
        //     case ComCommandE::AUTOCHECK:
        //         //Send("Auto check", 11);
        //         break;
        //     case ComCommandE::HELP:
        //         //Send("Help", 5);
        //         break;
        //     case ComCommandE::QZ:
        //         break;
        //     case ComCommandE::ST:
        //         break;
        //     case ComCommandE::DI:
        //         break;
        //     case ComCommandE::ID:
        //         break;
        //     case ComCommandE::LAT:
        //         break;
        //     case ComCommandE::LONG:
        //         break;
        //     case ComCommandE::DATE:
        //         break;
        //     case ComCommandE::TIME:
        //         break;
        //     case ComCommandE::DATETIME:
        //         break;
        //     case ComCommandE::FTD:
        //         break;

        //     case ComCommandE::DOWN:
        //         break;
        //     default:
        //         std::cout << "this is not a valid command, please check" << std::endl;
        //         break;
        // }

    }
}

void Ethernet::Terminate()
{
    if (!mIsRunning)
    {
        return;
    }

    mIsRunning = false;
    mThread->join();
}
