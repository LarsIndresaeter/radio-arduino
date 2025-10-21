#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>

#include <epollevent.hpp>

class EventProcess {
public:
    EventProcess(std::function<void(int)> tick = nullptr, int max_events = 50, int timeout = 2000);

    ~EventProcess();

    void setTickFunction(std::function<void()> cb);
    void addEvent(EpollEvent* ev, int events);
    void removeEvent(EpollEvent* ev);
    void Run();

private:
    std::unique_ptr<struct epoll_event[]> events;
    int m_timeout;
    int max_events;
    int epoll_fd;
    std::function<void()> tick = nullptr;
};
