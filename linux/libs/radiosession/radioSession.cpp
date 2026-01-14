#include <radioSession.hpp>

uint64_t RadioSession::milliSecondsSinceEpoch()
{
    using namespace std::chrono;
    uint64_t seconds = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    return seconds;
}

uint64_t RadioSession::secondsSinceEpoch() { return milliSecondsSinceEpoch() / 1000; }

void RadioSession::setVerbose(bool verbose) { m_verbose = verbose; }

RadioSession::RadioSession(monitor& mon, uint8_t address)
    : m_monitor(mon)
{
    m_wakeupAttempts = 1;
    m_isAlive = false;
    m_wakeupSuccessCounter = 0;
    m_wakeupFailedCounter = 0;
    m_activeTime = 0;
    m_timeLastWakeup = 0;
    m_verbose = false;
};

void RadioSession::close()
{
    if (m_isAlive) {
        uint64_t activeTimeSinceLastWakeup = milliSecondsSinceEpoch() - m_timeLastWakeup;

        //m_monitor.getRadio<>(RaduinoCommandKeepAlive(0)); // tell radio node to go do sleep

        if (m_monitor.lastCommandReturnedValidResponse()) {
            m_activeTime = m_activeTime + activeTimeSinceLastWakeup;
        }
        else {
            m_activeTime
                = m_activeTime + 1000; // keep alive interval set during initial ping
        }

        m_isAlive = false;

        if (m_verbose) {
            std::cout << ", activeTime(ms)=" << std::to_string(m_activeTime)
                      << ", wakeupSuccess=" << std::to_string(getWakeupSuccessCounter())
                      << ", wakeupFailed=" << std::to_string(getWakeupFailedCounter()) << std::endl;
        }
    }
}

uint32_t RadioSession::getWakeupSuccessCounter() { return (m_wakeupSuccessCounter); }

uint32_t RadioSession::getWakeupFailedCounter() { return (m_wakeupFailedCounter); }

uint64_t RadioSession::getActiveTimeForRadioNode() { return (m_activeTime); }

bool RadioSession::wakeupNotRespondingTryOnce()
{
    m_isAlive = false;

    m_monitor.getRadio<>(RaduinoCommandPing(), static_cast<std::chrono::milliseconds>(500));

    if (m_monitor.lastCommandReturnedValidResponse()) {
        if (m_verbose) {
            std::cout << "DEBUG: radio node responded to ping, no need to wake up" << std::endl;
        }
        m_isAlive = true;
    }
    else {
        auto response = m_monitor.get<>(RaduinoCommandWakeup(false, 0), static_cast<std::chrono::milliseconds>(10000));

        if (response.responseStruct().getDiscovered() == 1) {
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
            std::cout << "DEBUG: Wake up device: (OK)" << std::endl;
        }
        else {
            std::cout << "DEBUG: Wake up device: (FAILED)" << std::endl;
        }
    }

    return (m_isAlive);
}

bool RadioSession::wakeupNotResponding()
{
    uint8_t cnt = 0;

    while (cnt < m_wakeupAttempts) {
        cnt++;
        if (wakeupNotRespondingTryOnce()) {
            return (true);
        }
    }

    return (false);
}
