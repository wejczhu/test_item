#ifndef _CLIMATE_DATA_HANDLER_H_
#define _CLIMATE_DATA_HANDLER_H_

#include "DataHandler.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

class ClimateDataHandler: public DataHandler
{
public:
    ClimateDataHandler() = default;
    ~ClimateDataHandler() = default;
    void ParseData(vector<std::string> data) override;
    
    void PrintDataHead();
    void PrintDataMain();

private:
    ClimateDataHead mClimateDataHead;
    ClimateDataMain mClimateDataMain;
    std::string mCheckNum;
};

#endif // _CLIMATE_DATA_HANDLER_H_
