#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <thread>

class Ethernet
{
public:
    Ethernet();
    ~Ethernet();

    bool Initialize();
    in_addr_t GetLocalIP();
    void Send(const char* data, int len);
    //void Receive(char* data, int len);

    void Run();
    void Terminate();
private:
    static Ethernet* mInstance;
    static constexpr int mServerPort = 8080;
    std::thread* mThread;
    bool mIsRunning;
    int mSocketFd;
    int mClientFd;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientAddrLen;
    char mBuffer[1024];
};

#endif // _ETHERNET_H_
