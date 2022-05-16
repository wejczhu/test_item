#ifndef _COMMAND_DATA_HANDLER_H_
#define _COMMAND_DATA_HANDLER_H_

#include "DataHandler.h"
#include "CoreController.h"

class CoreController;

using namespace std;

class CommandDataHandler: public DataHandler
{
public:
    CommandDataHandler(CoreController* controller);
    ~CommandDataHandler() = default;
    void ParseData(vector<std::string> data, std::string originalData) override;
    
    //void PrintDataHead();
    //void PrintDataMain();

private:
    CoreController* mController;
    //ClimateDataHead mClimateDataHead;
    //ClimateDataMain mClimateDataMain;
    //std::string mCheckNum;
};

#endif // _COMMAND_DATA_HANDLER_H_
