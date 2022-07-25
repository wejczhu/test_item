// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#include "Sensor.h"

#include <iostream>

Sensor::Sensor(std::string zoneNumber, std::string serviceType, 
               std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit)
: mZoneNumber(zoneNumber)
, mServiceType(serviceType)
, mEquipmentIdentification(equipmentIdentification)
, mEquipmentId(equipmentId)
, mStorageUnit(mStorageUnit)
{
    mIsActive = true;
    mAutoSend = true;
}

Sensor::Sensor(std::string equipmentId , DataStorageUnit* dataStorageUnit)
: mEquipmentId(equipmentId)
, mStorageUnit(dataStorageUnit)
{
    mIsActive = true;
    mAutoSend = true;
}


std::string Sensor::GetEquipmentId()
{
    return mEquipmentId;
}

std::string Sensor::GetZoneNumber()
{
    return mZoneNumber;
}

std::string Sensor::GetEquipmentIdentification()
{
    return mEquipmentIdentification;
}

std::string Sensor::GetServiceType()
{
    return mServiceType;
}

bool Sensor::GetIsAutoSend()
{
    return mAutoSend;
}

DataStorageUnit* Sensor::GetDataStorageUnit()
{
    return mStorageUnit;
}

void Sensor::Command_RegisterRequest()
{
    //std::string sensorId = GetEquipmentId();
    //std::string command = 
}

std::vector<std::string> Sensor::GetLatestData()
{
    std::vector<std::string> latestData;
    auto database = GetDataStorageUnit()->GetDatabase();

    // Get latest data from database
    std::string sql = "SELECT * FROM Sensor002 ORDER BY TIME DESC LIMIT 1";
    sqlite3_stmt* stmt;
    auto rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        latestData.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
    }

    return latestData;
}