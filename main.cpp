//

#include "UartUser.h"
#include "Ethernet.h"


#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <unistd.h>

#include <signal.h>

void handler(int sig)
{
    std::cout << "get signal!!!!!!!!" << std::endl;
}

using namespace LinuxSerial;

int main(int argc, char *argv[])
{

    // if (signal(SIGINT, handler) == SIG_ERR)
    // {
    //     perror("signal");
    // }
    // else
    // {
    //     std::cout << "regist successful " << std::endl;
    // }

    UartUser *uartUser = new UartUser();
    
    //Ethernet *ethernet = new Ethernet();

    while(true)
    {
        sleep(1);
    }
    // pause();
    sleep(20);

    //delete ethernet;
    //ethernet = nullptr;
    
    delete uartUser;
    uartUser = nullptr;

    // sleep(20);

    return EXIT_SUCCESS;
}