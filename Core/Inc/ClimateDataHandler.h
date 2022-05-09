#ifndef _CLIMATE_DATA_HANDLER_H_
#define _CLIMATE_DATA_HANDLER_H_

#include "DataHandler.h"
#include "CoreController.h"

#include <vector>
#include <map>
#include <string>

class CoreController;

using namespace std;

class ClimateDataHandler: public DataHandler
{
public:
    ClimateDataHandler(CoreController* controller);
    ~ClimateDataHandler() = default;
    void ParseData(vector<std::string> data) override;
    
    void PrintDataHead();
    void PrintDataMain();

private:
    ClimateDataHead mClimateDataHead;
    ClimateDataMain mClimateDataMain;
    CoreController* mController;
    std::string mCheckNum;
};

#endif // _CLIMATE_DATA_HANDLER_H_
