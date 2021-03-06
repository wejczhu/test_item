#include "UartUser.h"

#include <thread>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace LinuxSerial;
using namespace std;

void UartUser::Initialize()
{
    // Initialize uart
    mSerialPort = new SerialPort();
    mSerialPort->SetDevice("/dev/ttymxc2");
    mSerialPort->SetTimeout(-1);
    mSerialPort->Open();

    // Initialize thread
    mThread = new std::thread(&UartUser::Run, this);
    mIsRunning = true;

    // CreateDirectory(SD_CARD_DEVICE + DATA_DIR_MINUTE);
    // CreateDirectory(SD_CARD_DEVICE + DATA_DIR_5_MINUTE);
    // CreateDirectory(SD_CARD_DEVICE + DATA_DIR_HOUR);

    mFd.mStoreFd_1_min = open((SD_CARD_DEVICE + DATA_FILE_MINUTE).c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    mFd.mStoreFd_5_min = open((SD_CARD_DEVICE + DATA_FILE_5_MINUTE).c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    mFd.mStoreFd_1_hour = open((SD_CARD_DEVICE + DATA_FILE_HOUR).c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);

    if (mFd.mStoreFd_1_min == -1)
    {
        perror("Open SD-Card file 1_min");
    }

    if (mFd.mStoreFd_5_min == -1)
    {
        perror("Open SD-Card file 5_min");
    }

    if (mFd.mStoreFd_1_hour == -1)
    {
        perror("Open SD-Card file 1_hour");
    }
}

UartUser::UartUser()
: mSerialPort(nullptr)
, mThread(nullptr)
, mFd()
, mDataHandler()
{
    Initialize();
}

UartUser::~UartUser()
{
    Terminate();
    delete mSerialPort;
    mSerialPort = nullptr;
    delete mThread;
    mThread = nullptr;

    close(mFd.mStoreFd_1_min);
    close(mFd.mStoreFd_1_min);
    close(mFd.mStoreFd_1_hour);
}

void UartUser::Run()
{
    static std::string readBuffer;
    while (mIsRunning)
    {
        mSerialPort->Read(readBuffer);
        //std::cout << readBuffer << std::endl;
        vector<string> data = mDataHandler.SplitDataFrame(readBuffer);
        mDataHandler.ParseData(data);

        StoreData(readBuffer);
    }
}

void UartUser::Terminate()
{
    if (!mIsRunning)
    {
        return;
    }

    mIsRunning = false;
    mThread->join();
}

void UartUser::StoreData(std::string &data)
{
    int ret = write(mFd.mStoreFd_1_min, data.data(), data.length());
    ret = write(mFd.mStoreFd_5_min, data.data(), data.length());
    ret = write(mFd.mStoreFd_1_hour, data.data(), data.length());
    if (ret < 0)
    {
        perror("Write data");
    }
}

void UartUser::CreateDirectory(std::string path)
{
    // TODO:
    // Rewrite this function! This is not a stable solution!
    // std::string command = "mkdir -p " + path;
    // int ret = system(command.c_str());
    // perror("create dir");

    // if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
    // {
    //     if( errno == EEXIST ) {
    //     // already exists
    //     } else {
    //         std::cout << "error number: " << errno << std::endl;
    //         std::cerr << " can not create folder " << std::endl;
    //         //std::cout << "cannot create sessionnamefolder error:" << strerror(errno) << std::endl;
    //         //throw std::runtime_error( strerror(errno) );
    //     }
    // }
}
