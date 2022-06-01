#include "CommandDataHandler.h"

#include <iostream>
#include <string.h>

CommandDataHandler::CommandDataHandler(CoreController* controller)
{
    mController = controller;
}

void CommandDataHandler::ParseData(std::vector<std::string> data, std::string originalData)
{
    string command = data[0];

    //Get last 2 char of command
    string last2char = command.substr(command.length() - 2, 2);

    // compare command with command
    if(last2char.compare(std::string()) == 0)
    {
        mController->HandleSetCom(data);
    }
    else if(command.compare(std::string(COMMAND_AUTO_CHECK)) == 0)
    {
        //mController->
    }
    else if(command.compare(std::string(COMMAND_HELP)) == 0)
    {
        mController->ShowHelp();
    }
    else if(command.compare(std::string(COMMAND_EQUIPMENT_ZONE_NUMBER)) == 0)
    {
        mController->HandleEquipmentZoneNumber(data);
    }
    else if(command.compare(std::string(COMMAND_SERVICE_TYPE)) == 0)
    {
        mController->HandleServiceType(data);
    }
    else if(command.compare(std::string(COMMAND_EQUIPMENT_BIT)) == 0)
    {
        mController->HandleEquipmentBit(data);
    }
    else if(command.compare(std::string(COMMAND_EQUIPMENT_ID)) == 0)
    {
        mController->HandleEquipmentId(data);
    }
    else if(command.compare(std::string(COMMAND_LATITUDE)) == 0)
    {
        mController->HandleLatitude(data);
    }
    else if(command.compare(std::string(COMMAND_LONGITUDE)) == 0)
    {
        mController->HandleLongitude(data);
    }
    else if(command.compare(std::string(COMMAND_DATE)) == 0)
    {
        mController->HandleSystemDate(data);
    }
    else if(command.compare(std::string(COMMAND_TIME)) == 0)
    {
        mController->HandleSystemTime(data);
    }
    else if(command.compare(std::string(COMMAND_DATE_AND_TIME)) == 0)
    {
        mController->HandleSystemDateAndTime(data);
    }
    else if(command.compare(std::string(COMMAND_FACILITY_TIME_INTERVAL)) == 0)
    {
        mController->HandleFacilityTimeInterval(data);
    }
    else if(command.compare(std::string(COMMAND_DOWNLOAD_HISTORY)) == 0)
    {
        mController->HandleHistoryDownload(data);
    }
    else if(command.compare(std::string(COMMAND_READ_DATA)) == 0)
    {
        mController->HandleLastestData(data);
    }
    else if(command.compare(std::string(COMMAND_SET_COM_WAY)) == 0)
    {
        mController->HandleSetComWay(data);
    }
    else if(command.compare("") == 0)
    {

    }
    else
    {
        std::cout << COMMAND_BADCOMMAND << std::endl;
    }
}
