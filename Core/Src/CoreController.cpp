#include "CoreController.h"

#include <sstream>
#include <fstream>
#include <functional>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

CoreController::CoreController()
: mClimateDataHandler(nullptr)
, mCommandDataHandler(nullptr)
, mStorageUnit(nullptr)
, mUartUserSensor(nullptr)
, mUartUserCommand(nullptr)
, mUartUserGps(nullptr)
, mUartUserGprs(nullptr)
, mTimer1Minute(nullptr)
, mTimer5Minute(nullptr)
, mTimer1Hour(nullptr)
, mTimerStorage(nullptr)
, mIsAutoSend(true)
, m1MinuteFinish(false)
, m5MinuteFinish(false)
, m1HourFinish(false)
, mStorageFinish(false)
, mTimeCalibrationFinish(false)
{
    mStorageUnit = new DataStorageUnit();

    mClimateDataHandler = new ClimateDataHandler(this);
    mCommandDataHandler = new CommandDataHandler(this);

    mUartUserSensor = new UartUser(mClimateDataHandler, "/dev/ttymxc2");
    mUartUserCommand = new UartUser(mCommandDataHandler, "/dev/ttymxc3");

    mTimer1Minute = new Timer(1, std::bind(&CoreController::OnTimeEvent_SensorData_1Min, this));
    mTimerStorage = new Timer(1, std::bind(&CoreController::OnTimeEvent_StorageData, this));
    mTimer1Hour = new Timer(1, std::bind(&CoreController::OnTimeEvent_SensorData_1Hour,this));
    //CreateDatabaseTable();
}

CoreController::~CoreController()
{
    delete mStorageUnit;
    delete mUartUserSensor;
    delete mUartUserCommand;
    delete mClimateDataHandler;
    delete mCommandDataHandler;

    delete mTimer1Minute;
    delete mTimerStorage;
    delete mTimer1Hour;

    mStorageUnit = nullptr;
    mUartUserSensor = nullptr;
    mUartUserCommand = nullptr;
    mClimateDataHandler = nullptr;
    mCommandDataHandler = nullptr;

    mTimer1Minute = nullptr;
    mTimerStorage = nullptr;
    mTimer1Hour = nullptr;
}

void CoreController::CreateDatabaseTable()
{
    auto database = GetDataStorageUnit()->GetDatabase();
    // Create new table of database if not exist

    std::string sql = "CREATE TABLE IF NOT EXISTS CoreController (" \
                      "TIME TEXT NOT NULL," \
                      "DATA TEXT NOT NULL," \
                      "FILTER INT NOT NULL)";

    int rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
    {
        std::cout << "Table CoreController created successfully" << std::endl;
    }
}

