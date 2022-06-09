#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <string>
#include "DataStorageUnit.h"

class Sensor
{
public:
    Sensor(std::string zoneNumber, std::string serviceType, 
           std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit);
    Sensor(std::string equipmentId, DataStorageUnit* dataStorageUnit);
    virtual ~Sensor() = default;

    std::string GetZoneNumber();
    std::string GetServiceType();
    std::string GetEquipmentIdentification();
    std::string GetEquipmentId();

    void SetZoneNumber(std::string);
    void SetServiceType(std::string);
    void SetEquipmentIdentification(std::string);
    void SetEquipmentId(std::string);

    bool GetIsAutoSend();

    DataStorageUnit* GetDataStorageUnit();

    virtual std::vector<std::string> CalculateData(std::string startTime, std::string endTime) = 0;
    virtual std::vector<std::string> GetStatusInfo(std::string startTime, std::string endTime) = 0;
    virtual std::string GetQualityControlBit() = 0;
    virtual void StoreData(std::vector<std::string> data, std::string originalData) = 0;
    virtual void CheckMissingData(std::string startTime, std::string endTime, std::string filter) = 0;

    virtual uint8_t GetNumberOfMeasureElement() = 0;
    virtual uint8_t GetNumberOfEquipmentStatus() = 0;

private:
    bool mIsActive;
    bool mAutoSend;

    std::string mZoneNumber;
    std::string mServiceType;
    std::string mEquipmentIdentification;
    std::string mEquipmentId;

    DataStorageUnit* mStorageUnit;
};

#endif // _SENSOR_H_
