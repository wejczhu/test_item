// Authors: Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _CORE_CONTROLLER_H_
#define _CORE_CONTROLLER_H_

//#include "UartUser.h"
#include "CommandDataHandler.h"
#include "ClimateDataHandler.h"
#include "UartUser.h"
#include "DataStorageUnit.h"
#include "Timer.h"
#include "Sensor.h"
#include "SensorAirPressure.h"
#include "SensorTemperature.h"

#include <time.h>

#include <iostream>
#include <map>

#define COMMAND_BADCOMMAND "Bad Command"
#define COMMAND_READ_DATA "READDATA"
#define COMMAND_SET_COM_PARAMETER "SETCOM"
#define COMMAND_SET_COM_WAY "SETCOMWAY"
#define COMMAND_AUTO_CHECK "AUTOCHECK"
#define COMMAND_HELP "HELP"
#define COMMAND_EQUIPMENT_ZONE_NUMBER "QZ"
#define COMMAND_SERVICE_TYPE "ST"
#define COMMAND_EQUIPMENT_BIT "DI"
#define COMMAND_EQUIPMENT_ID "ID"
#define COMMAND_LATITUDE "LAT"
#define COMMAND_LONGITUDE "LONG"
#define COMMAND_DATE "DATE"
#define COMMAND_TIME "TIME"
#define COMMAND_DATE_AND_TIME "DATETIME"
#define COMMAND_FACILITY_TIME_INTERVAL "FTD"
#define COMMAND_DOWNLOAD_HISTORY "DOWN"

#define SET_SUCCESS "T"
#define SET_FAILURE "F"


#define EQUIPMENT_AUTO_CHECK_STATUS "z"
#define SENSOR_ATMOSPHERIC_PRESSURE_STATUS "y_AGA"
#define SENSOR_AIR_TEMPERATURE_STATUS "y_AAA"
#define SENSOR_RELATE_HUMIDITY_STATUS "y_ADA"
#define SENSOR__RAIN_FORCE_STATUS "y_AHA"
#define SENSOR_HEAVY_RAIN_FORCE_STATUS "y_AHC"
#define SENSOR_1_POINT_5_WIND_STATUS "y_AWA150"
#define SENSOR_10_WIND_STATUS "y_AWA"
#define SENSOR_EARTH_SURFACE_TEMPERATURE_STATUS "y_ABB"
#define SENSOR_LAND_MOISTURE_STATUS "y_ARA"
#define SENSOR_LAND_TEMPERATURE_STATUS "y_AB"
#define SENSOR_RADIATION_STATUS "y_AJA"
#define BOARD_EXTERNAL_POWER_STATUS "xA"
#define BOARD_VOLTAGE_STATUS "xB"
#define BOARD_TEMPERATURE_STATUS "wA"
#define GPS_CONNECTION_STATUS "tP"
#define GPRS_WORK_STATUS "tQ"


#define SENSOR_COMMAND_HEADER_RESPONSE "!"
#define SENSOR_COMMAND_HEADER_GENERAL "#"
#define SENSOR_COMMAND_HEADER_SPECIAL "$"
#define SENSOR_COMMAND_HEADER_TRANSPARENT "&"
#define SENSOR_COMMAND_CONNECTION_INFO "CI"
#define SENSOR_COMMAND_READ_DATA "DM"
#define SENSOR_COMMAND_DATE_AND_TIME "DT"
#define SENSOR_COMMAND_DOWNLOAD_HISTORY "DO"



//#define SD_CARD_MOUNT_DIR "/run/media/mmcblk0"
#define SD_CARD_MOUNT_DIR "/tmp"


class UartUser;
class ClimateDataHandler;
class CommandDataHandler;

class CoreController
{
public:
    struct GpsDataS
    {
        double latitude;
        double longitude;
        double altitude;
        double speed;
        double heading;
        double hdop;
        double vdop;
        double pdop;
        double fix;
        double satellites;
        double time;
    };

    struct GprsDataS
    {
        double rssi;
        double ber;
        double rsrq;
    };

    struct SensorDataS
    {

    };

    CoreController();
    ~CoreController();

    CoreController &operator=(const CoreController &) = delete;
    CoreController(const CoreController &) = delete;

    static CoreController &GetInstance();

    void SetEquipmentId(const std::string &equipmentId);
    std::string GetEquipmentId();

    void SetServiceType(const std::string &serviceType);
    std::string GetServiceType();

    // Handle time related stuff
    void HandleSystemTime(std::vector<std::string> command);
    void HandleSystemDate(std::vector<std::string> command);
    void HandleSystemDateAndTime(std::vector<std::string> command);

