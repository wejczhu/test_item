// Authors: Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _CORE_CONTROLLER_H_
#define _CORE_CONTROLLER_H_

//#include "UartUser.h"
#include "CommandDataHandler.h"
#include "ClimateDataHandler.h"
#include "UartUser.h"
#include "DataStorageUnit.h"
#include <time.h>

#include <iostream>

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
    


    void HandleLatitude(std::vector<std::string> command);
    void HandleLongitude(std::vector<std::string> command);


    void HandleHistoryDownload(std::vector<std::string> command);

    // void HandleEquipmentId(std::vector<std::string> command);
    // std::string GetEquipmentId();
    // void SetEquipmentId(std::string equipmentId);

    DataStorageUnit* GetDataStorageUnit();

    std::string RemoveNonNumeric(std::string str);

private:
    static CoreController *mInstance;

    // Command parameter
    std::string mEquipmentZoneNumber;
    std::string mEquipmentId;
    std::string mServiceType;
    std::string mLatitude;
    std::string mLongitude;

    ClimateDataHandler* mClimateDataHandler;
    CommandDataHandler* mCommandDataHandler;

    DataStorageUnit* mStorageUnit;

    // Four uart for GPS, GPRS, Sensor and Command
    UartUser* mUartUserSensor;
    UartUser* mUartUserCommand;
    UartUser* mUartUserGps;
    UartUser* mUartUserGprs;
};

#endif // _CORE_CONTROLLER_H_