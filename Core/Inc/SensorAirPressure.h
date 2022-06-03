#ifndef _SENSOR_AIR_PRESSURE_H_
#define _SENSOR_AIR_PRESSURE_H_

#include "Sensor.h"

class SensorAirPressure : public Sensor
{
public:
    SensorAirPressure(std::string zoneNumber, std::string serviceType,
            std::string equipmentIdentification, std::string equipmentId, DataStorageUnit* dataStorageUnit);
    SensorAirPressure(std::string equipmentId, DataStorageUnit* dataStorageUnit);
    ~SensorAirPressure() = default;

    std::string CalculateData() override;
private:
    std::vector<std::string> mElements;
};

#endif // _SENSOR_AIR_PRESSURE_H_
