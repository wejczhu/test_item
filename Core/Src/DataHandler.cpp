#include "DataHandler.h"

#include <iostream>

std::vector<std::string> DataHandler::SplitDataFrame(std::string data)
{
    // split data by comma
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    std::vector<std::string> data_vector;
    while ((pos = data.find(delimiter)) != std::string::npos)
    {
        token = data.substr(0, pos);
        data_vector.push_back(token);
        data.erase(0, pos + delimiter.length());
    }
    data_vector.push_back(data);

    return data_vector;
}
