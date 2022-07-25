// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#include "SensorLowWind.h"

#include <iostream>

SensorLowWind::SensorLowWind(std::string zoneNumber, std::string serviceType,
        std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(zoneNumber, serviceType, equipmentIdentification, equipmentId, dataStorageUnit)
{
    auto database = GetDataStorageUnit()->GetDatabase();
    // Create new table of database if not exist
    std::string sql = "CREATE TABLE IF NOT EXISTS Sensor006 (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "AAA TEXT NOT NULL," \
                      "AAAI TEXT NOT NULL," \
                      "AAA5i TEXT NOT NULL," \
                      "AAAl5i TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    auto rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    {
        std::cout << "Table Sensor006 created successfully" << std::endl;
    }
}

SensorLowWind::SensorLowWind(std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(equipmentId, dataStorageUnit)
{
    auto database = GetDataStorageUnit()->GetDatabase();
    // Create new table of database if not exist
    std::string sql = "CREATE TABLE IF NOT EXISTS Sensor006 (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "AAA TEXT NOT NULL," \
                      "AAAI TEXT NOT NULL," \
                      "AAA5i TEXT NOT NULL," \
                      "AAAl5i TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    auto rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    {
        std::cout << "Table Sensor006 created successfully" << std::endl;
    }

}

void SensorLowWind::StoreData(std::vector<std::string> data, std::string originalData)
{
    std::string time = data[9];
    // Ignore second info of time, change second to 00
    // replace last char of time to 00
    time.replace(time.size() - 2, 2, "00");
    
    std::string filter = data[10];
    std::string dataAAA = data[14];
    std::string dataAAAI = data[16];
    std::string dataAAA5i = data[18];
    std::string dataAAAl5i = data[20];
    auto database = GetDataStorageUnit()->GetDatabase();

    std::string sql = "INSERT INTO Sensor006 (TIME, DATA, AAA, AAAI, AAA5i, AAAl5i, FILTER) VALUES ('" + time + "', '" + originalData + "', '" + dataAAA + "', '" + dataAAAI + "', '" + dataAAA5i + "', '" + dataAAAl5i + "', " + filter + ")";
    int rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
}

std::vector<std::string> SensorLowWind::CalculateData(std::string startTime, std::string endTime)
{
    // Get data of current hour from database table Sensor006
    auto database = GetDataStorageUnit()->GetDatabase();

    std::string AAA;
    std::string AAAa;
    std::string AAAb;
    std::string AAAc;
    std::string AAAd;
    std::string AAAi;

    // Get latest AAA data between startTime and endTime
    std::string sql = "SELECT * FROM Sensor006 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' ORDER BY TIME DESC LIMIT 1";
    sqlite3_stmt* stmt;
    auto rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        if (sqlite3_column_text(stmt, 2) == NULL)
        {
            AAA = "///";
        }
        else
        {
            AAA = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        }
    }

    // Get max AAA value
    sql = "SELECT MAX(AAA) FROM Sensor006 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
    rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    else
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            if (sqlite3_column_text(stmt, 2) == NULL)
            {
                //std::string
                AAAa = "///";
            }
            else
            {
                AAAa = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            }
        }
    }

    // For max AAA value, get time
    sql = "SELECT TIME FROM Sensor006 WHERE AAA = '" + AAAa + "'";
    rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    else
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            if (sqlite3_column_text(stmt, 0) == NULL)
            {
                //std::string
                AAAb = "///";
            }
            else
            {
                AAAb = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            }
        }
    }

    // Get min AAAc value
    sql = "SELECT MIN(AAA) FROM Sensor006 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
    rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    else
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            if (sqlite3_column_text(stmt, 2) == NULL)
            {
                //std::string
                AAAc = "///";
            }
            else 
            {
                AAAc = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            }
        }
    }

    // For min AAAc value, get time
    sql = "SELECT TIME FROM Sensor006 WHERE AAA = '" + AAAc + "'";
    rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    else
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            if (sqlite3_column_text(stmt, 0) == NULL)
            {
                //std::string
                AAAd = "///";
            }
            else
            {
                AAAd = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            }
        }
    }

    // Get average AAA value
    sql = "SELECT AVG(AAA) FROM Sensor006 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
    rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    else
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            if (sqlite3_column_text(stmt, 0) == NULL)
            {
                //std::string
                AAAi = "///";
            }
            else
            {
                AAAi = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            }
        }
    }

    std::vector<std::string> outputData;

    outputData.push_back("AAA");
    outputData.push_back(AAA);
    outputData.push_back("AAAa");
    outputData.push_back(AAAa);
    outputData.push_back("AAAb");
    outputData.push_back(AAAb);
    outputData.push_back("AAAc");
    outputData.push_back(AAAc);
    outputData.push_back("AAAd");
    outputData.push_back(AAAd);
    outputData.push_back("AAAi");
    outputData.push_back(AAAi);

    std::cout << "Finish to calculate 1 hour data for sensor 002" << std::endl;

    return outputData;
}

