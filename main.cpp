//

#include "UartUser.h"
#include "Ethernet.h"
#include "CoreController.h"
#include "DataStorageUnit.h"

#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <unistd.h>

#include <signal.h>



using namespace LinuxSerial;

int main(int argc, char *argv[])
{


    CoreController* coreController = new CoreController();

    
    //Ethernet *ethernet = new Ethernet();

    while(true)
    {
        sleep(1);
    }
    // pause();
    sleep(20);

    //delete ethernet;
    //ethernet = nullptr;
    
    //delete uartUser;
    //uartUser = nullptr;

    delete coreController;
    coreController = nullptr;

    // sleep(20);

    return EXIT_SUCCESS;
}