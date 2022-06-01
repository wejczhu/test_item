#include "Sensor.h"

Sensor::Sensor(std::string zoneNumber, std::string serviceType, 
               std::string equipmentIdentification, std::string equipmentId)
: mZoneNumber(zoneNumber)
, mServiceType(serviceType)
, mEquipmentIdentification(equipmentIdentification)
, mEquipmentId(equipmentId)
{
    mIsActive = true;
    mAutoSend = true;
}

Sensor::Sensor(std::string equipmentId)
: mEquipmentId(equipmentId)
{
    mIsActive = true;
}


std::string Sensor::GetEquipmentId()
{
    return mEquipmentId;
}

std::string Sensor::GetZoneNumber()
{
    return mZoneNumber;
}

std::string Sensor::GetEquipmentIdentification()
{
    return mEquipmentIdentification;
}

std::string Sensor::GetServiceType()
{
    return mServiceType;
}

bool Sensor::GetIsAutoSend()
{
    return mAutoSend;
}
