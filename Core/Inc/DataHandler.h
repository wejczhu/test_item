// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_

#include <vector>
#include <map>
#include <string>

using namespace std;

#define COMMAND_BADCOMMAND  "Bad Command"
#define COMMAND_READ_DATA  "READDATA"
#define COMMAND_SET_COM_PARAMETER  "SETCOM"
#define COMMAND_SET_COM_WAY  "SETCOMWAY"
#define COMMAND_AUTO_CHECK  "AUTOCHECK"
#define COMMAND_HELP  "HELP"
#define COMMAND_EQUIPMENT_ZONE_NUMBER  "QZ"
#define COMMAND_SERVICE_TYPE  "ST"
#define COMMAND_EQUIPMENT_BIT  "DI"
#define COMMAND_EQUIPMENT_ID  "ID"
#define COMMAND_LATITUDE  "LAT"
#define COMMAND_LONGITUDE  "LONG"
#define COMMAND_DATE  "DATE"
#define COMMAND_TIME  "TIME"
#define COMMAND_DATE_AND_TIME  "DATETIME"
#define COMMAND_FACILITY_TIME_INTERVAL  "FTD"
#define COMMAND_DOWNLOAD_HISTORY  "DOWN"

#define COMMAND_RETURN_SUCCESS  "T"
#define COMMAND_RETURN_FAILURE  "F"


class DataHandler
{
public:
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



    /* data */
public:
    DataHandler() = default;
    ~DataHandler() = default;
    virtual void ParseData(vector<std::string> data, std::string originalData) = 0;

    std::vector<std::string> SplitDataFrame(std::string data);

private:
    ClimateDataHead mClimateDataHead;
    ClimateDataMain mClimateDataMain;
    std::string mCheckNum;
};

#endif // _DATA_HANDLER_H_
