#include <radioSession.hpp>

uint64_t RadioSession::milliSecondsSinceEpoch()
{
    using namespace std::chrono;
    uint64_t seconds
        = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())
                .count());
    return seconds;
}

uint64_t RadioSession::secondsSinceEpoch()
{
    return milliSecondsSinceEpoch()/1000;
}

void RadioSession::setVerbose(bool verbose)
{
    m_verbose = verbose;
}

RadioSession::RadioSession(monitor& mon, uint8_t address) : m_monitor(mon), m_radioAddress(address)
{
    m_wakeupAttempts = 3;
    m_keepAliveInterval = 0;
    m_initialKeepAliveInterval = 0; // 100 ms
    m_isAlive = false;
    m_wakeupSuccessCounter = 0;
    m_wakeupFailedCounter = 0;
    m_activeTime = 0;
    m_timeLastWakeup = 0;
    m_verbose = false;
};

void RadioSession::close()
{
    if(m_isAlive)
    {
        uint64_t activeTimeSinceLastWakeup=milliSecondsSinceEpoch() - m_timeLastWakeup;
        
        m_monitor.getRadio<>(UartCommandKeepAlive(m_keepAliveInterval));

        if(m_monitor.lastCommandReturnedValidResponse()) {
            if(m_keepAliveInterval != 0)
            {
                m_activeTime = m_activeTime + 100 + m_keepAliveInterval*100; // new keep alive interval
            }

            m_activeTime = m_activeTime + activeTimeSinceLastWakeup;
        }
        else{
            m_activeTime = m_activeTime + 100 + 100*m_initialKeepAliveInterval; // keep alive interval set during initial ping
        }

        m_isAlive = false;

        if (m_verbose) {
            std::cout << "DEBUG: radioAddress=" << std::to_string(m_radioAddress)
                      << ", activeTime(ms)=" << std::to_string(m_activeTime)
                      << ", wakeupSuccess=" << std::to_string(getWakeupSuccessCounter())
                      << ", wakeupFailed=" << std::to_string(getWakeupFailedCounter())
                      << std::endl;
        }
    }
}

uint32_t RadioSession::getWakeupSuccessCounter()
{
    return (m_wakeupSuccessCounter);
}

uint32_t RadioSession::getWakeupFailedCounter()
{
    return (m_wakeupFailedCounter);
}

uint64_t RadioSession::getActiveTimeForRadioNode()
{
    return(m_activeTime);
}

void RadioSession::setKeepAliveInterval(uint8_t interval)
{
    m_keepAliveInterval = interval;
}

bool RadioSession::wakeupNotRespondingTryOnce()
{
    m_isAlive = false;

    m_monitor.getRadio<>(UartCommandPing(), static_cast<std::chrono::milliseconds>(500));

    if (m_monitor.lastCommandReturnedValidResponse()) {
        if (m_verbose) {
            std::cout << "DEBUG: radio node responded to ping, no need to wake up" << std::endl;
        }
        m_isAlive = true;
    } else {
        UartCommandWakeup result = m_monitor.get<>(UartCommandWakeup(false), static_cast<std::chrono::milliseconds>(6000));

        if (m_monitor.lastCommandReturnedValidResponse()) {
            m_isAlive = true;
            m_wakeupSuccessCounter++;
            m_timeLastWakeup = milliSecondsSinceEpoch();
        }
        else {
            m_wakeupFailedCounter++;
        }
    }

    if (m_verbose) {
        if (m_isAlive) {
            std::cout << "DEBUG: Wake up device: " << std::to_string(m_radioAddress) << " (OK)" << std::endl;
        }
        else {
            std::cout << "DEBUG: Wake up device: " << std::to_string(m_radioAddress) << " (FAILED)" << std::endl;
        }
    }

        return (m_isAlive);
    }

bool RadioSession::wakeupNotResponding()
{
    uint8_t cnt = 0;

    m_monitor.get<>(UartCommandSetNodeAddress(m_radioAddress));

    while(cnt<=m_wakeupAttempts)
    {
        cnt++;
        if(wakeupNotRespondingTryOnce())
        {
            return(true);
        }
    }

    std::this_thread::sleep_for(10ms);

    return(false);
}

std::string RadioSession::readNodeName(monitor& mon)
{
    std::string nodeName;

    auto nodeDeviceInfo = mon.getRadio<>(UartCommandGetDeviceInfo());
    if (nodeDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = nodeDeviceInfo.responseStruct();

        for (int i = 0; i < sizeof(response.nameString) && response.nameString[i] != 0; i++) {
            nodeName += response.nameString[i];
        }
    }

    return (nodeName);
}

std::string RadioSession::getNodeName()
{
    std::string nodeName("");

    auto nodeDeviceInfo = m_monitor.getRadio<>(UartCommandGetDeviceInfo());

    if (nodeDeviceInfo.getReplyStatus() != UartCommandBase::ReplyStatus::Complete) {
        nodeDeviceInfo = m_monitor.getRadio<>(UartCommandGetDeviceInfo());
    }

    if (nodeDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = nodeDeviceInfo.responseStruct();

        // refactor this: use getNameString() when it is completed
        for (int i = 0; i < sizeof(response.nameString) && response.nameString[i] != 0; i++) {
            nodeName += response.nameString[i];
        }
    }

    return (nodeName);
}


