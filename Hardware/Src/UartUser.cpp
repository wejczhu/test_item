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
    : mSerialPort(nullptr), mThread(nullptr), mFd()
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
        vector<string> data = SplitDataFrame(readBuffer);
        ParseData(data);

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

// Parse data frame from uart
std::vector<std::string> UartUser::SplitDataFrame(std::string data)
{
    // split data by comma
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> data_vector;
    while ((pos = data.find(delimiter)) != std::string::npos)
    {
        token = data.substr(0, pos);
        data_vector.push_back(token);
        data.erase(0, pos + delimiter.length());
    }
    data_vector.push_back(data);

    return data_vector;
}

void UartUser::ParseData(std::vector<std::string> data)
{
    // check if data is valid
    // TODO : remove \n
    if (data.front() != "BG" || data.back() != "ED\n")
    {
        std::cout << "data is not valid" << std::endl;
        std::cout << "111"  << data.front() << "1111" << std::endl;
        std::cout << "111" << data.back() << "111" << std::endl;
        return;
    }

    // VERSION_NUMBER = 1,
    // ZONE_NUMBER = 2,
    // LONGITUDE = 3,
    // LATITUDE = 4,
    // HEIGHT = 5,
    // SERVICE_TYPE = 6,
    // EQUIPMENT_SYMBOL = 7,
    // EQUIPMENT_ID = 8,
    // MEASURE_TIME = 9,
    // FRAME_SYMBOL = 10,
    // MEASURE_ELEMENT_NUMBER = 11,
    // EQUIPMENT_ELEMENT_NUMBER = 12,
    // MAIN_DATA_START = 13

    mClimateDataHead.mDataVersionNumber = data[VERSION_NUMBER];
    mClimateDataHead.mDataZoneNumber = data[ZONE_NUMBER];
    mClimateDataHead.mDataLongitude = data[LONGITUDE];
    mClimateDataHead.mDataLatitude = data[LATITUDE];
    mClimateDataHead.mDataHeight = data[HEIGHT];
    mClimateDataHead.mDataServiceType = data[SERVICE_TYPE];
    mClimateDataHead.mDataEquipmentSymbol = data[EQUIPMENT_SYMBOL];
    mClimateDataHead.mDataEquipmentId = data[EQUIPMENT_ID];
    mClimateDataHead.mDataMeasureTime = data[MEASURE_TIME];
    mClimateDataHead.mDataFrameSymbol = data[FRAME_SYMBOL];
    mClimateDataHead.mDataMeasureElementNumber = data[MEASURE_ELEMENT_NUMBER];
    mClimateDataHead.mDataEquipmentElementNumber = data[EQUIPMENT_ELEMENT_NUMBER];
    //mClimateDataHead. = data[MAIN_DATA_START];

    // Handle main data
    std::vector<std::string> main_data;
    int flag = 0;
    main_data.assign(data.begin() + MAIN_DATA_START, data.end() - 2);
    for(int i = 0; i < atoi(data[MEASURE_ELEMENT_NUMBER].c_str()) * 2; i += 2)
    {
        mClimateDataMain.mMeasureElement.insert(pair<string, string>(main_data[i], main_data[i + 1]));
        flag += 2;
    }

    mClimateDataMain.mQualityControl = main_data[flag];
    flag++;

    for(int i = flag; i < atoi(data[EQUIPMENT_ELEMENT_NUMBER].c_str()); i += 2)
    {
        mClimateDataMain.mMeasureElement.insert(pair<string, string>(main_data[i], main_data[i + 1]));
        flag += 2;
    }

    mCheckNum = main_data[flag];

    PrintDataHead();
    //std::cout << "-------------------------------------" << std::endl;
    PrintDataMain();
    std::cout << "-------------------------------------" << std::endl;
}

void UartUser::PrintDataHead()
{
    std::cout << "Version Number: " << mClimateDataHead.mDataVersionNumber << std::endl;
    std::cout << "Zone Number: " << mClimateDataHead.mDataZoneNumber << std::endl;
    std::cout << "Longitude: " << mClimateDataHead.mDataLongitude << std::endl;
    std::cout << "Latitude: " << mClimateDataHead.mDataLatitude << std::endl;
    std::cout << "Height: " << mClimateDataHead.mDataHeight << std::endl;
    std::cout << "Service Type: " << mClimateDataHead.mDataServiceType << std::endl;
    std::cout << "Equipment Symbol: " << mClimateDataHead.mDataEquipmentSymbol << std::endl;
    std::cout << "Equipment Id: " << mClimateDataHead.mDataEquipmentId << std::endl;
    std::cout << "Measure Time: " << mClimateDataHead.mDataMeasureTime << std::endl;
    std::cout << "Frame Symbol: " << mClimateDataHead.mDataFrameSymbol << std::endl;
    std::cout << "Measure Element Number: " << mClimateDataHead.mDataMeasureElementNumber << std::endl;
    std::cout << "Equipment Element Number: " << mClimateDataHead.mDataEquipmentElementNumber << std::endl;
}

void UartUser::PrintDataMain()
{
    std::cout << "Measure Element: " << std::endl;
    for(auto it = mClimateDataMain.mMeasureElement.begin(); it != mClimateDataMain.mMeasureElement.end(); it++)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "Quality Control: " << mClimateDataMain.mQualityControl << std::endl;
}