    bool SetSystemTime(std::string time);
    bool SetSystemDate(std::string date);

    std::string GetSystemDate();
    std::string GetSystemTime();
    std::string GetSystemDateAndTime();

    void HandleSetCom(std::vector<std::string> command);

    void ShowHelp();

    void HandleEquipmentZoneNumber(std::vector<std::string> command);
    void HandleServiceType(std::vector<std::string> command);
    void HandleEquipmentBit(std::vector<std::string> command);
    void HandleEquipmentId(std::vector<std::string> command);
    
    void OnTimeEvent_SensorData_1Min();
    void OnTimeEvent_SensorData_1Hour();
    void OnTimeEvent_StorageData();
    void OnTimeEvent_Time_Calibration();



    void HandleLatitude(std::vector<std::string> command);
    void HandleLongitude(std::vector<std::string> command);


    void HandleHistoryDownload(std::vector<std::string> command);

    void HandleLatestData(std::vector<std::string> command);

    void HandleSetComWay(std::vector<std::string> command);

    void HandleFacilityTimeInterval(std::vector<std::string> command);

    // void HandleEquipmentId(std::vector<std::string> command);
    // std::string GetEquipmentId();
    // void SetEquipmentId(std::string equipmentId);

    DataStorageUnit* GetDataStorageUnit();

    std::string RemoveNonNumeric(std::string str);

    void TimeEventHandler(void);

    void CollectData_1_Min(void);
    void CollectData_5_Min(void);
    void CollectData_1_Hour(void);

    void RegisterSensor(std::string sensorId);
    bool IsSensorValid(std::string registerInfo);

    void HandleSensorConnectionRequest(std::string sensorId, std::string connectionTime, std::string requestMD5);
    std::string CalculateMD5Sum(std::string originalData);

    int ConvertToASCII(string letter);

    void SendRegisterRequestToAllSensors();
    bool IfSensorExist(std::string sensorId);

    void CheckMissingData(std::string startTime, std::string endTime, std::string filter);

    void CalculateHourClimateData(std::string startTime, std::string endTime);
    std::vector<std::string> GenerateClimateMessage_Measurement(std::string startTime, std::string endTime);
    std::vector<std::string> GenerateClimateMessage_StatusInfo(std::string startTime, std::string endTime);

    bool AutoCheck();

    void CreateDatabaseTable();
    void InsertData(std::string time, std::string data, std::string filter);

    std::string GenerateClimateMessage(std::string startTime, std::string endTime);
    std::vector<std::string> GenerateClimateMessageHeader();
    std::vector<std::string> GenerateClimateMessageMain(std::string startTime, std::string endTime);

    Sensor* GetSensorById(std::string sensorId);

    void OnTimeEvent_SensorData();
    void OnTimeEvent_SensorData_5Min();
    std::string GenerateClimateMessage_5Min(std::string startTime, std::string endTime);
    std::vector<std::string> GenerateClimateMessageMain_5Min(std::string startTime, std::string endTime);
    std::vector<std::string> GenerateClimateMessage_Measurement_5Min(std::string startTime, std::string endTime);
private:
    static CoreController *mInstance;

    Timer* mTimer1Minute;
    Timer* mTimer5Minute;
    Timer* mTimer1Hour;
    Timer* mTimerStorage;

    bool mIsAutoSend;
    bool m1MinuteFinish;
    bool m5MinuteFinish;
    bool m1HourFinish;
    bool mStorageFinish;
    bool mTimeCalibrationFinish;

    bool mCheck1MinFinish;

    // Command parameter
    std::string mEquipmentZoneNumber;
    std::string mEquipmentId;
    std::string mServiceType;
    std::string mLatitude;
    std::string mLongitude;

    ClimateDataHandler* mClimateDataHandler;
    CommandDataHandler* mCommandDataHandler;

    DataStorageUnit* mStorageUnit;

    std::string m1MinTimeInterval;
    std::string m5MinTimeInterval;
    std::string m1HourTimeInterval;

    // Four uart for GPS, GPRS, Sensor and Command
    UartUser* mUartUserSensor;
    UartUser* mUartUserCommand;
    UartUser* mUartUserGps;
    UartUser* mUartUserGprs;

    std::map<std::string, Sensor*> mSensors;

    const static uint8_t DATA_SEND_START_TIME = 5;
    const static uint8_t MAX_SENSOR_NUMBER = 10;
    const static uint8_t TOTAL_TRANSMIT_TIME = 10;
};

#endif // _CORE_CONTROLLER_H_
