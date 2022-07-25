// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#include "SensorHumidity.h"

SensorHumidity::SensorHumidity(std::string zoneNumber, std::string serviceType,
        std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit)
{

}

SensorHumidity::SensorHumidity(std::string equipmentId, DataStorageUnit* dataStorageUnit)
{
    auto database = GetDataStorageUnit()->GetDatabase();
    // Create new table of database if not exist
    std::string sql = "CREATE TABLE IF NOT EXISTS Sensor003 (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "ADA TEXT NOT NULL," \
                      "ADAl TEXT NOT NULL," \
                      "ADP TEXT NOT NULL," \
                      "ADPl TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    auto rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    else
    {
        std::cout << "Table Sensor003 created successfully" << std::endl;
    }
}

SensorHumidity::~SensorHumidity()
{

}

void SensorHumidity::StoreData(std::vector<std::string> data, std::string originalData)
{
    std::string time = data[9];
    // Ignore second info of time, change second to 00
    // replace last char of time to 00
    time.replace(time.size() - 2, 2, "00");
    
    std::string filter = data[10];
    std::string dataADA = data[14];
    std::string dataADAl = data[16];
    std::string dataADP = data[18];
    std::string dataADPl = data[20];
    auto database = GetDataStorageUnit()->GetDatabase();

    std::string sql = "INSERT INTO Sensor003 (TIME, DATA, ADA, ADAl, ADP, ADPl, FILTER) VALUES ('" + time + "', '" + originalData + "', '" + dataADA + "', '" + dataADAl + "', '" + dataADP + "', '" + dataADPl + "', " + filter + ")";
    int rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
}

std::vector<std::string> SensorHumidity::CalculateData(std::string startTime, std::string endTime)
{
    // Get data of current hour from database table Sensor003
    auto database = GetDataStorageUnit()->GetDatabase();

    std::string ADA;
    std::string ADAc;
    std::string ADAd;
    std::string ADAi;

    // Get latest ADA data between startTime and endTime
    std::string sql = "SELECT * FROM Sensor003 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' ORDER BY TIME DESC LIMIT 1";
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
            ADA = "///";
        }
        else
        {
            ADA = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        }
    }

    // Get min ADA value
    sql = "SELECT MIN(ADA) FROM Sensor003 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
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
                ADAc = "///";
            }
            else
            {
                ADAc = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            }
        }
    }

    // For min ADA value, get time
    sql = "SELECT TIME FROM Sensor003 WHERE ADA = '" + ADAc + "'";
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
                ADAd = "///";
            }
            else
            {
                ADAd = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            }
        }
    }

    // Get average value of ADA
    sql = "SELECT AVG(ADA) FROM Sensor003 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
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
                ADAi = "///";
            }
            else
            {
                ADAi = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            }
        }
    }

    std::vector<std::string> outputData;

    outputData.push_back("ADA");
    outputData.push_back(ADA);
    outputData.push_back("ADAc");
    outputData.push_back(ADAc);
    outputData.push_back("ADAd");
    outputData.push_back(ADAd);
    outputData.push_back("ADAi");
    outputData.push_back(ADAi);

    std::cout << "Finish to calculate 1 hour data for sensor 003" << std::endl;

    return outputData;
}

std::string SensorHumidity::GetQualityControlBit()
{
    return "0000";
}

std::string SensorHumidity::GetQualityControlBit_5Min()
{
    return "0";
}

uint8_t SensorHumidity::GetNumberOfMeasureElement()
{
    return 4;
}

uint8_t SensorHumidity::GetNumberOfMeasureElement_5Min()
{
    return 0;
}

uint8_t SensorHumidity::GetNumberOfEquipmentStatus()
{
    return 1;
}

std::vector<std::string> SensorHumidity::GetStatusInfo(std::string startTime, std::string endTime)
{
    std::vector<std::string> statusInfo;

    statusInfo.push_back("y_AAA");
    statusInfo.push_back("0");

    return statusInfo;
}

std::vector<std::string> SensorHumidity::CheckMissingData(std::string startTime, std::string endTime, std::string filter)
{
    std::cout << "Start to check missing data for sensor Sensor003" << std::endl;
    auto database = GetDataStorageUnit()->GetDatabase();
    // Get data from database between startTime and endTime and FILTER equal to filter
    std::string sql = "SELECT * FROM Sensor003 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' AND FILTER = '" + filter + "'";
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

void SensorHumidity::Command_ReadData()
{
    std::string command = "";
}

std::vector<std::string> SensorHumidity::GetSensorData(std::string startTime, std::string endTime, std::string filter)
{
    std::vector<std::string> historyData;
    auto database = GetDataStorageUnit()->GetDatabase();

    // Get data from database between startTime and endTime and FILTER equal to filter
    std::string sql = "SELECT * FROM Sensor003 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' AND FILTER = '" + filter + "'";
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

std::vector<std::string> SensorHumidity::CalculateData_5Min(std::string startTime, std::string endTime)
{
    // There is no 5 min data for sensor 003, return empty vector
    std::vector<std::string> data;
    return data;
}
