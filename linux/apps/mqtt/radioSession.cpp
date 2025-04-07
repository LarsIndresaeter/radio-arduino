#include "radioSession.hpp"
#include "desiredStateConfiguration.hpp"

//RadioSession::RadioSession(monitor& mon) : m_monitor(std::make_shared<monitor>(mon))
RadioSession::RadioSession(uint8_t address) : m_radioAddress(address)
{
};

bool RadioSession::wakeupNotRespondingTryOnce(monitor& mon, uint8_t address)
{
    bool status = true;

    int initialInvalidResponses = mon.getInvalidResponses();
    mon.getRadio<>(UartCommandDebug(), static_cast<std::chrono::milliseconds>(2000));
    int invalidResponsesAfterPing = mon.getInvalidResponses();

    if (invalidResponsesAfterPing > initialInvalidResponses) {
        UartCommandWakeup result = mon.get<>(UartCommandWakeup(), static_cast<std::chrono::milliseconds>(12000));
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
        //std::cout << "Wake up device: " << std::to_string(address) << " (OK)" << std::endl;
    //}
    //else {
        //std::cout << "Wake up device: " << std::to_string(address) << " (FAILED)" << std::endl;
    //}

    return (status);
}

bool RadioSession::wakeupNotResponding(monitor& mon, uint8_t address, uint8_t attempts)
{
    uint8_t cnt = 0;

    mon.get<>(UartCommandSetSlaveAddress(address));

    while(cnt<=attempts)
    {
        cnt++;
        if(wakeupNotRespondingTryOnce(mon, address))
        {
            return(true);
        }
    }

    std::this_thread::sleep_for(10ms);

    return(false);
}


