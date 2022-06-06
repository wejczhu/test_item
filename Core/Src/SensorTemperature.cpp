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

std::vector<std::string> SensorTemperature::CalculateData(std::string startTime, std::string endTime)
{
    // Get data of current hour from database table 002
    auto dataBase = dataStorageUnit->GetDataBase();

    std::string AAA;
    std::string AAAa;
    std::string AAAb;
    std::string AAAc;
    std::string AAAd;
    std::string AAAi;

    // Get latest AAA data between startTime and endTime
    std::string sql = "SELECT * FROM 002 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "' ORDER BY TIME DESC LIMIT 1";
    rc = sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        AAA = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    }

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
            std::string AAAa = std::string((const char*)sqlite3_column_text(stmt, 0));
            std::cout << "Max AAA: " << AAA << std::endl;
        }
    }

    // For max AAA value, get time
    sql = "SELECT TIME FROM 002 WHERE AAA = '" + AAAa + "'";
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
            std::string AAAb = std::string((const char*)sqlite3_column_text(stmt, 0));
            std::cout << "Max time: " << AAAb << std::endl;
        }
    }

    // Get min AAAc value
    sql = "SELECT MIN(AAA) FROM 002 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
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
            std::string AAAc = std::string((const char*)sqlite3_column_text(stmt, 0));
            std::cout << "Min AAAc: " << AAAc << std::endl;
        }
    }


    // For min AAAc value, get time
    sql = "SELECT TIME FROM 002 WHERE AAA = '" + AAAc + "'";
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
            std::string AAAd = std::string((const char*)sqlite3_column_text(stmt, 0));
            std::cout << "Min time: " << AAAd << std::endl;
        }
    }

    // Get average AAA value
    sql = "SELECT AVG(AAA) FROM 002 WHERE TIME BETWEEN '" + startTime + "' AND '" + endTime + "'";
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
            std::string AAAi = std::string((const char*)sqlite3_column_text(stmt, 0));
            std::cout << "Average AAA: " << AAAi << std::endl;
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

    return outputData;
}
