#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <thread>



#define COMMAND_BADCOMMAND  "Bad Command"
#define COMMAND_READ_DATA  "READDATA"
#define COMMAND_SET_COM_PARAMETER  "SETCOM"
#define COMMAND_SET_COM_WAY  "SETCOMWAY"
#define COMMAND_AUTO_CHECK  "AUTOCHECK"
#define COMNAND_HELP  "HELP"
#define COMMAND_EQUIPMENT_ZONE_NUMBER  "QZ"
#define COMMAND_SERVICE_TYPE  "ST"
#define COMMAND_EQUIPMENT_I  "DI"
#define COMMAND_EQUIPMENT_ID  "ID"
#define COMMAND_LATITUDE  "LAT"
#define COMMAND_LONGITUDE  "LONG"
#define COMMAND_DATE  "DATE"
#define COMMAND_TIME  "TIME"
#define COMMAND_DATE_AND_TIME  "DATETIME"
#define COMMAND_FACILITY_TIME_INTERVAL  "FTD"
#define COMMAND_DOWNLOAD_HISTORY  "DOWN"

#define COMMAND_RETURN_SUCCESS  "T"
#define COMMAND_RETURN_FAILURE  "F"


// Extended monitor command 



// enum class ComCommandE
// {
//     BADCOMMAND = "123g", // Bad command
//     READDATA,
//     SETCOM,
//     SETCOMWAY,
//     AUTOCHECK,
//     HELP, // Show help info
//     QZ,
//     ST,
//     DI, // Read equipment ID
//     ID,
//     LAT, // Read latitude
//     LONG,
//     DATE,
//     TIME,
//     DATETIME,
//     FTD,
//     DOWN,
// };



class Ethernet
{
public:
    Ethernet();
    ~Ethernet();

    bool Initialize();
    in_addr_t GetLocalIP();
    void Send(const char* data, int len);
    //void Receive(char* data, int len);

    void Run();
    void Terminate();
private:
    static Ethernet* mInstance;
    static constexpr int mServerPort = 8080;
    std::thread* mThread;
    bool mIsRunning;
    int mSocketFd;
    int mClientFd;
    struct sockaddr_in mServerAddr;
    struct sockaddr_in mClientAddr;
    socklen_t mClientAddrLen;
    char mBuffer[1024];

// private:
//     static constexpr char* COMMAND_BADCOMMAND = "Bad Command";
//     static constexpr char* COMMAND_READDATA = "READDATA";
//     static constexpr char* COMMAND_SET_COM_PARAMETER = "SETCOM";
//     static constexpr char* COMMAND_SETCOMWAY = "SETCOMWAY";
//     static constexpr char* COMMAND_AUTO_CHECK = "AUTOCHECK";
//     static constexpr char* COMNAND_HELP = "HELP";
//     static constexpr char* COMMAND_EQUIPMENT_ZONE_NUMBER = "QZ";
//     static constexpr char* COMMAND_BADCOMMAND = "ST";
//     static constexpr char* COMMAND_BADCOMMAND = "DI";
//     static constexpr char* COMMAND_BADCOMMAND = "ID";
//     static constexpr char* COMMAND_BADCOMMAND = "LAT";
//     static constexpr char* COMMAND_BADCOMMAND = "LONG";
//     static constexpr char* COMMAND_BADCOMMAND = "DATE";

//     static constexpr char* COMMAND_BADCOMMAND = "TIME";
//     static constexpr char* COMMAND_BADCOMMAND = "DATETIME";
//     static constexpr char* COMMAND_BADCOMMAND = "FTD";
//     static constexpr char* COMMAND_BADCOMMAND = "DOWN";


//     static constexpr char* COMMAND_RETURN_SUCCESS = "T";
//     static constexpr char* COMMAND_RETURN_FAILURE = "F";
};

#endif // _ETHERNET_H_
