// Authors: Wei Jianxing <Jx.Wei@outlook.com>

#include "DataHandler.h"
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

using namespace Storage;

bool DataHandler::Write(int fd, std::string data)
{
    write(fd, &data, data.length());
    return true;
}

bool DataHandler::Read(std::string& data)
{
    return true;
}

