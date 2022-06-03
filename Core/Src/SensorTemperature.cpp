#include "SensorTemperature.h"

SensorTemperature::SensorTemperature(std::string zoneNumber, std::string serviceType,
        std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(zoneNumber, serviceType, equipmentIdentification, equipmentId, dataStorageUnit)
{
    auto dataBase = dataStorageUnit->GetDataBase();
    // Create new table of database if not exist
    std::string sql = "CREATE TABLE IF NOT EXISTS 002 (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "AAA TEXT NOT NULL," \
                      "AAAI TEXT NOT NULL," \
                      "AAA5i TEXT NOT NULL," \
                      "AAAl5i TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    {
        std::cout << "Table climate_data created successfully" << std::endl;
    }
}

SensorTemperature::SensorTemperature(std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(equipmentId, dataStorageUnit)
{
    auto dataBase = dataStorageUnit->GetDataBase();
    // Create new table of database if not exist
    std::string sql = "CREATE TABLE IF NOT EXISTS 002 (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "AAA TEXT NOT NULL," \
                      "AAAI TEXT NOT NULL," \
                      "AAA5i TEXT NOT NULL," \
                      "AAAl5i TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    {
        std::cout << "Table climate_data created successfully" << std::endl;
    }

}


void SensorTemperature::StoreData(std::string data)
{
    auto dataBase = dataStorageUnit->GetDataBase();
    std::vector<std::string> elements = data.split(',');
    std::string time = elements[9];
    std::string filter = elements[10];
    std::string dataAAA = elements[14];
    std::string dataAAAI = elements[16];
    std::string dataAAA5i = elements[18];
    std::string dataAAAl5i = elements[20];
    std::string sql = "INSERT INTO 002 (TIME, DATA, AAA, AAAI, AAA5i, AAAl5i, FILTER) VALUES ('" + time + "', '" + data + "', '" + dataAAA + "', '" + dataAAAI + "', '" + dataAAA5i + "', '" + dataAAAl5i + "', " + filter + ")";
    int rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
}

std::string SensorTemperature::CalculateData(std::string startTime, std::string endTime)
{
    // Get data of current hour from database table 002
    auto dataBase = dataStorageUnit->GetDataBase();

    std::string AAA;
    std::string AAAa;
    std::string AAAb;
    std::string AAAc;
    std::string AAAi;

    // Get max AAA value
    std::string sql = "SELECT MAX(AAA) FROM 002 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
    sqlite3_stmt *stmt;
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
            std::string maxAAA = std::string((const char*)sqlite3_column_text(stmt, 0));
            std::cout << "Max AAA: " << maxAAA << std::endl;
        }
    }

    // For max AAA value, get time
    sql = "SELECT TIME FROM 002 WHERE AAA = '" + maxAAA + "'";
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
            std::string maxTime = std::string((const char*)sqlite3_column_text(stmt, 0));
            std::cout << "Max time: " << maxTime << std::endl;
        }
    }

    std::vector<int> temperature;
    for(auto data : historyData)
    {
        // Split data by comma
        std::vector<std::string> elements = Split(data, ',');
        temperature.push_back(std::stoi(elements[14]));
    }

    // Order temperature
    std::sort(temperature.begin(), temperature.end());



    return "";
}