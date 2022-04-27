// Authors: Wei Jianxing <Jx.Wei@outlook.com>

#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include <string>
#include <vector>
#include <asm/termbits.h>

namespace LinuxSerial
{

enum class BaudRateType {
    STANDARD,
    CUSTOM,
};

enum class BaudRate {
    B_0,
    B_50,
    B_75,
    B_110,
    B_134,
    B_150,
    B_200,
    B_300,
    B_600,
    B_1200,
    B_1800,
    B_2400,
    B_4800,
    B_9600,
    B_19200,
    B_38400,
    B_57600,
    B_115200,
    B_230400,
    B_460800,
    B_CUSTOM, // Placeholder
};

enum class NumDataBits {
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
};

enum class Parity {
    NONE,
    EVEN,
    ODD,
};

enum class NumStopBits {
    ONE,
    TWO,
};

enum class State {
    CLOSED,
    OPEN,
};

class SerialPort
{
public:
    // Constructor
    SerialPort();

    // Destructor
    SerialPort(const std::string &device, BaudRate baudRate);

    SerialPort(const std::string &device, BaudRate baudRate, NumDataBits numDataBits, Parity parity, NumStopBits numStopBits);

    SerialPort(const std::string &device, speed_t baudRate);

    virtual ~SerialPort();

    // Open com port
    bool Open();

    // Close com port
    void Close();

    // Write data to com port
    bool Write(const std::string& data);

    // Read data from com port
    bool Read(std::string& data);

    // Set the timeout
    void SetTimeout(int32_t timeout_ms);

    bool SetDevice(const std::string &device);

    // Set a standard baud rate
    bool SetBaudRate(BaudRate baudRate);

    // Set a custom baud rate
    bool SetBaudRate(speed_t baudRate);

    // Set the number of data bits
    bool SetNumDataBits(NumDataBits numDataBits);

    // Set the parity
    bool SetParity(Parity parity);

    // Set the num stop bits
    bool SetNumStopBits(NumStopBits numStopBits);

    // Set if Echo or not
    bool SetEcho(bool isEcho);

private:
    void ConfigureTermios();

    termios2 GetTermios2();

    void SetTermios2(termios2 tty);

    State mState;

    std::string mDevice;

    BaudRateType mBaudRateType;

    BaudRate mBaudRateStandard;

    speed_t mBaudRateCustom;

    NumDataBits mNumDataBits = NumDataBits::EIGHT;

    Parity mParity = Parity::NONE;

    NumStopBits mNumStopBits = NumStopBits::ONE;

    int mFileDescriptor;

    bool mEcho;

    // Timeout miliseconds
    int32_t mTimeout;

    std::vector<char> mReadBuffer;
    unsigned int mReadBufferSize;

    static constexpr BaudRate mDefaultBaudRate = BaudRate::B_115200;
    static constexpr int32_t mDefaultTimeout = -1;
    //static constexpr unsigned char mDefaultReadBufferSize = 255;
    static constexpr unsigned int mDefaultReadBufferSize = 512;
    static constexpr int READ_DELAY = 20000;
};

} // namespace LinuxSerial

#endif // _SERIAL_PORT_H_