#ifndef _SENSOR_COMMAND_HANDLER_H_
#define _SENSOR_COMMAND_HANDLER_H_

#include "DataHandler.h"

// Forward declaration
class CoreController;

#define SENSOR_CONNECTION_INFO "CI"

class SensorCommandHandler : DataHandler
{
public:
    SensorCommandHandler(CoreController* controller);
    ~SensorCommandHandler() = default;
    void ParseData(vector<std::string> data, std::string originalData) override;

private:
    CoreController* mController;
};

#endif // _SENSOR_COMMAND_HANDLER_H_
