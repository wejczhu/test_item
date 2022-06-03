// // Authors: Wei Jianxing <Jx.Wei@outlook.com>

#include "DataStorageUnit.h"
#include <iostream>

DataStorageUnit::DataStorageUnit()
{
    Initialize();
}

DataStorageUnit::~DataStorageUnit()
{
    sqlite3_close(mDatabase);
}

void DataStorageUnit::Initialize()
{
    int rc = sqlite3_open(DATA_BASE_PATH, &mDatabase);
    if (rc)
    {
        std::cout << "Can't open database: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
    }

    // Set database to Serialized mode
    // char* errorMsg = nullptr;
    // std::string sql = "PRAGMA journal_mode = WAL;";
    // rc = sqlite3_exec(mDatabase, sql.c_str(), nullptr, nullptr, &errorMsg);
    // if (rc != SQLITE_OK)
    // {
    //     std::cout << "Can't set database to Serialized mode: " << errorMsg << std::endl;
    //     sqlite3_free(errorMsg);
    // }

    // Create table climate data
    std::string sql = "CREATE TABLE IF NOT EXISTS climate_data (" \
                      "ID TEXT NO NULL," \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    rc = sqlite3_exec(mDatabase, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
    }
    {
        std::cout << "Table climate_data created successfully" << std::endl;
    }
}

void DataStorageUnit::InsertClimateData(std::string id, std::string time, std::string data, std::string filter)
{
    std::string sql = "INSERT INTO climate_data (ID, TIME, DATA, FILTER) VALUES ('" + id + "', '" + time + "', '" + data + "', " + filter + ")";
    int rc = sqlite3_exec(mDatabase, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
    }
    {
        std::cout << "Insert climate data successfully" << std::endl;
    }
}

std::vector<std::string> DataStorageUnit::GetClimateDataBetweenTime(std::string time1, std::string time2)
{
    std::string sql = "SELECT * FROM climate_data WHERE TIME BETWEEN '" + time1 + "' AND '" + time2 + "'";
    sqlite3_stmt* t_statement;
    int rc = sqlite3_prepare_v2(mDatabase, sql.c_str(), -1, &t_statement, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
    }

    // iterate over the columns to get the data
    std::vector<std::string> data;
    while (sqlite3_step(t_statement) == SQLITE_ROW)
    {
        auto temp = sqlite3_column_text(t_statement, 1);
        data.push_back(std::string(reinterpret_cast<const char*>(temp)));
    }

    return data;
}

std::vector<std::string> DataStorageUnit::GetClimateDataBetweenTime(std::string time1, std::string time2, std::string id)
{
    std::string sql = "SELECT * FROM climate_data WHERE TIME BETWEEN '" + time1 + "' AND '" + time2 + "' AND ID = " + id;
    sqlite3_stmt* t_statement;
    int rc = sqlite3_prepare_v2(mDatabase, sql.c_str(), -1, &t_statement, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
    }

    // iterate over the columns to get the data
    std::vector<std::string> data;
    while (sqlite3_step(t_statement) == SQLITE_ROW)
    {
        auto temp = sqlite3_column_text(t_statement, 1);
        data.push_back(std::string(reinterpret_cast<const char*>(temp)));
    }

    return data;
}

std::vector<std::string> DataStorageUnit::GetClimateDataBetweenTime(std::string time1, std::string time2, std::string id, std::string filter)
{
    std::string sql = "SELECT * FROM climate_data WHERE TIME BETWEEN '" + time1 + "' AND '" + time2 + "' AND ID = " + id + " AND FILTER = " + filter;
    sqlite3_stmt* t_statement;
    int rc = sqlite3_prepare_v2(mDatabase, sql.c_str(), -1, &t_statement, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
    }

    // iterate over the columns to get the data
    std::vector<std::string> data;
    while (sqlite3_step(t_statement) == SQLITE_ROW)
    {
        auto temp = sqlite3_column_text(t_statement, 1);
        data.push_back(std::string(reinterpret_cast<const char*>(temp)));
    }

    return data;
}


std::vector<std::string> DataStorageUnit::GetLatestClimateDataByFilter(std::string filter)
{
    std::string sql = "SELECT * FROM climate_data WHERE FILTER = " + filter + " ORDER BY TIME DESC LIMIT 1";
    sqlite3_stmt* t_statement;
    int rc = sqlite3_prepare_v2(mDatabase, sql.c_str(), -1, &t_statement, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
    }

    // iterate over the columns to get the data
    std::vector<std::string> data;
    while (sqlite3_step(t_statement) == SQLITE_ROW)
    {
        auto temp = sqlite3_column_text(t_statement, 1);
        data.push_back(std::string(reinterpret_cast<const char*>(temp)));
    }

    return data;
}

bool DataStorageUnit::GetLatestClimateDataByTimeAndId(std::string time, std::string id, std::vector<std::string>& data)
{
    // Replace last 2 character of time to *
    time = time.substr(0, time.size() - 2) + "**";
    std::string sql = "SELECT * FROM climate_data WHERE TIME = '" + time + "' AND ID = '" + id + "'";
    sqlite3_stmt* t_statement;
    int rc = sqlite3_prepare_v2(mDatabase, sql.c_str(), -1, &t_statement, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(mDatabase) << std::endl;
        sqlite3_close(mDatabase);
        return false;
    }

    // iterate over the columns to get the data
    while (sqlite3_step(t_statement) == SQLITE_ROW)
    {
        auto temp = sqlite3_column_text(t_statement, 1);
        data.push_back(std::string(reinterpret_cast<const char*>(temp)));
    }

    return true;
}

Json::Value DataStorageUnit::ReadJsonFile(const std::string& tag)
{
    Json::Value root;
    mJsonFile.open(DATA_STORAGE_UNIT_JSON_FILE_PATH);
    if (!mJsonFile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
        return NULL;
    }

    if (!mJsonReader.parse(mJsonFile, root))
    {
        std::cout << "Parse file failed!" << std::endl;
        return NULL;
    }

    //std::string info = mJsonRoot[tag].asString();
    mJsonFile.close();

    return root;
}

bool DataStorageUnit::WriteJsonFile(const std::string& tag, const std::string& info)
{
    mJsonFile.open(DATA_STORAGE_UNIT_JSON_FILE_PATH);
    if (!mJsonFile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
        return NULL;
    }

    if (!mJsonReader.parse(mJsonFile, mJsonRoot))
    {
        std::cout << "Parse file failed!" << std::endl;
        return NULL;
    }

    mJsonFile.close();
    mJsonRoot[tag] = info;

    Json::StyledWriter mJsonWriter;
    std::string json_str = mJsonWriter.write(mJsonRoot);

    std::ofstream outputFileStream(DATA_STORAGE_UNIT_JSON_FILE_PATH);

    outputFileStream << json_str;
    //writer -> write(rootJsonValue, &outputFileStream);
}

sqlite3* DataStorageUnit::GetDatabase()
{
    return mDatabase;
}
