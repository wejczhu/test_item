#include "Sensor.h"

Sensor::Sensor(std::string zoneNumber, std::string serviceType, 
               std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit)
: mZoneNumber(zoneNumber)
, mServiceType(serviceType)
, mEquipmentIdentification(equipmentIdentification)
, mEquipmentId(equipmentId)
, mStorageUnit(mStorageUnit)
{
    mIsActive = true;
    mAutoSend = true;
}

Sensor::Sensor(std::string equipmentId , DataStorageUnit* dataStorageUnit)
: mEquipmentId(equipmentId)
, mStorageUnit(dataStorageUnit)
{
    mIsActive = true;
    mAutoSend = true;
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

DataStorageUnit* Sensor::GetDataStorageUnit()
{
    return mStorageUnit;
}