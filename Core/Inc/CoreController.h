// Authors: Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _CORE_CONTROLLER_H_
#define _CORE_CONTROLLER_H_

#include <iostream>

class CoreController
{
public:

    CoreController() = default;
    ~CoreController() = default;

    CoreController& operator=(const CoreController&) = delete;
    CoreController(const CoreController&) = delete;

    static CoreController& GetInstance();

private:
    static CoreController* mInstance;
    static constexpr char* mID = "000";


    


};






#endif // _CORE_CONTROLLER_H_