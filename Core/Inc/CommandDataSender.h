// Copyright (C) 2022 - All Rights Reserved

// Authors : Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _COMMAND_DATA_SENDER_H_
#define _COMMAND_DATA_SENDER_H_


// Send command data to sensor
class CommandDataSender
{
public:
    CommandDataSender();
    ~CommandDataSender();

    void SendCommandData(std::string command);

    
};

#endif // _COMMAND_DATA_SENDER_H_