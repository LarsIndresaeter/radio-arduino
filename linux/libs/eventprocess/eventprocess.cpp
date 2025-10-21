
#include <eventprocess.hpp>

#define UNLIKELY(x) __builtin_expect(x, 0)

EventProcess::EventProcess(std::function<void(int)> tick, int max_events, int timeout)
    : events(new struct epoll_event[max_events])
    , m_timeout(timeout)
    , max_events(max_events)
{
    epoll_fd = epoll_create1(0);
}

EventProcess::~EventProcess() { close(epoll_fd); }

void EventProcess::setTickFunction(std::function<void()> cb) { tick = cb; }

void EventProcess::addEvent(EpollEvent* ev, int events)
{
    struct epoll_event event;
    event.events = events;

    // std::cout << "EventProcess::addEvent() fd:" << static_cast<int>(ev->getFd()) << std::endl;

    event.data.u64 = (uint64_t)(ev);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev->getFd(), &event) != 0) {
        throw std::runtime_error("Failed to add fd");
    }
}

void EventProcess::removeEvent(EpollEvent* ev)
{
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev->getFd(), NULL) != 0) {
        throw std::runtime_error("Failed to remove fd");
    }
}

void EventProcess::Run()
{
    while (1) {
        struct epoll_event* event_list = events.get();
        int count
            = epoll_wait(epoll_fd, event_list, max_events, m_timeout); // wait for an I/O event on an epoll descriptor
        // std::cout << "EventProcess::Run() count:" << static_cast<int>(count) << " epoll_fd: " <<
        // static_cast<int>(epoll_fd) << std::endl;
        if (UNLIKELY(count <= 0)) {
            if (UNLIKELY(count < 0)) {
                // something really bad has happened
            }
            else {
                // std::cout << "EventProcess tick" << std::endl;
                //  run CB on tick on tick!!
                if (tick != nullptr) {
                    tick();
                }
            }
        }
        else {
            // call event function in all registered events
            for (int i = 0; i < count; i++) {
                EpollEvent* ev = (EpollEvent*)event_list[i].data.u64;
                ev->execute();
            }
        }
    }
}
