#include "ClimateDataHandler.h"

#include <iostream>

ClimateDataHandler::ClimateDataHandler(CoreController* controller)
{
    mController = controller;
}

void ClimateDataHandler::ParseData(std::vector<std::string> data, std::string originalData)
{
    // Check if data is valid
    if (data.front() != "BG" || data.back() != "ED\r\n")
    {
        if(data.front() == "!001CI" && (data.size() > 2))
        {
            mController->HandleSensorConnectionRequest(data);
        }
        else
        {
            std::cout << "Invalid data: " << originalData << std::endl;
        }

        return;
    }

    // Check id number from data
    std::string sensorId = data[8];

    // Search for sensor id in sensor list
    Sensor* sensor = mController->GetSensorById(sensorId);
    if(sensor == nullptr)
    {
        std::cerr << "Id " << sensorId << " is not registered, this data will be ignored" << std::endl;
        return;
    }

    // Store data into sensor's database
    sensor->StoreData(data, originalData);

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

void ParseSensorCommend(std::vector<std::string> data)
{
    // If data[0] last 2 char is "CI"
    std::string command = data[0].substr(data[0].length() - 2, 2);

    if(command == "CI")
    {
        // Check if sensor already exist
        std::string sensorId = data[0].substr(1, 3);
        if(!mController->IfSensorExist(sensorId))
        {
            std::string connectionTime = data[1];
            std::string requestMD5 = data[3].substr(0, data[3].length() - 1);
            mController->HandleSensorConnectionRequest(sensorId, connectionTime, requestMD5);
        }

        std::cout << "The sensor " << sensorId << " is already connected." << std::endl;
    }
    else if(command == "")
    {

    }
    else if(command == "")
    {

    }
    else
    {
        
    }

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

void ClimateDataHandler::StoreData(std::string time, std::string data, std::string filter)
{
    mController->GetDataStorageUnit()->InsertClimateData(time, data, filter);
}