void CoreController::InsertData(std::string time, std::string data, std::string filter)
{
    auto database = GetDataStorageUnit()->GetDatabase();
    std::string id = "000";
    std::string sql = "INSERT INTO CoreController (ID, TIME, DATA, FILTER) VALUES ('" + id + "', '" + time + "', '" + data + "', " + filter + ")";
    auto rc = sqlite3_exec(database, sql.c_str(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        std::cout << "SQL error: " << sqlite3_errmsg(database) << std::endl;
        sqlite3_close(database);
    }
}

void CoreController::HandleSystemTime(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        std::string time = GetSystemTime();
        mUartUserCommand->SendData(time);
    }
    else
    {
        bool ret = SetSystemTime(command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleSystemDate(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        std::string date = GetSystemDate();
        mUartUserCommand->SendData(date);
    }
    else
    {
        bool ret = SetSystemDate(command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleSystemDateAndTime(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        std::string dateAndTime = GetSystemDateAndTime();
        mUartUserCommand->SendData(dateAndTime);
    }
    else
    {
        bool ret = SetSystemDate(command[1]);
        ret &= SetSystemTime(command[2]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

std::string CoreController::GetSystemTime()
{
    time_t now = time(0);
    struct tm *dt = localtime(&now);
    std::stringstream ss;
    std::stringstream hour;

    if(dt->tm_hour < 10)
    {
        hour << "0" << dt->tm_hour;
    }
    else
    {
        hour << dt->tm_hour;
    }

    std::stringstream minute;
    if(dt->tm_min < 10)
    {
        minute << "0" << dt->tm_min;
    }
    else
    {
        minute << dt->tm_min;
    }

    std::stringstream second;
    if(dt->tm_sec < 10)
    {
        second << "0" << dt->tm_sec;
    }
    else
    {
        second << dt->tm_sec;
    }

    ss << hour.str() << ":" << minute.str() << ":" << second.str();
    return ss.str();
}

std::string CoreController::GetSystemDate()
{
    time_t now = time(0);
    struct tm *dt = localtime(&now);
    std::stringstream ss;
    std::stringstream year;
    year << dt->tm_year + 1900;

    std::stringstream month;
    if(dt->tm_mon < 10)
    {
        month << "0" << dt->tm_mon + 1;
    }
    else
    {
        month << dt->tm_mon + 1;
    }

    std::stringstream day;
    if(dt->tm_mday < 10)
    {
        day << "0" << dt->tm_mday;
    }
    else
    {
        day << dt->tm_mday;
    }

    ss << year.str() << "-" << month.str() << "-" << day.str();
    return ss.str();
}

std::string CoreController::GetSystemDateAndTime()
{
    string date = GetSystemDate();
    string time = GetSystemTime();
    return date + "," + time;
}

bool CoreController::SetSystemTime(std::string newTime)
{
    std::string delimiter = ":";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> data_vector;
    while ((pos = newTime.find(delimiter)) != std::string::npos)
    {
        token = newTime.substr(0, pos);
        data_vector.push_back(token);
        newTime.erase(0, pos + delimiter.length());
    }
    data_vector.push_back(newTime);

    struct tm new_time;

    time_t now = time(0);
    struct tm *dt = localtime(&now);

    dt->tm_hour = std::stoi(data_vector[0]);
    dt->tm_min = std::stoi(data_vector[1]);
    dt->tm_sec = std::stoi(data_vector[2]);

    time_t t = mktime(dt);

    if (t != (time_t) -1)
    {
        int ret = stime(&t);
        if(ret == 0)
        {
            return true;
        }
    }

    return false;
}

bool CoreController::SetSystemDate(std::string date)
{
    std::string delimiter = "-";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> data_vector;
    while ((pos = date.find(delimiter)) != std::string::npos)
    {
        token = date.substr(0, pos);
        data_vector.push_back(token);
        date.erase(0, pos + delimiter.length());
    }
    data_vector.push_back(date);

    time_t now = time(0);
    struct tm *dt = localtime(&now);

    dt->tm_year = std::stoi(data_vector[0]) - 1900;
    dt->tm_mon = std::stoi(data_vector[1]) - 1;
    dt->tm_mday = std::stoi(data_vector[2]);

    time_t t = mktime(dt);

    if (t != (time_t) -1)
    {
        int ret = stime(&t);
        if(ret == 0)
        {
            return true;
        }
    }

    return false;
}

void CoreController::HandleSetCom(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        std::string baudRate = std::to_string(mUartUserCommand->GetSerialPort()->GetBaudRate());
        std::string numBit = std::to_string(mUartUserCommand->GetSerialPort()->GetNumDataBits());
        std::string parity = std::to_string(mUartUserCommand->GetSerialPort()->GetParity());
        std::string stopBit = std::to_string(mUartUserCommand->GetSerialPort()->GetNumStopBits());

        std::string totalInfo = baudRate + ", " + numBit + ", " + parity + ", " + stopBit;
        mUartUserCommand->SendData(totalInfo);
    }
}

void CoreController::ShowHelp()
{
    std::string helpInfo = "This function is work in progress";
    mUartUserCommand->SendData(helpInfo);
}

void CoreController::HandleEquipmentZoneNumber(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        auto value = mStorageUnit->ReadJsonFile("controller_config");
        std::string zoneNumber = value["controller_config"]["equipment_zone_number"].asString();
        mUartUserCommand->SendData(zoneNumber);
    }
    else
    {
        bool ret = mStorageUnit->WriteJsonFile("equipment_zone_number", command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleServiceType(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        auto value = mStorageUnit->ReadJsonFile("controller_config");
        std::string serviceType = value["controller_config"]["service_type"].asString();
        mUartUserCommand->SendData(serviceType);
    }
    else
    {
        bool ret = mStorageUnit->WriteJsonFile("service_type", command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleEquipmentBit(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        auto value = mStorageUnit->ReadJsonFile("controller_config");
        std::string bitrate = value["controller_config"]["equipment_bit"].asString();
        mUartUserCommand->SendData(bitrate);
    }
    else
    {
        bool ret = mStorageUnit->WriteJsonFile("equipment_bit", command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleEquipmentId(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        auto value = mStorageUnit->ReadJsonFile("controller_config");
        std::string equipmentId = value["controller_config"]["equipment_id"].asString();
        mUartUserCommand->SendData(equipmentId);
    }
    else
    {
        bool ret = mStorageUnit->WriteJsonFile("equipment_id", command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleLatitude(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        auto value = mStorageUnit->ReadJsonFile("controller_config");
        std::string latitude = value["controller_config"]["latitude"].asString();
        mUartUserCommand->SendData(latitude);
    }
    else
    {
        bool ret = mStorageUnit->WriteJsonFile("latitude", command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleLongitude(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
        auto value = mStorageUnit->ReadJsonFile("controller_config");
        std::string longitude = value["controller_config"]["longitude"].asString();
        mUartUserCommand->SendData(longitude);
    }
    else
    {
        bool ret = mStorageUnit->WriteJsonFile("longitude", command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
}

void CoreController::HandleHistoryDownload(std::vector<std::string> command)
{
    std::vector<std::string> history;
    if(command.size() == 5)
    {
        std::string startDate = command[1];
        std::string startTime = command[2];
        std::string start = RemoveNonNumeric(startDate + startTime);
        std::string endDate = command[3];
        std::string endTime = command[4];
        std::string end = RemoveNonNumeric(endDate + endTime);
        std::string filter = "001";
        
        std::vector<std::string> sensorData;
        for(auto sensor : mSensors)
        {
            sensorData = sensor.second->GetSensorData(start, end, filter);
            history.insert(history.end(), sensorData.begin(), sensorData.end());
            sensorData.clear();
        }

    }
    // Get the hour data
    else if(command.size() == 6)
    {
        std::string startDate = command[1];
        std::string startTime = command[2];
        std::string start = RemoveNonNumeric(startDate + startTime);
        std::string endDate = command[3];
        std::string endTime = command[4];
        std::string end = RemoveNonNumeric(endDate + endTime);
        std::string filter = command[5];

        if(filter == "005")
        {
            std::vector<std::string> sensorData;
            for(auto sensor : mSensors)
            {
                sensorData = sensor.second->GetSensorData(start, end, filter);
                history.insert(history.end(), sensorData.begin(), sensorData.end());
                sensorData.clear();
            }
        }
        else if(filter == "160")
        {
            history = mStorageUnit->GetClimateDataBetweenTime(start, end, filter);
        }
    }
    else
    {
        mUartUserCommand->SendData(COMMAND_BADCOMMAND);
        return;
    }

    for(auto data : history)
    {
        mUartUserCommand->SendData(data);
    }
}

void CoreController::HandleLatestData(std::vector<std::string> command)
{
    std::vector<std::string> history;
    if(command.size() == 1)
    {
        history = mStorageUnit->GetLatestClimateDataByFilter("001");
    }
    else if(command.size() == 2)
    {
        history = mStorageUnit->GetLatestClimateDataByFilter(command[1]);
    }
    else
    {
        mUartUserCommand->SendData(COMMAND_BADCOMMAND);
        return;
    }

    for(auto data : history)
    {
        mUartUserCommand->SendData(data);
    }
}

void CoreController::HandleFacilityTimeInterval(std::vector<std::string> command)
{
    if(command.size() == 1)
    {
    }
    else
    {
        int timeInterval = std::stoi(command[2]);
        if(command[1] == "001")
        {
            mTimer1Minute->SetTimerInterval(timeInterval);
        }
        else if(command[1] == "005")
        {
            mTimer5Minute->SetTimerInterval(timeInterval);
        }
        else if(command[1] == "160")
        {
            mTimer1Hour->SetTimerInterval(timeInterval);
        }
    }
}

void CoreController::TimeEventHandler(void)
{
    if(!mIsAutoSend)
    {
        return;
    }

    if(mTimer1Minute->GetTimerStatus())
    {
        mTimer1Minute->StopTimer();
        std::vector<std::string> history = mStorageUnit->GetLatestClimateDataByFilter("001");
        for(auto data : history)
        {
            mUartUserCommand->SendData(data);
        }
        mTimer1Minute->StartTimer();
    }
    else if(mTimer5Minute->GetTimerStatus())
    {
        mTimer5Minute->StopTimer();
        std::vector<std::string> history = mStorageUnit->GetLatestClimateDataByFilter("005");
        for(auto data : history)
        {
            mUartUserCommand->SendData(data);
        }
        mTimer5Minute->StartTimer();
    }
    else if(mTimer1Hour->GetTimerStatus())
    {
        mTimer1Hour->StopTimer();
        std::vector<std::string> history = mStorageUnit->GetLatestClimateDataByFilter("160");
        for(auto data : history)
        {
            mUartUserCommand->SendData(data);
        }
        mTimer1Hour->StartTimer();
    }
}

void CoreController::HandleSetComWay(std::vector<std::string> command)
{
    if(command.size() == 2)
    {
        if(command[1] == "1")
        {
            mIsAutoSend = true;
        }
        else if(command[1] == "0")
        {
            mIsAutoSend = false;
        }
    }
    else
    {
       // mUartUserCommand->SendData(std::string(int(mIsAutoSend)));
    }
}

void CoreController::OnTimeEvent_SensorData_1Min()
{
    // Get current time
    std::string CurrentTime = GetSystemTime();
    // Check if second is zero

    std::string second = CurrentTime.substr(CurrentTime.size() - 2, 2);
    std::string minute = CurrentTime.substr(CurrentTime.size() - 4, 2);
    std::string hour = CurrentTime.substr(CurrentTime.size() - 6, 2);

    if(!m1MinuteFinish)
    {
        if(second == "05" || second == "06")
        {
            std::cout << "start to collect data for 1 min" << std::endl;
            CollectData_1_Min();
            m1MinuteFinish = true;
        }
    }
    else
    {
        if(second == "57" || second == "58")
        {
            m1MinuteFinish = false;
        }
    }

    if(!mCheck1MinFinish)
    {
        if(second == "10" || second == "11")
        {
            // Get time of current minute
            std::string time = GetSystemDateAndTime();
            time = RemoveNonNumeric(time);

            std::string filter = "001";
            std::string startTime = time.substr(0, time.size() - 2) + "00";
            std::string endTime = time.substr(0, time.size() - 2) + "59";

            CheckMissingData(startTime, endTime, filter);

            mCheck1MinFinish = true;
        }
    }
    else
    {
        if(second == "50" || second == "51")
        {
            mCheck1MinFinish = false;
        }
    }
}

void CoreController::OnTimeEvent_SensorData_1Hour()
{
    // Get current time
    std::string currentTime = RemoveNonNumeric(GetSystemDateAndTime());

    // Check if second is zero
    std::string second = currentTime.substr(currentTime.size() - 2, 2);
    std::string minute = currentTime.substr(currentTime.size() - 4, 2);
    std::string hour = currentTime.substr(currentTime.size() - 6, 2);

    if(!m1HourFinish)
    {
        if(minute == "00" && (second == "05" || second == "06" || second == "07"))
        {
            std::cout << "Start to calculate 1 hour data" << std::endl;
            auto startTime = currentTime.substr(0, currentTime.size() - 4) + "0000";
            auto endTime = currentTime.substr(0, currentTime.size() - 4) + "5959";
            std::cout << "start time: " << startTime << std::endl;
            std::cout << "end time : " << endTime << std::endl;
            std::string climateData = GenerateClimateMessage(startTime, endTime);
            mUartUserCommand->SendData(climateData);

            // Store 1 hour data into database
            InsertData(currentTime, climateData, "160");
            
            m1HourFinish = true;
        }
    }
    else
    {
        if(second == "59")
        {
            m1HourFinish = false;
        }
    }
}

void CoreController::OnTimeEvent_StorageData()
{
    // Get current time
    std::string CurrentTime = GetSystemTime();
    // Check if second is zero
    
    std::string second = CurrentTime.substr(CurrentTime.size() - 2, 2);
    std::string minute = CurrentTime.substr(CurrentTime.size() - 5, 2);
    std::string hour = CurrentTime.substr(CurrentTime.size() - 8, 2);

    //std::cout << CurrentTime << std::endl;
    if(!mStorageFinish)
    {
        if(hour == "20" && minute == "00" && ( second == "00" || second == "01" || second == "02"))
        {
            // Store data to txt file
            std::cout << "start to store data " << std::endl;
            std::string currentTime = RemoveNonNumeric(GetSystemDate());
            std::string today = RemoveNonNumeric(GetSystemDate());

            std::string startTime = std::to_string(stoi(currentTime.c_str()) - 1) + "200000";
            std::string endTime = currentTime + "195959";

            for(auto sensor : mSensors)
            {
                std::string sensorId = sensor.first;
                std::vector<std::string> history_1_min = sensor.second->GetSensorData(startTime, endTime, "001");
                std::vector<std::string> history_5_min = sensor.second->GetSensorData(startTime, endTime, "005");
                std::vector<std::string> history_1_hour = sensor.second->GetSensorData(startTime, endTime, "160");

                std::string label = mStorageUnit->ReadJsonFile(sensorId)["sensors"][sensorId]["label"].asString();
                std::string directory_1_min = std::string(SD_CARD_MOUNT_DIR) + "/DATA/" + label + "/QC" + "/Minute/";
                std::string directory_5_min = std::string(SD_CARD_MOUNT_DIR) + "/DATA/" + label + "/QC" + "/5Minute/";
                std::string directory_1_hour = std::string(SD_CARD_MOUNT_DIR) + "/DATA/" + label + "/DE" + "/Hour/";
                std::string fileName_1_min = directory_1_min + label + "_" + sensorId + "_" + today  + "_1_min.txt";
                std::string fileName_5_min = directory_5_min + label + "_" + sensorId + "_" + today  + "_5_min.txt";
                std::string fileName_1_hour = directory_1_hour + label + "_" + sensorId + "_" + today  + "_Hour.txt"; 

                // Open file to w
                std::string command_create_directory_1_min = "mkdir -p " + directory_1_min;
                std::string command_create_directory_5_min = "mkdir -p " + directory_5_min;
                std::string command_create_directory_1_hour = "mkdir -p " + directory_1_hour;

                system(command_create_directory_1_min.c_str());
                system(command_create_directory_5_min.c_str());
                system(command_create_directory_1_hour.c_str());

                auto fd_1_min = open(fileName_1_min.c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
                if(fd_1_min < 0)
                {
                    perror("Open file error");
                }

                auto fd_5_min = open(fileName_5_min.c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
                if(fd_5_min < 0)
                {
                    perror("Open file error");
                }

                auto fd_1_hour = open(fileName_1_hour.c_str(), O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
                if(fd_1_hour < 0)
                {
                    perror("Open file error");
                }

                for(auto data : history_1_min)
                {
                    data = data + "\n";
                    write(fd_1_min, data.c_str(), data.size());
                }

                for(auto data : history_5_min)
                {
                    data = data + "\n";
                    write(fd_5_min, data.c_str(), data.size());
                }

                for(auto data : history_1_hour)
                {
                    data = data + "\n";
                    write(fd_1_hour, data.c_str(), data.size());
                }

                close(fd_1_min);
                close(fd_5_min);
                close(fd_1_hour);

                mStorageFinish = true;
                std::cout << "Finish to store daily data!" << std::endl;
            }
        }
    }
    else
    {
        if(hour == "20" && minute == "01" && second == "00")
        {
            mStorageFinish = false;
        }
    }
}

void CoreController::OnTimeEvent_Time_Calibration()
{
    // Get current time
    std::string CurrentTime = GetSystemTime();
    // Check if second is zero
    
    std::string second = CurrentTime.substr(CurrentTime.size() - 2, 2);
    std::string minute = CurrentTime.substr(CurrentTime.size() - 4, 2);
    std::string hour = CurrentTime.substr(CurrentTime.size() - 6, 2);

    if(!mTimeCalibrationFinish)
    {
        if(minute == "16" && (second == "21" || second == "22"))
        {
            // Get current time
            std::string currentDate = GetSystemDate();
            std::string currentTime = GetSystemTime();
            std::cout << "Send time calibration command" << std::endl;
            for(auto sensor : mSensors)
            {
                std::string command = SENSOR_COMMAND_HEADER_GENERAL + sensor.first + "," + currentDate + "," + currentTime;
                mUartUserSensor->SendData(command);
            }
        }
        mTimeCalibrationFinish = true;
    }
    else
    {
        if(minute == "16" && (second == "23" || second == "24"))
        {
            mTimeCalibrationFinish = false;
        }
    }
}

void CoreController::CheckMissingData(std::string startTime, std::string endTime, std::string filter)
{
    for(auto sensor : mSensors)
    {
        std::vector<std::string> missData;

        // Sensor level miss data check will ignore second.
        startTime = startTime.substr(0, startTime.size() - 2);
        endTime = endTime.substr(0, endTime.size() - 2);

        missData = sensor.second->CheckMissingData(startTime, endTime, filter);
        for(auto time : missData)
        {
            // Send command to sensor to ask for data
            std::string startTime = time + "00"; 
            std::string endTime = time + "59";
            std::cout << "sensor: " << sensor.first <<" missing data: " << startTime << " - " << endTime << std::endl;
        }

        // Send command to ask sensor to send the missing data
        //std::string command = 
    }
}

void CoreController::CollectData_1_Min(void)
{
    // If sensor working in waiting mode, send read data command to it.
    for(auto sensor : mSensors)
    {
        if(!sensor.second->GetIsAutoSend())
        {
            // Send read data command to this sensor
            std::string command = SENSOR_COMMAND_HEADER_GENERAL + sensor.first + SENSOR_COMMAND_READ_DATA;
            mUartUserCommand->SendData(command);
        }
    }
}

void CoreController::CollectData_5_Min(void)
{

}

void CoreController::HandleSensorConnectionRequest(std::string sensorId, std::string connectionTime, std::string requestMD5)
{
    // Get serial number by sensor id

    auto config = mStorageUnit->ReadJsonFile("core_controller");
    std::string serialNumber = config["sensors"][sensorId]["serial_number"].asString();
    int ascii = ConvertToASCII(sensorId) + ConvertToASCII(connectionTime) + ConvertToASCII(serialNumber);
    // get last 16 bit of string
    std::string tempMD5 = CalculateMD5Sum(std::to_string(ascii));
    std::string responseMD5 = tempMD5.substr(tempMD5.length() - 16, 16);

    // Check if requestMD5 equals to responseMD5
    if(requestMD5 == responseMD5)
    {
        // Send response to sensor
        double time = DATA_SEND_START_TIME + ((TOTAL_TRANSMIT_TIME - DATA_SEND_START_TIME) * std::stoi(sensorId)) / MAX_SENSOR_NUMBER;
        std::string responseInfo = SENSOR_COMMAND_HEADER_GENERAL + sensorId + SENSOR_COMMAND_CONNECTION_INFO + "," + "ok" + "," + "00000001" + "," + std::to_string(time);
        mUartUserSensor->SendData(responseInfo);
    }
    else
    {
        std::cout << "the request MD5 is not equal to the response MD5" << std::endl;
        std::cout << "response MD5: " << responseMD5 << std::endl;
        //mUartUserSensor->SendData("The request MD5 is not equal to response MD5");
    }
}

std::string CoreController::GenerateClimateMessage(std::string startTime, std::string endTime)
{

    std::vector<std::string> climateData;
    std::string Begin = "BG";
    std::string End = "ED";
    climateData.push_back(Begin);
    // Merge climateData and header
    std::cout << "1111111111111" << std::endl;
    auto header = GenerateClimateMessageHeader();
    climateData.insert(climateData.end(), header.begin(), header.end());
    std::cout << "222222222222" << std::endl;
    auto main = GenerateClimateMessageMain(startTime, endTime);
    std::cout << "33333333333333" << std::endl;
    climateData.insert(climateData.end(), main.begin(), main.end());

    // Calculate CRC
    std::string finalMessage;
    for(auto data : climateData)
    {
        finalMessage += data;
        finalMessage += ",";
    }

    std::string CRC = std::to_string(ConvertToASCII(finalMessage));
    finalMessage += CRC;
    finalMessage += ",";
    finalMessage += End;

    return finalMessage;
}

std::vector<std::string> CoreController::GenerateClimateMessageHeader()
{
    std::vector<std::string> header;
    auto value = mStorageUnit->ReadJsonFile("controller_config");

    std::string Header_Version_Number = value["version_number"].asString();
    std::string Header_Zone_Number = value["equipment_zone_number"].asString();
    std::string Header_Latitude = value["latitude"].asString();
    std::string Header_Longitude = value["longitude"].asString();
    std::string Header_Height = value["height"].asString();
    std::string Header_Service_Type = value["service_type"].asString();
    std::string Header_Equipment_Bit = value["equipment_bit"].asString();
    std::string Header_Equipment_Id = value["equipment_id"].asString();
    std::string Header_Time = RemoveNonNumeric(GetSystemDateAndTime());
    std::string Header_Frame = "160";

    header.push_back(Header_Version_Number);
    header.push_back(Header_Zone_Number);
    header.push_back(Header_Latitude);
    header.push_back(Header_Longitude);
    header.push_back(Header_Height);
    header.push_back(Header_Service_Type);
    header.push_back(Header_Equipment_Bit);
    header.push_back(Header_Equipment_Id);
    header.push_back(Header_Time);
    header.push_back(Header_Frame);

    // Calculate number of measure elements
    uint8_t numberOfMeasureElement = 0;
    for(auto sensor : mSensors)
    {
        numberOfMeasureElement += sensor.second->GetNumberOfMeasureElement();
    }

    header.push_back(std::to_string(numberOfMeasureElement));

    // 
    uint8_t numberOfEquipmentStatus = 0;
    for(auto sensor : mSensors)
    {
        numberOfEquipmentStatus += sensor.second->GetNumberOfEquipmentStatus();
    }
    header.push_back(std::to_string(numberOfEquipmentStatus));

    return header;
}

std::vector<std::string> CoreController::GenerateClimateMessageMain(std::string startTime, std::string endTime)
{
    std::vector<std::string> messageMain;
    std::cout << "444444444444444" << std::endl;
    std::vector<std::string> messageMain_Measurement = GenerateClimateMessage_Measurement(startTime, endTime);
    std::cout << "444444444444" << std::endl;
    std::vector<std::string> messageMain_StatusInfo = GenerateClimateMessage_StatusInfo(startTime, endTime);
    messageMain.insert(messageMain.end(), messageMain_Measurement.begin(), messageMain_Measurement.end());

    return messageMain;
}

std::vector<std::string> CoreController::GenerateClimateMessage_Measurement(std::string startTime, std::string endTime)
{
    std::vector<std::string> mainData;
    std::string qualityControl;

    for(auto sensor : mSensors)
    {
        std::cout << "5555555555555555" << std::endl;
        auto data = sensor.second->CalculateData(startTime, endTime);
        std::cout << "finishi to calculte data " << std::endl;
        mainData.insert(mainData.end(), data.begin(), data.end());
        qualityControl += sensor.second->GetQualityControlBit();
    }

    mainData.push_back(qualityControl);

    return mainData;
}

std::vector<std::string> CoreController::GenerateClimateMessage_StatusInfo(std::string startTime, std::string endTime)
{
    std::vector<std::string> statusInfo;
    for(auto sensor : mSensors)
    {
        auto data = sensor.second->GetStatusInfo(startTime, endTime);
        statusInfo.insert(statusInfo.end(), data.begin(), data.end());
    }
}

// Check if sensor is valid
bool CoreController::IsSensorValid(std::string registerInfo)
{
    return true;
}

std::string CoreController::CalculateMD5Sum(std::string originalData)
{

    std::string command = "echo -n " + originalData + " | md5sum > /tmp/md5.txt";
    std::string md5 = "";
    system(command.c_str());
    std::ifstream ifs("/tmp/md5.txt");
    ifs >> md5;
    return md5;
}

bool CoreController::AutoCheck()
{
    // Todo: check if sensor is valid
    return true;
}

void CoreController::RegisterSensor(std::string sensorId)
{
    // Check if sensor is already registered
    if(IfSensorExist(sensorId))
    {
        std::cout << "The sensor " << sensorId  << " is already registered" << std::endl;
        return;
    }

    Sensor* sensor = nullptr;
    if(sensorId == "001")
    {
        sensor = new SensorAirPressure(sensorId, mStorageUnit);
    }
    else if(sensorId == "002")
    {
        sensor = new SensorTemperature(sensorId, mStorageUnit);
    }
    else 
    {
        std::cout << "The sensor " << sensorId << " is not supported" << std::endl;
    }

    std::cout << "register sensor: " << sensorId << std::endl;
    mSensors[sensorId] = sensor;
}

int CoreController::ConvertToASCII(string letter)
{
    int ascii = 0;
    for (int i = 0; i < letter.length(); i++)
    {
        char x = letter.at(i);
        ascii += x;
    }

    return ascii;
}

std::string CoreController::RemoveNonNumeric(std::string str)
{
    std::string result = "";
    for(int i = 0; i < str.length(); i++)
    {
        if(isdigit(str[i]))
        {
            result += str[i];
        }
    }
    return result;
}

void CoreController::SendRegisterRequestToAllSensors()
{
    // Get sensor info from config file
    auto sensors = mStorageUnit->ReadJsonFile("sensors");
    for(auto sensor : sensors)
    {
        std::string sensorId = sensor["id"].asString();
        std::string registerCommand = SENSOR_COMMAND_HEADER_GENERAL + sensorId + SENSOR_COMMAND_CONNECTION_INFO;

        // Send request to sensor
        std::cout << "Send register request to sensor: " << sensorId << std::endl;
        mUartUserSensor->SendData(registerCommand);
    }
}

bool CoreController::IfSensorExist(std::string sensorId)
{
    auto it = mSensors.find(sensorId);
    if(it != mSensors.end())
    {
        return true;
    }

    return false;
}

DataStorageUnit* CoreController::GetDataStorageUnit()
{
    return mStorageUnit;
}

Sensor* CoreController::GetSensorById(std::string sensorId)
{
    // Find sensor id in mSensors
    auto it = mSensors.find(sensorId);
    if(it != mSensors.end())
    {
        return it->second;
    }

    return nullptr;
}
