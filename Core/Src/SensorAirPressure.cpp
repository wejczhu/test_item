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


std::string SensorAirPressure::CalculateData(std::string startTime, std::string endTime)
{
    // Get data of current hour from database
    std::vector<std::string> historyData = mStorageUnit->GetClimateDataBetweenTime(startTime, endTime, "001", "001");

    return "";
}