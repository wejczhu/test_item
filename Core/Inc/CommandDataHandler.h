#ifndef _COMMAND_DATA_HANDLER_H_
#define _COMMAND_DATA_HANDLER_H_

#include "DataHandler.h"

using namespace std;

class CommandDataHandler: public DataHandler
{
public:
    CommandDataHandler() = default;
    ~CommandDataHandler() = default;
    void ParseData(vector<std::string> data) override;
    
    //void PrintDataHead();
    //void PrintDataMain();

private:
    //ClimateDataHead mClimateDataHead;
    //ClimateDataMain mClimateDataMain;
    //std::string mCheckNum;
};

#endif // _COMMAND_DATA_HANDLER_H_
