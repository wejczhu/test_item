#include "SensorAirPressure.h"

SensorAirPressure::SensorAirPressure(std::string zoneNumber, std::string serviceType,
        std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(zoneNumber, serviceType, equipmentIdentification, equipmentId, DataStorageUnit* dataStorageUnit)
{

}

SensorAirPressure::SensorAirPressure(std::string equipmentId, DataStorageUnit* dataStorageUnit)
: Sensor(equipmentId, dataStorageUnit)
{

}


std::vector<std::string> SensorAirPressure::CalculateData(std::string startTime, std::string endTime)
{
    // Get data of current hour from database
    std::vector<std::string> historyData = mStorageUnit->GetClimateDataBetweenTime(startTime, endTime, "001", "001");

    return 0;
}

void SensorAirPressure::StoreData(std::vector<std::string> data, std::string originalData)
{

}

std::string SensorAirPressure::GetQualityControlBit()
{
    return "";
}

uint8_t SensorAirPressure::GetNumberOfMeasureElement()
{
    return 1;
}

uint8_t SensorAirPressure::GetNumberOfEquipmentStatus()
{
    return 1;
}

std::vector<std::string> SensorAirPressure::GetStatusInfo(std::string startTime, std::string endTime)
{
    std::vector<std::string> status;
    return status;
}


void SensorAirPressure::CheckMissingData(std::string startTime, std::string endTime, std::string filter)
{

}