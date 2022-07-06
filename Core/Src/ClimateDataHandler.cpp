#include "ClimateDataHandler.h"

#include <iostream>

ClimateDataHandler::ClimateDataHandler(CoreController* controller)
{
    mController = controller;
}

void ClimateDataHandler::HandleClimateData(std::vector<std::string> data, std::string originalData)
{
    // Check id number from data
    std::string sensorId = data[8];

    mController->SetLastSensor(sensorId);
    // Search for sensor id in sensor list
    Sensor* sensor = mController->GetSensorById(sensorId);
    if(sensor == nullptr)
    {
        std::cerr << "Id " << sensorId << " is not registered, this data will be ignored" << std::endl;
        return;
    }

    // Store data into sensor's database
    sensor->StoreData(data, originalData);
}

void ClimateDataHandler::HandleSensorCommand(std::vector<std::string> data, std::string originalData)
{

    std::cout << "handle sensor command" << std::endl;
    std::cout << "command: " << originalData << std::endl;
    auto firstSymbol = data.front().substr(0, 1);

    if(firstSymbol == SENSOR_COMMAND_HEADER_RESPONSE)
    {
        // This is response from sensor
        std::string command = data.front();
        // Get last 2 char of command
        command = command.substr(command.length() - 2);
        std::string sensorId = data.front().substr(1, 3);
        if(command == SENSOR_COMMAND_CONNECTION_INFO)
        {
            if(data.size() == 2)
            {
                // This is response from sensor
                if(data[1] == "F")
                {
                    // Failed to register the sensor
                    std::cerr << "Failed to register the sensor " << sensorId << std::endl;
                }
                else if(data[1] == "T")
                {
                    // Sensor is connected
                    // Register sensor
                    mController->RegisterSensor(sensorId);
                }

            }
            else
            {
                std::string time = data[1];
                std::string md5 = data[3];
                mController->HandleSensorConnectionRequest(sensorId, time, md5);
            }
        }
    }

    // if(command == SENSOR_COMMAND_CONNECTION_INFO)
    // {
    //     std::string time = data[1];
    //     std::string md5 = data[3];
    //     mController->HandleSensorConnectionRequest(sensorId, time, md5);
    // }
    // else if(command == SENSOR_COMMAND_READ_DATA)
    // {

    // }
    // else if(command == SENSOR_COMMAND_DATE_AND_TIME)
    // {

    // }
    // else
    // {
    //     std::cout << "invalid data:" << originalData << std::endl;
    // }
}


void ClimateDataHandler::ParseData(std::vector<std::string> data, std::string originalData)
{
    // Check if data is sensor command
    if (data.front() == "BG" && data.back() == "ED")
    {
        HandleClimateData(data, originalData);
    }
    else
    {
        HandleSensorCommand(data, originalData);
    }

    // mClimateDataHead.mDataVersionNumber = data[VERSION_NUMBER];
    // mClimateDataHead.mDataZoneNumber = data[ZONE_NUMBER];
    // mClimateDataHead.mDataLongitude = data[LONGITUDE];
    // mClimateDataHead.mDataLatitude = data[LATITUDE];
    // mClimateDataHead.mDataHeight = data[HEIGHT];
    // mClimateDataHead.mDataServiceType = data[SERVICE_TYPE];
    // mClimateDataHead.mDataEquipmentSymbol = data[EQUIPMENT_SYMBOL];
    // mClimateDataHead.mDataEquipmentId = data[EQUIPMENT_ID];
    // mClimateDataHead.mDataMeasureTime = data[MEASURE_TIME];
    // mClimateDataHead.mDataFrameSymbol = data[FRAME_SYMBOL];
    // mClimateDataHead.mDataMeasureElementNumber = data[MEASURE_ELEMENT_NUMBER];
    // mClimateDataHead.mDataEquipmentElementNumber = data[EQUIPMENT_ELEMENT_NUMBER];
    // //mClimateDataHead. = data[MAIN_DATA_START];

    // // Handle main data
    // std::vector<std::string> main_data;
    // int flag = 0;
    // main_data.assign(data.begin() + MAIN_DATA_START, data.end() - 2);
    // for(int i = 0; i < atoi(data[MEASURE_ELEMENT_NUMBER].c_str()) * 2; i += 2)
    // {
    //     mClimateDataMain.mMeasureElement.insert(pair<string, string>(main_data[i], main_data[i + 1]));
    //     flag += 2;
    // }

    // mClimateDataMain.mQualityControl = main_data[flag];
    // flag++;

    // for(int i = flag; i < atoi(data[EQUIPMENT_ELEMENT_NUMBER].c_str()); i += 2)
    // {
    //     mClimateDataMain.mMeasureElement.insert(pair<string, string>(main_data[i], main_data[i + 1]));
    //     flag += 2;
    // }

    // mCheckNum = main_data[flag];

    // PrintDataHead();
    // PrintDataMain();
    // std::cout << "-------------------------------------" << std::endl;
}

void ClimateDataHandler::PrintDataHead()
{
    std::cout << "Version Number: " << mClimateDataHead.mDataVersionNumber << std::endl;
    std::cout << "Zone Number: " << mClimateDataHead.mDataZoneNumber << std::endl;
    std::cout << "Longitude: " << mClimateDataHead.mDataLongitude << std::endl;
    std::cout << "Latitude: " << mClimateDataHead.mDataLatitude << std::endl;
    std::cout << "Height: " << mClimateDataHead.mDataHeight << std::endl;
    std::cout << "Service Type: " << mClimateDataHead.mDataServiceType << std::endl;
    std::cout << "Equipment Symbol: " << mClimateDataHead.mDataEquipmentSymbol << std::endl;
    std::cout << "Equipment Id: " << mClimateDataHead.mDataEquipmentId << std::endl;
    std::cout << "Measure Time: " << mClimateDataHead.mDataMeasureTime << std::endl;
    std::cout << "Frame Symbol: " << mClimateDataHead.mDataFrameSymbol << std::endl;
    std::cout << "Measure Element Number: " << mClimateDataHead.mDataMeasureElementNumber << std::endl;
    std::cout << "Equipment Element Number: " << mClimateDataHead.mDataEquipmentElementNumber << std::endl;
}

void ClimateDataHandler::PrintDataMain()
{
    std::cout << "Measure Element: " << std::endl;
    for(auto it = mClimateDataMain.mMeasureElement.begin(); it != mClimateDataMain.mMeasureElement.end(); it++)
    {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "Quality Control: " << mClimateDataMain.mQualityControl << std::endl;
}

// void ClimateDataHandler::StoreData(std::string time, std::string data, std::string filter)
// {
//     mController->GetDataStorageUnit()->InsertClimateData(time, data, filter);
// }
