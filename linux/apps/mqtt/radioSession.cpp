#include "radioSession.hpp"

//RadioSession::RadioSession(monitor& mon) : m_monitor(std::make_shared<monitor>(mon))
RadioSession::RadioSession(monitor& mon, uint8_t address) : m_monitor(mon), m_radioAddress(address)
{
    m_wakeupAttempts = 2;
    m_keepAliveInterval = 0;
    m_initialKeepAliveInterval = 4; // 500 ms
    m_isAlive = false;
    m_wakeupSuccessCounter = 0;
    m_wakeupFailedCounter = 0;
    m_activeTime = 0;
    m_timeLastWakeup = 0;
};

void RadioSession::close()
{
    if(m_isAlive)
    {
        uint64_t activeTimeSinceLastWakeup=milliSecondsSinceEpoch() - m_timeLastWakeup;
        
        int initialValidResponses = m_monitor.getValidResponses();
        auto keepAliveReply = m_monitor.getRadio<>(UartCommandKeepAlive(m_keepAliveInterval));
        int validResponsesAfterKeepAlive = m_monitor.getValidResponses();

        if (validResponsesAfterKeepAlive > initialValidResponses) {
            auto response = keepAliveReply.responseStruct();

            if(response.status == 1)
            {
                if(m_keepAliveInterval != 0)
                {
                    m_activeTime = m_activeTime + 100 + m_keepAliveInterval*100; // new keep alive interval
                }
            }

            m_activeTime = m_activeTime + activeTimeSinceLastWakeup;
        }
        else{
            m_activeTime = m_activeTime + 100 + 100*m_initialKeepAliveInterval; // keep alive interval set during initial ping
        }

        m_isAlive = false;

        std::cout << "DEBUG: radioAddress=" << std::to_string(m_radioAddress) 
                  << ", activeTime(ms)=" << std::to_string(m_activeTime) 
                  << ", wakeupSuccess=" << std::to_string(getWakeupSuccessCounter()) 
                  << ", wakeupFailed=" << std::to_string(getWakeupFailedCounter()) 
                  << std::endl;
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

uint64_t RadioSession::getActiveTimeForRadioSlave()
{
    return(m_activeTime);
}

void RadioSession::setKeepAliveInterval(uint8_t interval)
{
    m_keepAliveInterval = interval;
}

bool RadioSession::wakeupNotRespondingTryOnce()
{
    bool status = true;

    int initialInvalidResponses = m_monitor.getInvalidResponses();
    m_monitor.getRadio<>(UartCommandKeepAlive(m_initialKeepAliveInterval), static_cast<std::chrono::milliseconds>(2000));
    int invalidResponsesAfterPing = m_monitor.getInvalidResponses();

    if (invalidResponsesAfterPing > initialInvalidResponses) {
        UartCommandWakeup result = m_monitor.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
        COMMANDS::WAKEUP::response_t response_struct = result.responseStruct();

        if(response_struct.status == 1)
        {
            status=true;
            m_wakeupSuccessCounter++;
            m_timeLastWakeup = milliSecondsSinceEpoch();
        }
        else{
            status=false;
            m_wakeupFailedCounter++;
        }
    }

    m_isAlive = status;

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

std::string RadioSession::getSlaveNameAndPublishBirth(mqtt::async_client& mqtt_client)
{
    std::string slaveName("");

    auto slaveDeviceInfo = m_monitor.getRadio<>(UartCommandGetDeviceInfo());

    if (slaveDeviceInfo.getReplyStatus() != UartCommandBase::ReplyStatus::Complete) {
        slaveDeviceInfo = m_monitor.getRadio<>(UartCommandGetDeviceInfo());
    }

    if (slaveDeviceInfo.getReplyStatus() == UartCommandBase::ReplyStatus::Complete) {
        auto response = slaveDeviceInfo.responseStruct();

        for (int i = 0; i < 16 && response.name[i] != 0; i++) {
            slaveName += response.name[i];
        }

        publishNbirth(mqtt_client, slaveName);
    }

    return (slaveName);
}


