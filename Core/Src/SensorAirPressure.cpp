#include "SensorAirPressure.h"
#include <iostream>


SensorAirPressure::SensorAirPressure(std::string zoneNumber, std::string serviceType,
        std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(zoneNumber, serviceType, equipmentIdentification, equipmentId, dataStorageUnit)
{
    auto database = GetDataStorageUnit()->GetDatabase();
    // Create new table of database if not exist
    std::string sql = "CREATE TABLE IF NOT EXISTS Sensor001 (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "AGA TEXT NOT NULL," \
                      "AGAl TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    auto rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    {
        std::cout << "Table Sensor002 created successfully" << std::endl;
    }
}

SensorAirPressure::SensorAirPressure(std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(equipmentId, dataStorageUnit)
{
    auto database = GetDataStorageUnit()->GetDatabase();
    // Create new table of database if not exist
    std::string sql = "CREATE TABLE IF NOT EXISTS Sensor001 (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "AGA TEXT NOT NULL," \
                      "AGAl TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    auto rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    {
        std::cout << "Table Sensor002 created successfully" << std::endl;
    }
}

std::vector<std::string> SensorAirPressure::CalculateData(std::string startTime, std::string endTime)
{
    // Get data of current hour from database table Sensor001
    auto database = GetDataStorageUnit()->GetDatabase();

// AGA 整点本站气压 hPa 1 5
// AGAa 小时最高气压 hPa 1 5
// AGAb 小时最高气压对应时间 时分 0 4
// AGAc 小时最低气压 hPa 1 5
// AGAd 小时最低气压对应时间 时分 0 4

    std::string AGA;
    std::string AGAa;
    std::string AGAb;
    std::string AGAc;
    std::string AGAd;

    // Get latest AGA data between startTime and endTime
    std::string sql = "SELECT * FROM Sensor001 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' ORDER BY TIME DESC LIMIT 1";
    sqlite3_stmt* stmt;
    auto rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        AGA = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    }

    // Get max AGA value
    sql = "SELECT MAX(AGA) FROM Sensor001 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
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
            std::string AGAa = std::string((const char*)sqlite3_column_text(stmt, 2));
            std::cout << "Max AGA: " << AGAa << std::endl;
        }
    }

    // For max AGA value, get time
    sql = "SELECT TIME FROM Sensor001 WHERE AGA = '" + AGAa + "'";
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
            std::string AGAb = std::string((const char*)sqlite3_column_text(stmt, 2));
            std::cout << "Max time: " << AGAb << std::endl;
        }
    }

    // Get min AGAc value
    sql = "SELECT MIN(AGA) FROM Sensor001 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
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
            std::string AGAc = std::string((const char*)sqlite3_column_text(stmt, 2));
            std::cout << "Min AGAc: " << AGAc << std::endl;
        }
    }

    // For min AGAc value, get time
    sql = "SELECT TIME FROM Sensor001 WHERE AGA = '" + AGAc + "'";
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
            std::string AGAd = std::string((const char*)sqlite3_column_text(stmt, 2));
            std::cout << "Min time: " << AGAd << std::endl;
        }
    }

    std::vector<std::string> outputData;

    outputData.push_back("AGA");
    outputData.push_back(AGA);
    outputData.push_back("AGAa");
    outputData.push_back(AGAa);
    outputData.push_back("AGAb");
    outputData.push_back(AGAb);
    outputData.push_back("AGAc");
    outputData.push_back(AGAc);
    outputData.push_back("AGAd");
    outputData.push_back(AGAd);

    return outputData;
}

void SensorAirPressure::StoreData(std::vector<std::string> data, std::string originalData)
{
    std::string time = data[9];
    // Ignore second info of time, change second to 00
    // replace last char of time to 00
    time.replace(time.size() - 2, 2, "00");
    
    std::string filter = data[10];
    std::string dataAGA = data[14];
    std::string dataAGAl = data[16];
    auto database = GetDataStorageUnit()->GetDatabase();

    std::string sql = "INSERT INTO Sensor001 (TIME, DATA, AGA, AGAl, FILTER) VALUES ('" + time + "', '" + originalData + "', '" + dataAGA + "', '" + dataAGAl + "', " + filter + ")";
    int rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
}

std::string SensorAirPressure::GetQualityControlBit()
{
    return "0000";
}

std::string SensorAirPressure::GetQualityControlBit_5Min()
{
    return "0";
}

uint8_t SensorAirPressure::GetNumberOfMeasureElement()
{
    return 5;
}

uint8_t SensorAirPressure::GetNumberOfMeasureElement_5Min()
{
    return 1;
}

uint8_t SensorAirPressure::GetNumberOfEquipmentStatus()
{
    return 1;
}

std::vector<std::string> SensorAirPressure::GetStatusInfo(std::string startTime, std::string endTime)
{
    std::vector<std::string> statusInfo;

    statusInfo.push_back("y_AGA");
    statusInfo.push_back("0");

    return statusInfo;
}


std::vector<std::string> SensorAirPressure::CheckMissingData(std::string startTime, std::string endTime, std::string filter)
{
    std::cout << "Start to check missing data for sensor Sensor001" << std::endl;
    auto database = GetDataStorageUnit()->GetDatabase();
    // Get data from database between startTime and endTime and FILTER equal to filter
    std::string sql = "SELECT * FROM Sensor001 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' AND FILTER = '" + filter + "'";
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

    for(auto i : missData)
    {
        std::cout << "missdata: " << i << std::endl;
    }

    return missData;

}

void SensorAirPressure::Command_ReadData()
{

}

std::vector<std::string> SensorAirPressure::GetSensorData(std::string startTime, std::string endTime, std::string filter)
{
    std::vector<std::string> historyData;
    auto database = GetDataStorageUnit()->GetDatabase();

    // Get data from database between startTime and endTime and FILTER equal to filter
    std::string sql = "SELECT * FROM Sensor001 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' AND FILTER = '" + filter + "'";
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

std::vector<std::string> SensorAirPressure::CalculateData_5Min(std::string startTime, std::string endTime)
{
    
}