// Copyright (C) 2022 - All Rights Reserved

#include "Uart.h"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <exception>
#include <iostream>
#include <asm/termbits.h>
#include <sys/ioctl.h>

namespace LinuxSerial
{

SerialPort::SerialPort()
{
    mEcho = false;
    mTimeout = mDefaultTimeout;
    mBaudRateType = BaudRateType::STANDARD;
    mBaudRateStandard = mDefaultBaudRate;
    mReadBufferSize = mDefaultReadBufferSize;
    mReadBuffer.reserve(mReadBufferSize);
    mState = State::CLOSED;
}

SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::SetDevice(const std::string& device)
{
    mDevice = device;
    if(mState == State::OPEN)
    {
        ConfigureTermios();
    }

    return true;
}

bool SerialPort::SetBaudRate(BaudRate baudRate)
{   
    mBaudRateType = BaudRateType::STANDARD;
    mBaudRateStandard = baudRate;
    if(mState == State::OPEN)
    {
        ConfigureTermios();
    }

    return true;
}

bool SerialPort::SetBaudRate(speed_t baudRate)
{
    mBaudRateType = BaudRateType::CUSTOM;
    mBaudRateCustom = baudRate;
    if(mState == State::OPEN)
    {
        ConfigureTermios();
    }
    
    return true;
}

bool SerialPort::SetParity(Parity parity)
{
    mParity = parity;
    if(mState == State::OPEN)
    {
        ConfigureTermios();
    }
    
    return true;
}

bool SerialPort::SetNumStopBits(NumStopBits numStopBits)
{
    mNumStopBits = numStopBits;
    if(mState == State::OPEN)
    {
        ConfigureTermios();
    }
    
    return true;
}

bool SerialPort::SetAllConfig(std::string baudRate, std::string parity, std::string numDataBits, std::string numStopBits)
{
    // Convert baudrate to enum BaudRate
    BaudRate newBaudRate = BaudRate::B_0;
    Parity newParity = Parity::NONE;
    NumDataBits newNumDataBits = NumDataBits::EIGHT;
    NumStopBits newNumStopBits = NumStopBits::ONE;

    std::cout << baudRate << std::endl;
    std::cout << parity << std::endl;
    std::cout << numDataBits << std::endl;
    std::cout << numStopBits << std::endl;

    if(baudRate == "1200")
    {
        newBaudRate = BaudRate::B_1200;
    }
    else if(baudRate == "2400")
    {
        newBaudRate = BaudRate::B_2400;
    }
    else if(baudRate == "4800")
    {
        newBaudRate = BaudRate::B_4800;
    }
    else if(baudRate == "9600")
    {
        newBaudRate = BaudRate::B_9600;
    }
    else if(baudRate == "19200")
    {
        newBaudRate = BaudRate::B_19200;
    }
    else if(baudRate == "38400")
    {
        newBaudRate = BaudRate::B_38400;
    }
    else if(baudRate == "57600")
    {
        newBaudRate = BaudRate::B_57600;
    }
    else if(baudRate == "115200")
    {
        newBaudRate = BaudRate::B_115200;
    }
    else
    {
        throw std::runtime_error("Invalid baudrate");
    }


    if(parity == "N")
    {
        newParity = Parity::NONE;
    }
    else if(parity == "E")
    {
        newParity = Parity::EVEN;
    }
    else if(parity == "O")
    {
        newParity = Parity::ODD;
    }
    else
    {
        throw std::runtime_error("Invalid parity");
    }

    if(numDataBits == "5")
    {
        newNumDataBits = NumDataBits::FIVE;
    }
    else if(numDataBits == "6")
    {
        newNumDataBits = NumDataBits::SIX;
    }
    else if(numDataBits == "7")
    {
        newNumDataBits = NumDataBits::SEVEN;
    }
    else if(numDataBits == "8")
    {
        newNumDataBits = NumDataBits::EIGHT;
    }
    else
    {
        throw std::runtime_error("Invalid number of data bits");
    }

    if(numStopBits == "1")
    {
        newNumStopBits = NumStopBits::ONE;
    }
    else if(numStopBits == "2")
    {
        newNumStopBits = NumStopBits::TWO;
    }
    else
    {
        throw std::runtime_error("Invalid number of stop bits");
    }

    mBaudRateStandard = newBaudRate;
    mParity = newParity;
    mNumDataBits = newNumDataBits;
    mNumStopBits = newNumStopBits;

    if(mState == State::OPEN)
    {
        ConfigureTermios();
    }
    return true;
}

bool SerialPort::Open()
{
    if(mDevice.empty()) {
        std::cerr << "No device selected, please set a valid device " << std::endl;
        return false;
    }

    mFileDescriptor = open(mDevice.c_str(), O_RDWR);
    if(mFileDescriptor == -1) {
        perror("open");
        return false;
    }

    ConfigureTermios();
    mState = State::OPEN;

    return true;
}

bool SerialPort::SetEcho(bool isEcho)
{
    mEcho = isEcho;

    return true;
}

void SerialPort::SetTimeout(int32_t timeout_ms) {
    if(timeout_ms < -1)
    {

    }
        //THROW_EXCEPT(std::string() + "timeout_ms provided to " + __PRETTY_FUNCTION__ + " was < -1, which is invalid.");
    if(timeout_ms > 25500)
    {

    }
        //THROW_EXCEPT(std::string() + "timeout_ms provided to " + __PRETTY_FUNCTION__ + " was > 25500, which is invalid.");
    if(mState == State::OPEN)
    {

    }
        //THROW_EXCEPT(std::string() + __PRETTY_FUNCTION__ + " called while state == OPEN.");
    mTimeout = timeout_ms;
}

void SerialPort::ConfigureTermios()
{
    termios2 tty = GetTermios2();

    tty.c_cflag &= ~CSIZE;
    switch(mNumDataBits)
    {
        case NumDataBits::FIVE:
            tty.c_cflag |= CS5;
            break;
        case NumDataBits::SIX:
            tty.c_cflag |= CS6;
            break;
        case NumDataBits::SEVEN:
            tty.c_cflag |= CS7;
            break;
        case NumDataBits::EIGHT:
            tty.c_cflag |= CS8;
            break;
        default:
            break;
            //THROW_EXCEPT("NumDataBits value not supported!");
    }

    switch(mParity)
    {
        case Parity::NONE:
            tty.c_cflag &= ~PARENB;
            break;
        case Parity::EVEN:
            tty.c_cflag |= PARENB;
            tty.c_cflag &= ~PARODD;
            break;
        case Parity::ODD:
            tty.c_cflag |= PARENB;
            tty.c_cflag |= PARODD;
            break;
        default:
            break;
            //THROW_EXCEPT("Parity value not supported!");
    }

    // Set num. stop bits
    switch(mNumStopBits)
    {
        case NumStopBits::ONE:
            tty.c_cflag &= ~CSTOPB;
            break;
        case NumStopBits::TWO:
            tty.c_cflag |= CSTOPB;
            break;
        default:
            break;
            //THROW_EXCEPT("NumStopBits value not supported!");
    }

    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    if (mBaudRateType == BaudRateType::STANDARD) {
        tty.c_cflag &= ~CBAUD;
        tty.c_cflag |= CBAUDEX;
        switch(mBaudRateStandard)
        {
            case BaudRate::B_0:
                tty.c_ispeed = 0;
                tty.c_ospeed = 0;
                break;
            case BaudRate::B_50:
                tty.c_ispeed = 50;
                tty.c_ospeed = 50;
                break;
            case BaudRate::B_75:
                tty.c_ispeed = 75;
                tty.c_ospeed = 75;
                break;
            case BaudRate::B_110:
                tty.c_ispeed = 110;
                tty.c_ospeed = 110;
                break;
            case BaudRate::B_134:
                tty.c_ispeed = 134;
                tty.c_ospeed = 134;
                break;
            case BaudRate::B_150:
                tty.c_ispeed = 150;
                tty.c_ospeed = 150;
                break;
            case BaudRate::B_200:
                tty.c_ispeed = 200;
                tty.c_ospeed = 200;
                break;
            case BaudRate::B_300:
                tty.c_ispeed = 300;
                tty.c_ospeed = 300;
                break;
            case BaudRate::B_600:
                tty.c_ispeed = 600;
                tty.c_ospeed = 600;
                break;
            case BaudRate::B_1200:
                tty.c_ispeed = 1200;
                tty.c_ospeed = 1200;
                break;
            case BaudRate::B_1800:
                tty.c_ispeed = 1800;
                tty.c_ospeed = 1800;
                break;
            case BaudRate::B_2400:
                tty.c_ispeed = 2400;
                tty.c_ospeed = 2400;
                break;
            case BaudRate::B_4800:
                tty.c_ispeed = 4800;
                tty.c_ospeed = 4800;
                break;
            case BaudRate::B_9600:
                tty.c_ispeed = 9600;
                tty.c_ospeed = 9600;
                break;
            case BaudRate::B_19200:
                tty.c_ispeed = 19200;
                tty.c_ospeed = 19200;
                break;
            case BaudRate::B_38400:
                tty.c_ispeed = 38400;
                tty.c_ospeed = 38400;
                break;
            case BaudRate::B_57600:
                tty.c_ispeed = 57600;
                tty.c_ospeed = 57600;
                break;
            case BaudRate::B_115200:
                tty.c_ispeed = 115200;
                tty.c_ospeed = 115200;
                break;
            case BaudRate::B_230400:
                tty.c_ispeed = 230400;
                tty.c_ospeed = 230400;
                break;
            case BaudRate::B_460800:
                tty.c_ispeed = 460800;
                tty.c_ospeed = 460800;
                break;
            default:
                throw std::runtime_error(std::string() + "baudRate passed to " + __PRETTY_FUNCTION__ + " unrecognized.");
        }
    }

    // them separate for now....
    else if (mBaudRateType == BaudRateType::CUSTOM)
    {
        tty.c_cflag &= ~CBAUD;
        tty.c_cflag |= CBAUDEX;
        tty.c_ispeed = mBaudRateCustom;
        tty.c_ospeed = mBaudRateCustom;
    }
    else
    {
        perror("termios config");
        exit(-1);
    }

    tty.c_oflag = 0; // No remapping, no delays
    tty.c_oflag &= ~OPOST; // Make raw

    if(mTimeout == -1)
    {
        // Always wait for at least one byte, this could
        // block infinitely
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 1;
    }
    else if(mTimeout == 0)
    {
        // Setting both to 0 will give a non-blocking read
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 0;
    }
    else if(mTimeout > 0)
    {
        tty.c_cc[VTIME] = (cc_t)(mTimeout/100);    // 0.5 seconds read timeout
        tty.c_cc[VMIN] = 0;
    }

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

    tty.c_lflag &= ~ICANON; // Turn off canonical input, which is suitable for pass-through
    // Configure echo depending on echo_ boolean
    if(mEcho)
    {
        tty.c_lflag |= ECHO;
    }
    else
    {
        tty.c_lflag &= ~(ECHO);
    }
    tty.c_lflag &= ~ECHOE; // Turn off echo erase (echo erase only relevant if canonical input is active)
    tty.c_lflag &= ~ECHONL; //
    tty.c_lflag &= ~ISIG; // Disables recognition of INTR (interrupt), QUIT and SUSP (suspend) characters

    this->SetTermios2(tty);
}

bool SerialPort::Write(const std::string& data)
{
    if(mState != State::OPEN)
    {
        std::cerr << "The serial port is not opened, please open is first!" << std::endl;
        return false;
    }

    if(mFileDescriptor < 0)
    {
        std::cerr << "File descriptor is invalid, can not write to serial port!" << std::endl;
    }

    int writeResult = write(mFileDescriptor, data.c_str(), data.size());

    // Check status
    if (writeResult == -1) {
        throw std::system_error(EFAULT, std::system_category());
    }

    return true;
}

bool SerialPort::Read(std::string& data)
{
    data.clear();

    if(mFileDescriptor == 0) {
        std::cerr << "File descriptor is invalid, can not read from serial port" << std::endl;
        return false;
    }

    ssize_t length = read(mFileDescriptor, &mReadBuffer[0], mReadBufferSize);

    // Delay 20ms for read buffer
    usleep(READ_DELAY);
    // Error Handling
    if(length < 0) {
        // Read was unsuccessful
        perror("Read");
        return false;
    }
    else if(length > 0) {
        data = std::string(&mReadBuffer[0], length);
    }

    return true;
}

termios2 SerialPort::GetTermios2()
{
    struct termios2 term2;
    ioctl(mFileDescriptor, TCGETS2, &term2);

    return term2;
}

void SerialPort::SetTermios2(termios2 tty)
{
    ioctl(mFileDescriptor, TCSETS2, &tty);
}

void SerialPort::Close() {

    if(mFileDescriptor != -1) {
        auto retVal = close(mFileDescriptor);
        if(retVal != 0)
        {
            perror("Close");
        }
        mFileDescriptor = -1;
    }

    mState = State::CLOSED;
}

speed_t SerialPort::GetBaudRate()
{
    // Get baud rate
    termios2 tty = GetTermios2();
    return tty.c_ispeed;
}


// TODO:
// Calculate the NumDataBits properly
tcflag_t SerialPort::GetNumDataBits()
{
    switch(mNumDataBits)
    {
        case NumDataBits::FIVE:
            return 5;
            break;
        case NumDataBits::SIX:
            return 6;
            break;
        case NumDataBits::SEVEN:
            return 7;
            break;
        case NumDataBits::EIGHT:
            return 8;
            break;
        default:
            return 0;
            break;
            //THROW_EXCEPT("NumDataBits value not supported!");
    }

    return 0;
}

std::string SerialPort::GetParity()
{
    switch(mParity)
    {
        case Parity::NONE:
            return "N";
            break;
        case Parity::EVEN:
            return "E";
            break;
        case Parity::ODD:
            return "O";
            break;
        default:
            return "N";
            break;
            //THROW_EXCEPT("Parity value not supported!");
    }

    return "N";
}

tcflag_t SerialPort::GetNumStopBits()
{
    switch(mNumStopBits)
    {
        case NumStopBits::ONE:
            return 1;
            break;
        case NumStopBits::TWO:
            return 2;
            break;
        default:
            return 0;
            break;
            //THROW_EXCEPT("NumStopBits value not supported!");
    }

    return 0;
}

} // namespace LinuxSerial
