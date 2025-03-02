#pragma once

class EpollEvent {
public:
    int getFd();
    virtual void execute() = 0;

protected:
    int m_fd;
};
