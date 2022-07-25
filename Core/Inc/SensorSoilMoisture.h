// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _SENSOR_SOIL_MOISTURE_H_
#define _SENSOR_SOIL_MOISTURE_H_

#include "Sensor.h"

class SensorSoilMoisture : public Sensor
{
public:
    SensorSoilMoisture(std::string zoneNumber, std::string serviceType,
            std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit);
    SensorSoilMoisture(std::string equipmentId, DataStorageUnit* dataStorageUnit);
    ~SensorSoilMoisture() = default;

    std::vector<std::string> CalculateData(std::string startTime, std::string endTime) override;
    std::string GetQualityControlBit() override;
    std::vector<std::string> GetStatusInfo(std::string startTime, std::string endTime) override;
    void StoreData(std::vector<std::string> data, std::string originalData) override;
    std::vector<std::string> CheckMissingData(std::string startTime, std::string endTime, std::string filter) override;
    uint8_t GetNumberOfMeasureElement() override;
    uint8_t GetNumberOfEquipmentStatus() override;
    std::vector<std::string> GetSensorData(std::string startTime, std::string endTime, std::string filter) override;
    std::vector<std::string> CalculateData_5Min(std::string startTime, std::string endTime) override;

    uint8_t GetNumberOfMeasureElement_5Min() override;
    std::string GetQualityControlBit_5Min() override;

    // Command
    void Command_ReadData() override;
private:
    std::vector<std::string> mElements;
};

#endif // _SENSOR_SOIL_MOISTURE_H_
