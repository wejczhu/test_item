
#ifndef _UART_USER_H_
#define _UART_USER_H_

#include "Uart.h"
#include <thread>
#include <map>

using namespace LinuxSerial;
using namespace std;
//using namespace Storage;



enum ClimateDataHeadE
{
    VERSION_NUMBER = 1,
    ZONE_NUMBER = 2,
    LONGITUDE = 3,
    LATITUDE = 4,
    HEIGHT = 5,
    SERVICE_TYPE = 6,
    EQUIPMENT_SYMBOL = 7,
    EQUIPMENT_ID = 8,
    MEASURE_TIME = 9,
    FRAME_SYMBOL = 10,
    MEASURE_ELEMENT_NUMBER = 11,
    EQUIPMENT_ELEMENT_NUMBER = 12,
    MAIN_DATA_START = 13
};

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


    struct ClimateDataHead
    {
        string mDataVersionNumber;
        string mDataZoneNumber;
        string mDataLongitude;
        string mDataLatitude;
        string mDataHeight;
        string mDataServiceType;
        string mDataEquipmentSymbol;
        string mDataEquipmentId;
        string mDataMeasureTime;
        string mDataFrameSymbol;
        string mDataMeasureElementNumber;
        string mDataEquipmentElementNumber;
        //string mDataMainData_start;
    };

    struct ClimateDataMain
    {
        map<string, string> mMeasureElement;
        string mQualityControl;
        map<string, int> mStateVariable;
    };

    

    UartUser();
    ~UartUser();

    void Run();

    void Terminate();

    void Initialize();

    void StoreData(std::string& data);

    void CreateDirectory(std::string path);

    std::vector<std::string> SplitDataFrame(std::string data);

    void ParseData(std::vector<std::string> data);

    void PrintDataHead();

    void PrintDataMain();

private:
    SerialPort* mSerialPort;
    std::thread* mThread;
    bool mIsRunning;
    FileDescriptor mFd;
    ClimateDataHead mClimateDataHead;
    ClimateDataMain mClimateDataMain;
    std::string mCheckNum;

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
