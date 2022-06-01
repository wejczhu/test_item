#ifndef _DATA_STORAGE_UNIT_H_
#define _DATA_STORAGE_UNIT_H_

#include <string>
#include <fstream>
#include <sqlite3.h>

#include "json/json.h"

#define DATA_STORAGE_UNIT_JSON_FILE_PATH "/usr/bin/Info.json"
#define DATA_BASE_PATH "/usr/bin/data.db"

class DataStorageUnit
{
public:
    DataStorageUnit();
    ~DataStorageUnit();

    void Initialize();

    void InsertClimateData(std::string id, std::string time, std::string data, std::string filter);

    std::vector<std::string> GetClimateDataBetweenTime(std::string time1, std::string time2);
    std::vector<std::string> GetClimateDataBetweenTime(std::string time1, std::string time2, std::string id);
    std::vector<std::string> GetClimateDataBetweenTime(std::string time1, std::string time2, std::string id, std::string filter);


    bool GetLatestClimateDataByTimeAndId(std::string time, std::string id, std::vector<std::string>& data);
    std::vector<std::string> GetLatestClimateDataByFilter(std::string filter);

    Json::Value ReadJsonFile(const std::string& tag);

    bool WriteJsonFile(const std::string& tag, const std::string& info);

private:
    std::ifstream mJsonFile;
    Json::Value mJsonRoot;
    Json::Reader mJsonReader;
    Json::FastWriter mJsonWriter;

    sqlite3* mDatabase;
};





#endif // _DATA_STORAGE_UNIT_H_
