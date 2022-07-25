// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <string>
#include <algorithm>
#include "DataStorageUnit.h"

#define SENSOR_COMMAND_BADCOMMAND "Bad Command"
#define SENSOR_COMMAND_READ_DATA "READDATA"
#define SENSOR_COMMAND_SET_COM_PARAMETER "SETCOM"
#define SENSOR_COMMAND_SET_COM_WAY "SETCOMWAY"
#define SENSOR_COMMAND_AUTO_CHECK "AUTOCHECK"
#define SENSOR_COMMAND_HELP "HELP"
#define SENSOR_COMMAND_EQUIPMENT_ZONE_NUMBER "QZ"
#define SENSOR_COMMAND_SERVICE_TYPE "ST"
#define SENSOR_COMMAND_EQUIPMENT_BIT "DI"
#define SENSOR_COMMAND_EQUIPMENT_ID "ID"
#define SENSOR_COMMAND_LATITUDE "LAT"
#define SENSOR_COMMAND_LONGITUDE "LONG"
#define SENSOR_COMMAND_DATE "DATE"
#define SENSOR_COMMAND_TIME "TIME"
#define SENSOR_COMMAND_DATE_AND_TIME "DATETIME"
#define SENSOR_COMMAND_FACILITY_TIME_INTERVAL "FTD"
#define SENSOR_COMMAND_DOWNLOAD_HISTORY "DOWN"

#define SENSOR_COMMAND_SET_DEVICE "STDEV"
#define SENSOR_COMMAND_FACILITY_TIME "FT"

#define SENSOR_COMMAND_SET_PARAMETER "SS"
#define SENSOR_COMMAND_STATUS "STAT"
#define SENSOR_COMMAND_AUTO_TEMPERATURE "AT"
#define SENSOR_COMMAND_VOLTAGE_RANGE "VV"
#define SENSOR_COMMAND_SERIAL_NUMBER "SN"
#define SENSOR_COMMAND_QCPS "QCPS"
#define SENSOR_COMMAND_QCPM "QCPM"
#define SENSOR_COMMAND_CR "CR"
#define SENSOR_COMMAND_RESET "RESET"


class Sensor
{
public:
    Sensor(std::string zoneNumber, std::string serviceType, 
           std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit);
    Sensor(std::string equipmentId, DataStorageUnit* dataStorageUnit);
    virtual ~Sensor() = default;

    std::string GetZoneNumber();
    std::string GetServiceType();
    std::string GetEquipmentIdentification();
    std::string GetEquipmentId();

    void SetZoneNumber(std::string);
    void SetServiceType(std::string);
    void SetEquipmentIdentification(std::string);
    void SetEquipmentId(std::string);

    bool GetIsAutoSend();

    DataStorageUnit* GetDataStorageUnit();

    virtual std::vector<std::string> CalculateData(std::string startTime, std::string endTime) = 0;
    virtual std::vector<std::string> GetStatusInfo(std::string startTime, std::string endTime) = 0;
    virtual std::string GetQualityControlBit() = 0;
    virtual void StoreData(std::vector<std::string> data, std::string originalData) = 0;
    virtual std::vector<std::string> CheckMissingData(std::string startTime, std::string endTime, std::string filter) = 0;

    virtual uint8_t GetNumberOfMeasureElement() = 0;
    virtual uint8_t GetNumberOfEquipmentStatus() = 0;

    virtual uint8_t GetNumberOfMeasureElement_5Min() = 0;
    virtual std::string GetQualityControlBit_5Min() = 0;

    // Command for sensor
    virtual void Command_ReadData();
    virtual void Command_SetCom();
    virtual void Command_ShowHelp();
    virtual void Command_AutoCheck();
    virtual void Command_RegisterRequest();
    virtual void Command_QZ();

    virtual std::vector<std::string> GetSensorData(std::string startTime, std::string endTime, std::string filter) = 0;

    virtual std::vector<std::string> CalculateData_5Min(std::string startTime, std::string endTime) = 0;

    std::vector<std::string> GetLatestData(); 

private:
    bool mIsActive;
    bool mAutoSend;

    std::string mZoneNumber;
    std::string mServiceType;
    std::string mEquipmentIdentification;
    std::string mEquipmentId;

    DataStorageUnit* mStorageUnit;
};

#endif // _SENSOR_H_
