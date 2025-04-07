#include "radioSession.hpp"
#include "desiredStateConfiguration.hpp"

//RadioSession::RadioSession(monitor& mon) : m_monitor(std::make_shared<monitor>(mon))
RadioSession::RadioSession(monitor& mon, uint8_t address) : m_monitor(mon), m_radioAddress(address)
{
    m_wakeupAttempts = 2;
    m_keepAliveInterval = 0;
};

RadioSession::~RadioSession()
{
    m_monitor.getRadio<>(UartCommandKeepAlive(m_keepAliveInterval));
};

void RadioSession::setKeepAliveInterval(uint8_t interval)
{
    m_keepAliveInterval = interval;
}

bool RadioSession::wakeupNotRespondingTryOnce()
{
    bool status = true;

    int initialInvalidResponses = m_monitor.getInvalidResponses();
    m_monitor.getRadio<>(UartCommandDebug(), static_cast<std::chrono::milliseconds>(2000));
    int invalidResponsesAfterPing = m_monitor.getInvalidResponses();

    if (invalidResponsesAfterPing > initialInvalidResponses) {
        UartCommandWakeup result = m_monitor.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
        COMMANDS::WAKEUP::response_t response_struct = result.responseStruct();

        if(response_struct.status == 1)
        {
            status=true;
        }
        else{
            status=false;
        }
    }

    //if (status) {
        //std::cout << "Wake up device: " << std::to_string(m_radioAddress) << " (OK)" << std::endl;
    //}
    //else {
        //std::cout << "Wake up device: " << std::to_string(m_radioAddress) << " (FAILED)" << std::endl;
    //}

    return (status);
}

bool RadioSession::wakeupNotResponding()
{
    uint8_t cnt = 0;

    m_monitor.get<>(UartCommandSetSlaveAddress(m_radioAddress));

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

std::string RadioSession::readSlaveName(monitor& mon)
{
    std::string slaveName;

    auto slaveDeviceInfo = mon.getRadio<>(UartCommandGetDeviceInfo());
    if (slaveDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = slaveDeviceInfo.responseStruct();

        for (int i = 0; i < 16 && response.name[i] != 0; i++) {
            slaveName += response.name[i];
        }
    }

    return (slaveName);
}

