#include "CoreController.h"
#include <sstream>
#include <functional>

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
{
    mStorageUnit = new DataStorageUnit();

    //mClimateDataHandler = new ClimateDataHandler(this);
    mCommandDataHandler = new CommandDataHandler(this);

    //mUartUserSensor = new UartUser(mClimateDataHandler);
    mUartUserCommand = new UartUser(mCommandDataHandler);

    mTimer1Minute = new Timer(1, std::bind(&CoreController::TimeEventHandler, this));
}

CoreController::~CoreController()
{
    delete mStorageUnit;
    //delete mUartUserSensor;
    delete mUartUserCommand;
    delete mClimateDataHandler;
    //delete mCommandDataHandler;

    mStorageUnit = nullptr;
    mUartUserSensor = nullptr;
    mUartUserCommand = nullptr;
    mClimateDataHandler = nullptr;
    mCommandDataHandler = nullptr;
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
    ss << dt->tm_hour << ":" << dt->tm_min << ":" << dt->tm_sec;
    return ss.str();
}

std::string CoreController::GetSystemDate()
{
    time_t now = time(0);
    struct tm *dt = localtime(&now);
    std::stringstream ss;
    ss << dt->tm_year + 1900 << "-" << dt->tm_mon + 1 << "-" << dt->tm_mday;
    return ss.str();
}

std::string CoreController::GetSystemDateAndTime()
{
    time_t now = time(0);
    struct tm *dt = localtime(&now);
    std::stringstream ss;
    ss << dt->tm_year + 1900 << "-" << dt->tm_mon + 1 << "-" << dt->tm_mday << ", " << dt->tm_hour << ":" << dt->tm_min << ":" << dt->tm_sec;
    return ss.str();
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
        std::string zoneNumber = mStorageUnit->ReadJsonFile("equipment_zone_number");
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
        std::string serviceType = mStorageUnit->ReadJsonFile("service_type");
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
        std::string bitrate = mStorageUnit->ReadJsonFile("equipment_bit");
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
        std::string equipmentId = mStorageUnit->ReadJsonFile("equipment_id");
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
        std::string latitude = mStorageUnit->ReadJsonFile("latitude");
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
        std::string longitude = mStorageUnit->ReadJsonFile("longitude");
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
        history = mStorageUnit->GetClimateDataBetweenTime(start, end);
    }
    else if(command.size() == 6)
    {
        std::string startDate = command[1];
        std::string startTime = command[2];
        std::string start = RemoveNonNumeric(startDate + startTime);
        std::string endDate = command[3];
        std::string endTime = command[4];
        std::string end = RemoveNonNumeric(endDate + endTime);
        std::string zoneNumber = command[5];
        history = mStorageUnit->GetClimateDataBetweenTime(start, end, zoneNumber);
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

void CoreController::HandleLastestData(std::vector<std::string> command)
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
    }
}

void CoreController::TimeEventHandler(void)
{
    std::cout << "TimeEventHandler is called!" << std::endl;
    mUartUserCommand->SendData("This is a test message!");
}

void CoreController::HandleSetComWay(std::vector<std::string> command)
{
    if(command.size() == 2)
    {
        bool ret = mStorageUnit->WriteJsonFile("com_way", command[1]);
        if(ret)
        {
            mUartUserCommand->SendData(SET_SUCCESS);
        }
        else
        {
            mUartUserCommand->SendData(SET_FAILURE);
        }
    }
    else
    {
        mUartUserCommand->SendData(COMMAND_BADCOMMAND);
    }
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



DataStorageUnit* CoreController::GetDataStorageUnit()
{
    return mStorageUnit;
}
