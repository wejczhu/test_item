
#ifndef _UART_USER_H_
#define _UART_USER_H_

#include "Uart.h"
#include "DataHandler.h"

#include <thread>

using namespace LinuxSerial;
using namespace std;
//using namespace Storage;

class UartUser
{
public:
    struct FileDescriptor
    {
        FileDescriptor()
        : mStoreFd_1_min(0)
        , mStoreFd_5_min(0)
        , mStoreFd_1_hour(0)
        {

        }
        int mStoreFd_1_min;
        int mStoreFd_5_min;
        int mStoreFd_1_hour;
    };

    UartUser();
    ~UartUser();

    void Run();

    void Terminate();

    void Initialize();

    void StoreData(std::string& data);

    void CreateDirectory(std::string path);
private:
    SerialPort* mSerialPort;
    std::thread* mThread;
    bool mIsRunning;
    FileDescriptor mFd;
    DataHandler mDataHandler;

    // TODO:
    // Handle following data properly!
    std::string SD_CARD_DEVICE = "/run/media/mmcblk0p1/";
    std::string DATA_DIR_MINUTE = "Data/Cama/Minute/";
    std::string DATA_DIR_5_MINUTE = "Data/Cama/5_Minute/";
    std::string DATA_DIR_HOUR = "Data/Cama/Hour/";
    std::string DATA_FILE_MINUTE = "data_1_minute.txt";
    std::string DATA_FILE_5_MINUTE = "data_5_minute.txt";
    std::string DATA_FILE_HOUR = "data_1_hour.txt";

};

#endif // _UART_USER_H_