std::string SensorLowWind::GetQualityControlBit()
{
    return "000000";
}

std::string SensorLowWind::GetQualityControlBit_5Min()
{
    return "0";
}

uint8_t SensorLowWind::GetNumberOfMeasureElement()
{
    return 6;
}

uint8_t SensorLowWind::GetNumberOfMeasureElement_5Min()
{
    return 1;
}

uint8_t SensorLowWind::GetNumberOfEquipmentStatus()
{
    return 1;
}

std::vector<std::string> SensorLowWind::GetStatusInfo(std::string startTime, std::string endTime)
{
    std::vector<std::string> statusInfo;

    statusInfo.push_back("y_AAA");
    statusInfo.push_back("0");

    return statusInfo;
}

std::vector<std::string> SensorLowWind::CheckMissingData(std::string startTime, std::string endTime, std::string filter)
{
    std::cout << "Start to check missing data for sensor Sensor006" << std::endl;
    auto database = GetDataStorageUnit()->GetDatabase();
    // Get data from database between startTime and endTime and FILTER equal to filter
    std::string sql = "SELECT * FROM Sensor006 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' AND FILTER = '" + filter + "'";
    sqlite3_stmt* stmt;
    auto rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }

    // iterate over the columns to get the data
    std::vector<std::string> data;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto temp = sqlite3_column_text(stmt, 0);
        data.push_back(std::string(reinterpret_cast<const char*>(temp)));
    }

    // Check if data is missing
    auto min = std::stoll(startTime);
    auto max = std::stoll(endTime);
    std::vector<std::string> missData;

    for(auto i = min; i <= max; i++)
    {
        // check if i is in data
        std::cout << "checking: " << i << std::endl;
        if (std::find(data.begin(), data.end(), std::to_string(i)) == data.end())
        {
            missData.push_back(std::to_string(i));
        }
    }

    return missData;
}

void SensorLowWind::Command_ReadData()
{
    std::string command = "";
}


std::vector<std::string> SensorLowWind::GetSensorData(std::string startTime, std::string endTime, std::string filter)
{
    std::vector<std::string> historyData;
    auto database = GetDataStorageUnit()->GetDatabase();

    // Get data from database between startTime and endTime and FILTER equal to filter
    std::string sql = "SELECT * FROM Sensor006 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' AND FILTER = '" + filter + "'";
    sqlite3_stmt* stmt;
    auto rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }

    // iterate over the columns to get the data
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string temp = std::string((const char*)sqlite3_column_text(stmt, 1));
        historyData.push_back(temp);
    }

    return historyData;
}

std::vector<std::string> SensorLowWind::CalculateData_5Min(std::string startTime, std::string endTime)
{
    auto database = GetDataStorageUnit()->GetDatabase();

    std::string AAA5i;

    // Get latest AAA5i data from database
    std::string sql = "SELECT * FROM Sensor006 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' ORDER BY TIME DESC LIMIT 1";
    sqlite3_stmt* stmt;
    auto rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Check if column 4 is empty or null
        if (sqlite3_column_text(stmt, 4) == NULL)
        {
            AAA5i = "///";
        }
        else
        {
            AAA5i = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        }
    }

    std::vector<std::string> outputData;

    outputData.push_back("AAA5i");
    outputData.push_back(AAA5i);

    return outputData;
}

