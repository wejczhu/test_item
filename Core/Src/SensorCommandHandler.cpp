#include "SensorCommandHandler.h"

#include <string>

SensorCommandHandler::SensorCommandHandler(CoreController* controller)
{
    mController = controller;
}

void SensorCommandHandler::ParseData(vector<std::string> data, std::string originalData)
{
    std::string command = data[0];
    if(command.compare(std::string(SENSOR_CONNECTION_INFO)) == 0)
    {
        mController->HandleSensorConnectionRequest(data);
    }
}