#include "CommandDataHandler.h"

#include <iostream>
#include <string.h>

void CommandDataHandler::ParseData(std::vector<std::string> data)
{
    string command = data[0];

    // compare command with command
    if(command.compare(std::string(COMMAND_SET_COM_PARAMETER)) == 0)
    {
        if(data.size() == 1)
        {
            
        }
        std::cout << "COMMAND_SET_COM_PARAMETER" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_AUTO_CHECK)) == 0)
    {
        std::cout << "COMMAND_AUTO_CHECK" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_HELP)) == 0)
    {
        std::cout << "COMMAND_HELP" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_EQUIPMENT_ZONE_NUMBER)) == 0)
    {
        std::cout << "COMMAND_EQUIPMENT_ZONE_NUMBER" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_SERVICE_TYPE)) == 0)
    {
        std::cout << "COMMAND_SERVICE_TYPE" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_EQUIPMENT_BIT)) == 0)
    {
        std::cout << "COMMAND_EQUIPMENT_BIT" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_EQUIPMENT_ID)) == 0)
    {
        std::cout << "COMMAND_EQUIPMENT_ID" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_LATITUDE)) == 0)
    {
        std::cout << "COMMAND_LATITUDE" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_LONGITUDE)) == 0)
    {
        std::cout << "COMMAND_LONGITUDE" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_DATE)) == 0)
    {
        std::cout << "COMMAND_DATE" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_TIME)) == 0)
    {
        std::cout << "COMMAND_TIME" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_DATE_AND_TIME)) == 0)
    {
        std::cout << "COMMAND_DATE_AND_TIME" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_FACILITY_TIME_INTERVAL)) == 0)
    {
        std::cout << "COMMAND_FACILITY_TIME_INTERVAL" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_DOWNLOAD_HISTORY)) == 0)
    {
        std::cout << "COMMAND_DOWNLOAD_HISTORY" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_READ_DATA)) == 0)
    {
        std::cout << "COMMAND_READ_DATA" << std::endl;
    }
    else if(command.compare(std::string(COMMAND_SET_COM_WAY)) == 0)
    {
        std::cout << "COMMAND_SET_COM_WAY" << std::endl;
    }
    else
    {
        std::cout << COMMAND_BADCOMMAND << std::endl;
    }
}
