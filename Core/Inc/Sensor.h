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

    virtual std::string CalculateData(std::string startTime, std::string endTime) = 0;
    virtual void StoreData(std::string data) = 0;

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
