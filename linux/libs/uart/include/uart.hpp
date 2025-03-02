#pragma once

#include <termios.h>
#include <unistd.h>

#include <epollevent.hpp>
#include <uart.hpp>

#include <fcntl.h>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <vector>

#include <termios.h>
#include <thread>
#include <unistd.h>

#define READ_CHUNK_SIZE 1024

struct control_modes {
    int parity;
    int stop;
    int bits;
    int hw_flow;
    int hangup_on_close;
};

enum class Status : int
{
    Success,
    Exit,
    Failure,
    Timeout,
};

using ReadHandler = std::function<void(std::vector<uint8_t>& data)>;

class Uart : public EpollEvent {
public:
    Uart(
        std::string name,
        int baudrate = 1000000,
        control_modes modes
        = { .parity = 1, .stop = 2, .bits = 8, .hw_flow = 0, .hangup_on_close = 1 });

    int getFd();

    void write(const std::vector<uint8_t>& data) const;
    std::vector<uint8_t> read();
    void setReadHandler(ReadHandler readhandler);
    void printDebug(bool d);
    void setExtraSleep();

    void execute() override;
    void open();
    void close();

private:
    void configure();

    std::string m_name;
    int m_baudrate;
    control_modes m_mode;
    int m_fd;
    ReadHandler m_readhandler = nullptr;
    bool m_printDebug = false;
    bool m_extraSleep = false;
};

