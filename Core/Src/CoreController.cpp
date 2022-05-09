#include "CoreController.h"

#include <sstream>

CoreController::CoreController()
: mClimateDataHandler(nullptr)
, mCommandDataHandler(nullptr)
, mUartUserSensor(nullptr)
, mUartUserCommand(nullptr)
, mUartUserGps(nullptr)
, mUartUserGprs(nullptr)
{
    //mClimateDataHandler = new ClimateDataHandler(this);
    mCommandDataHandler = new CommandDataHandler(this);

    //mUartUserSensor = new UartUser(mClimateDataHandler);
    mUartUserCommand = new UartUser(mCommandDataHandler);
}

CoreController::~CoreController()
{
    //delete mUartUserSensor;
    delete mUartUserCommand;
    //delete mClimateDataHandler;
    delete mCommandDataHandler;

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
