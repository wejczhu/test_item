#ifndef _SENSOR_TEMPERATURE_H_
#define _SENSOR_TEMPERATURE_H_

#include "Sensor.h"

class SensorTemperature : public Sensor
{
public:
    SensorTemperature(std::string zoneNumber, std::string serviceType,
            std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit);
    SensorTemperature(std::string equipmentId, DataStorageUnit* dataStorageUnit);
    ~SensorTemperature() = default;

    std::vector<std::string> CalculateData(std::string startTime, std::string endTime) override;
    std::string GetQualityControlBit() override;
    std::vector<std::string> GetStatusInfo(std::string startTime, std::string endTime) override;
    void StoreData(std::string data) override;
    uint8_t GetNumberOfMeasureElement() override;
    uint8_t GetNumberOfEquipmentStatus() override;
private:
    std::vector<std::string> mElements;
};

#endif // _SENSOR_TEMPERATURE_H_