#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <string>

class Sensor
{
public:
    Sensor(std::string zoneNumber, std::string serviceType, 
           std::string equipmentIdentification, std::string equipmentId);
    Sensor(std::string equipmentId);
    ~Sensor() = default;

    std::string GetZoneNumber();
    std::string GetServiceType();
    std::string GetEquipmentIdentification();
    std::string GetEquipmentId();

    void SetZoneNumber(std::string);
    void SetServiceType(std::string);
    void SetEquipmentIdentification(std::string);
    void SetEquipmentId(std::string);

    bool GetIsAutoSend();

private:
    bool mIsActive;
    bool mAutoSend;

    std::string mZoneNumber;
    std::string mServiceType;
    std::string mEquipmentIdentification;
    std::string mEquipmentId;
};

#endif // _SENSOR_H_
