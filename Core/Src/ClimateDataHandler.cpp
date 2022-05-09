#include "ClimateDataHandler.h"

#include <iostream>

ClimateDataHandler::ClimateDataHandler(CoreController* controller)
{
    mController = controller;
}

void ClimateDataHandler::ParseData(std::vector<std::string> data)
{
    // Check if data is valid
    if (data.front() != "BG" || data.back() != "ED")
    {
        std::cout << "Data is invalid, please double check" << std::endl;
        return;
    }

    mClimateDataHead.mDataVersionNumber = data[VERSION_NUMBER];
    mClimateDataHead.mDataZoneNumber = data[ZONE_NUMBER];
    mClimateDataHead.mDataLongitude = data[LONGITUDE];
    mClimateDataHead.mDataLatitude = data[LATITUDE];
    mClimateDataHead.mDataHeight = data[HEIGHT];
    mClimateDataHead.mDataServiceType = data[SERVICE_TYPE];
    mClimateDataHead.mDataEquipmentSymbol = data[EQUIPMENT_SYMBOL];
    mClimateDataHead.mDataEquipmentId = data[EQUIPMENT_ID];
    mClimateDataHead.mDataMeasureTime = data[MEASURE_TIME];
    mClimateDataHead.mDataFrameSymbol = data[FRAME_SYMBOL];
    mClimateDataHead.mDataMeasureElementNumber = data[MEASURE_ELEMENT_NUMBER];
    mClimateDataHead.mDataEquipmentElementNumber = data[EQUIPMENT_ELEMENT_NUMBER];
    //mClimateDataHead. = data[MAIN_DATA_START];

    // Handle main data
    std::vector<std::string> main_data;
    int flag = 0;
    main_data.assign(data.begin() + MAIN_DATA_START, data.end() - 2);
    for(int i = 0; i < atoi(data[MEASURE_ELEMENT_NUMBER].c_str()) * 2; i += 2)
    {
        mClimateDataMain.mMeasureElement.insert(pair<string, string>(main_data[i], main_data[i + 1]));
        flag += 2;
    }

    mClimateDataMain.mQualityControl = main_data[flag];
    flag++;

    for(int i = flag; i < atoi(data[EQUIPMENT_ELEMENT_NUMBER].c_str()); i += 2)
    {
        mClimateDataMain.mMeasureElement.insert(pair<string, string>(main_data[i], main_data[i + 1]));
        flag += 2;
    }

    mCheckNum = main_data[flag];

    PrintDataHead();
    PrintDataMain();
    std::cout << "-------------------------------------" << std::endl;
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
